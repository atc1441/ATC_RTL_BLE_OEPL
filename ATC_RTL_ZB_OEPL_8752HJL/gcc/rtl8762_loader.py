#!/usr/bin/env python3
"""
RTL8762ESL UART Bootloader Tool
Loads 2nd-stage loader into RAM via the ROM bootloader HCI
protocol, launches it, then uses the 2nd-stage loader protocol to flash a binary.
"""

import argparse
import struct
import sys
import time
from pathlib import Path
from typing import Optional, Tuple

try:
    import serial
except ImportError:
    sys.exit("pyserial not found — run: pip install pyserial")

try:
    from Crypto.Cipher import AES as _AES
    _AES_AVAILABLE = True
except ImportError:
    _AES_AVAILABLE = False

# ── Constants ────────────────────────────────────────────────────────────────

BAUDRATE      = 115200
CHUNK_PAYLOAD = 252     # Stage-1: FW bytes per HCI 0xFC20 packet

# Stage-1: ROM bootloader HCI opcodes
OP_GET_VERSION = 0x1001
OP_ROM_READ    = 0xFC2D
OP_SET_ADDR    = 0xFC17
OP_DOWNLOAD    = 0xFC20
OP_LAUNCH      = 0xFC62

# Stage-1: ROM constants (RTL8762E specific)
ROM_QUERY_ADDR   = 0x00019000
ROM_QUERY_LEN    = 4
FW_DOWNLOAD_ADDR = 0x0252C014

# Stage-2: flash protocol constants
#
# Packet wire format (Handle_UART_CMD @ 0x207304):
#   [0x87][cmd_lo][cmd_hi][addr_b0..b3 LE][count_b0..b3 LE][data...][crc16arc_lo crc16arc_hi]
#
# cmd_hi is always 0x10 for all flash commands (0x10xx range).
# Addresses are SoC memory-mapped flash byte addresses (e.g. 0x802000).
# Counts are actual byte counts (must be 256-byte aligned for erase/write,
# and a multiple of 16 for read due to AES).
#
# Command codes (from Handle_UART_CMD switch):
#   0x1030 → erase 4 KB sector at addr (count = 4096)
#   0x1031 → chip erase
#   0x1032 → write pages (count bytes of data follow)
#   0x1033 → read flash (response carries count bytes; AES-encrypted)
#   0x1035 → erase section / 64 KB block (count = 65536)
#   0x1041 → reboot (addr_b0 = 0 → normal, 1 → to bootloader)
#   0x1050 → verify (CRC16-ARC checksum in payload)
#
# NOTE: s2_build() uses "page" units (= byte / 256) as a convenient shorthand
# that maps correctly onto the wire format for 256-byte-aligned addresses and
# counts, because addr_b0 and count_b0 are always 0 in that case.

S2_SYNC        = 0x87
S2_CMD_HI      = 0x10              # high byte of all flash command IDs
S2_PROTO_VER   = bytes([S2_CMD_HI, 0x00])  # [cmd_hi, addr_b0=0]
S2_CMD_INIT    = 0x10   # flash connect / init       (full cmd = 0x1010)
S2_CMD_ERASE   = 0x30   # erase 4 KB sector          (0x1030)
S2_CMD_WRITE   = 0x32   # write pages                (0x1032)
S2_CMD_READ    = 0x33   # read flash (AES encrypted) (0x1033)
S2_CMD_SKIP    = 0x35   # erase / skip 64 KB block   (0x1035)
S2_CMD_VERIFY  = 0x50   # CRC16-ARC verify           (0x1050)
S2_CMD_REBOOT  = 0x41   # reboot SoC                 (0x1041)
S2_INIT_PARAMS = bytes([0xC2, 0x01, 0x00, 0xFF])  # init params from sniff

PAGE_SIZE        = 256            # bytes per flash page
SECTOR_SIZE      = 4096           # bytes per erasable sector (4 KB)
PAGES_PER_SECTOR = SECTOR_SIZE // PAGE_SIZE   # = 16 = 0x10

READ_CHUNK_PAGES = PAGES_PER_SECTOR           # 4 KB per read request

ERASE_TIMEOUT  = 5.0
WRITE_TIMEOUT  = 10.0
VERIFY_TIMEOUT = 3.0
READ_TIMEOUT   = 5.0

_AES_KEY_HEX    = "190D8D48E80007B700005234848D4890"
_AES_IV         = bytes(16)          # all-zeros IV, reset per read chunk
_AES_SEGMENT    = 0x4000             # 16 KB AES segment (RTL8762ESL)


# ── CRC helpers ──────────────────────────────────────────────────────────────

def crc16arc(data: bytes) -> int:
    """CRC-16/ARC: poly=0x8005, init=0, refin=True, refout=True, xorout=0."""
    crc = 0
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc


def decrypt_flash_data(data: bytes) -> bytes:
    """
    Decrypt AES-encrypted flash read data (CMD_Read_Flash @ 0x2058a8).
      - Key = _AES_KEY_HEX reversed byte-by-byte
      - IV  = 16 zero bytes, reset every _AES_SEGMENT (0x4000) bytes
      - Per 16-byte block: reverse → AES-CBC decrypt → reverse
    """
    if not _AES_AVAILABLE:
        print("  [warn] pycryptodome not installed — returning raw (encrypted) data")
        print("         Install with: pip install pycryptodome")
        return data

    key = bytes.fromhex(_AES_KEY_HEX)[::-1]   # reversed key
    out = bytearray()
    for seg_off in range(0, len(data), _AES_SEGMENT):
        segment = data[seg_off:seg_off + _AES_SEGMENT]
        # Pad to 16-byte boundary if needed (last segment)
        pad = (-len(segment)) % 16
        if pad:
            segment = segment + b'\xff' * pad

        cipher = _AES.new(key, _AES.MODE_CBC, _AES_IV)

        # Reverse each 16-byte block before decryption
        rev_in = bytearray()
        for i in range(0, len(segment), 16):
            rev_in += segment[i:i + 16][::-1]

        dec = cipher.decrypt(bytes(rev_in))

        # Reverse each 16-byte block of the decrypted result
        for i in range(0, len(dec), 16):
            out += dec[i:i + 16][::-1]

    # Strip padding (return only as many bytes as requested)
    return bytes(out[:len(data)])


# ── Stage-1 HCI helpers ───────────────────────────────────────────────────────

def make_hci_cmd(opcode: int, params: bytes = b'') -> bytes:
    return bytes([0x01]) + struct.pack('<H', opcode) + bytes([len(params)]) + params


def parse_cmd_complete(raw: bytes):
    """Parse HCI Command Complete event. Returns (opcode, status, return_params)."""
    if len(raw) < 7:
        raise ValueError(f"HCI packet too short ({len(raw)} B): {raw.hex(' ')}")
    if raw[0] != 0x04 or raw[1] != 0x0E:
        raise ValueError(f"Not a Command Complete event: {raw[:2].hex(' ')}")
    opcode = struct.unpack_from('<H', raw, 4)[0]
    return opcode, raw[6], raw[7:]


# ── Stage-2 packet helpers ────────────────────────────────────────────────────

def s2_build(cmd: int, page_addr: int, page_count: int,
             payload: bytes = b'') -> bytes:
    """
    Build a stage-2 protocol packet.

    Format: 87 [cmd] [10 00] [page_addr 4B LE] [page_count 2B LE] [00]
            [payload] [CRC16-ARC 2B LE]
    CRC covers all bytes from 0x87 up to (not including) the CRC itself.
    """
    body = (bytes([S2_SYNC, cmd])
            + S2_PROTO_VER
            + struct.pack('<I', page_addr)
            + struct.pack('<H', page_count)
            + b'\x00'
            + payload)
    return body + struct.pack('<H', crc16arc(body))


def s2_build_init() -> bytes:
    """
    Stage-2 flash-init packet (shorter format, no addr/count fields):
      87 10 10 00 [4 bytes params] [CRC 2B]
    """
    body = bytes([S2_SYNC, S2_CMD_INIT]) + S2_PROTO_VER + S2_INIT_PARAMS
    return body + struct.pack('<H', crc16arc(body))


def byte_addr_to_pages(byte_addr: int) -> int:
    """Convert flash byte address to protocol page address (addr / 256)."""
    if byte_addr % PAGE_SIZE:
        raise ValueError(f"Flash address 0x{byte_addr:X} is not page-aligned (256 B)")
    return byte_addr // PAGE_SIZE


# ── Launch-params extraction ──────────────────────────────────────────────────

def extract_launch_params(fw: bytes) -> bytes:
    """
    Derive 0xFC62 launch parameters from the FW binary header.
    See memory/project_ble_peripheral.md for field layout.
    exec_addr = thumb_ptr & ~0xF (from FW offset 15), suffix = FW[19:24].
    """
    if len(fw) < 24:
        raise ValueError("FW binary too short to extract launch params")
    thumb_ptr = struct.unpack_from('<I', fw, 15)[0]
    return struct.pack('<I', thumb_ptr & ~0xF) + fw[19:24]


# ── Loader class ─────────────────────────────────────────────────────────────

class RTL8762Loader:
    def __init__(self, port: str, timeout: float = 1.0):
        self.port    = port
        self.timeout = timeout
        self.ser: serial.Serial = None  # type: ignore[assignment]

    def __enter__(self):
        self.ser = serial.Serial(
            self.port,
            baudrate  = BAUDRATE,
            bytesize  = serial.EIGHTBITS,
            parity    = serial.PARITY_NONE,
            stopbits  = serial.STOPBITS_ONE,
            timeout   = self.timeout,
        )
        return self

    def __exit__(self, *_):
        if self.ser and self.ser.is_open:
            self.ser.close()

    # ── Pin control ──────────────────────────────────────────────────────────

    def _set_pin(self, pin: str, logic_high: bool, invert: bool = False):
        value = logic_high ^ invert
        if pin == 'dtr':
            self.ser.dtr = value
        elif pin == 'rts':
            self.ser.rts = value

    def enter_bootloader(self, rst_pin='dtr', rst_invert=False,
                         bl_pin='rts',  bl_invert=False):
        """Pulse RESET while BL_EN is asserted to enter ROM bootloader."""
        print("  Entering bootloader mode (Auto-Reset sequence)...")

        self._set_pin('dtr', False)
        self._set_pin('rts', False)
        time.sleep(0.1)

        self._set_pin(bl_pin, True,  invert=bl_invert)
        self._set_pin(rst_pin, False, invert=rst_invert)
        time.sleep(0.1)

        self._set_pin(rst_pin, True,  invert=rst_invert)
        self._set_pin(bl_pin, False, invert=bl_invert)
        time.sleep(0.1)

        self._set_pin(rst_pin, False, invert=rst_invert)
        self._set_pin(bl_pin, True,  invert=bl_invert)
        time.sleep(0.3)

        self._set_pin(bl_pin, False, invert=bl_invert)
        
        self.ser.reset_input_buffer()
        print("  done")

    # ── Stage-1: HCI I/O ─────────────────────────────────────────────────────

    def _recv_hci_event(self) -> bytes:
        hdr = self.ser.read(1)
        if not hdr:
            raise TimeoutError("Timeout waiting for HCI event")
        if hdr[0] != 0x04:
            raise ValueError(f"Expected HCI event 0x04, got 0x{hdr[0]:02X}")
        ev_hdr = self.ser.read(2)
        if len(ev_hdr) < 2:
            raise TimeoutError("Timeout reading HCI event header")
        payload = self.ser.read(ev_hdr[1])
        if len(payload) < ev_hdr[1]:
            raise TimeoutError(f"Truncated HCI payload ({len(payload)}/{ev_hdr[1]})")
        return bytes([0x04]) + ev_hdr + payload

    def _hci_cmd(self, opcode: int, params: bytes = b'') -> bytes:
        self.ser.write(make_hci_cmd(opcode, params))
        raw = self._recv_hci_event()
        ev_op, status, ret = parse_cmd_complete(raw)
        if ev_op != opcode:
            raise ValueError(f"Opcode mismatch: sent 0x{opcode:04X}, got 0x{ev_op:04X}")
        if status != 0x00:
            raise RuntimeError(f"Command 0x{opcode:04X} failed, status=0x{status:02X}")
        return ret

    # ── Stage-1: protocol steps ───────────────────────────────────────────────

    def _get_version(self, enter_bl: bool,
                     rst_pin: str, rst_invert: bool,
                     bl_pin:  str, bl_invert:  bool, 
                     retries: int = 20, retry_delay: float = 0.25) -> dict:
        """Sync with ROM bootloader; retry until it responds."""
        last_exc: Exception = TimeoutError("no attempts")
        for attempt in range(retries):
            try:
                if enter_bl:
                    self.enter_bootloader(rst_pin, rst_invert, bl_pin, bl_invert)
                self.ser.reset_input_buffer()
                ret  = self._hci_cmd(OP_GET_VERSION)
                info = {}
                if len(ret) >= 8:
                    info['chip_id'] = struct.unpack_from('<H', ret, 6)[0]
                return info
            except Exception as exc:
                last_exc = exc
                if attempt < retries - 1:
                    print(f"\r  Syncing... attempt {attempt + 1}/{retries}")
                    time.sleep(retry_delay)
        raise TimeoutError(f"ROM bootloader did not respond after {retries} attempts: {last_exc}")

    def _load_stage1(self, fw_path: str,
                     enter_bl: bool,
                     rst_pin: str, rst_invert: bool,
                     bl_pin:  str, bl_invert:  bool) -> bytes:
        """Download FW_.bin to RAM and launch, return raw 2nd-stage ready packet."""
        fw = Path(fw_path).read_bytes()
        num_chunks = (len(fw) + CHUNK_PAYLOAD - 1) // CHUNK_PAYLOAD

        print("  Syncing with ROM bootloader...")
        info    = self._get_version(enter_bl, rst_pin, rst_invert, bl_pin, bl_invert)
        chip_id = info.get('chip_id', 0)
        print(f"\r  Sync OK — chip_id=0x{chip_id:04X}                    ")

        self._hci_cmd(OP_SET_ADDR, struct.pack('<I', FW_DOWNLOAD_ADDR))
        time.sleep(0.1)
        self.ser.reset_input_buffer()

        print(f"  Downloading {len(fw)} bytes ({num_chunks} chunks)...")
        offset = idx = 0
        while offset < len(fw):
            chunk = fw[offset:offset + CHUNK_PAYLOAD]
            ret   = self._hci_cmd(OP_DOWNLOAD, bytes([idx]) + chunk)
            if ret and ret[0] != idx:
                raise RuntimeError(f"Chunk ACK mismatch: sent {idx}, got {ret[0]}")
            offset += len(chunk)
            pct = offset * 100 // len(fw)
            print(f"\r    [{pct:3d}%] chunk {idx:3d}/{num_chunks - 1}", end="", flush=True)
            idx += 1
        print(f"\r    [100%] {idx} chunks sent          ")

        print("  Launching 2nd-stage loader...", end=" ", flush=True)
        self._hci_cmd(OP_LAUNCH, extract_launch_params(fw))
        print("done")

        # Receive 2nd-stage ready packet (non-HCI, starts with 0x87)
        prev = self.ser.timeout
        self.ser.timeout = 3.0
        try:
            sync = self.ser.read(1)
            if sync and sync[0] == S2_SYNC:
                rest = self.ser.read(78)
                raw  = sync + rest
                # Parse flash name (ASCII, 9 bytes at offset 36)
                if len(raw) >= 45:
                    flash = raw[36:45].rstrip(b'\x00').decode('ascii', errors='replace')
                    print(f"  2nd-stage ready — flash: {flash}")
                else:
                    print("  2nd-stage ready (short packet)")
                return raw
            else:
                print("  2nd-stage ready timeout")
                return b''
        finally:
            self.ser.timeout = prev

    # ── Stage-2: low-level I/O ────────────────────────────────────────────────

    def _s2_recv_ack(self, expected_cmd: int, timeout: float = 3.0) -> None:
        """
        Receive a 10-byte stage-2 ACK and validate it.
        Format: 87 [cmd] [10 00] [00 00 00 00] [CRC 2B LE]

        Stale bytes (e.g. leftover from stage-1 launch or baud-rate change)
        are silently discarded until the 0x87 sync byte is found.
        """
        prev = self.ser.timeout
        self.ser.timeout = timeout
        try:
            # Scan for 0x87 sync, dropping any leading garbage bytes
            buf = bytearray()
            while True:
                b = self.ser.read(1)
                if not b:
                    raise TimeoutError(
                        f"Stage-2 ACK timeout for cmd 0x{expected_cmd:02X} "
                        f"(no sync byte found)"
                    )
                if b[0] == S2_SYNC:
                    buf += b
                    break
                # stale byte — discard silently
            # Read the remaining 9 bytes of the ACK
            rest = self.ser.read(9)
            buf += rest
        finally:
            self.ser.timeout = prev

        if len(buf) < 10:
            raise TimeoutError(
                f"Stage-2 ACK timeout for cmd 0x{expected_cmd:02X} "
                f"(got {len(buf)}/10 bytes after sync)"
            )
        if buf[1] != expected_cmd:
            raise ValueError(
                f"Stage-2 ACK cmd mismatch: expected 0x{expected_cmd:02X}, "
                f"got 0x{buf[1]:02X}"
            )
        expected_crc = crc16arc(buf[:8])
        actual_crc   = struct.unpack_from('<H', buf, 8)[0]
        if expected_crc != actual_crc:
            raise ValueError(
                f"Stage-2 CRC error (cmd 0x{expected_cmd:02X}): "
                f"expected 0x{expected_crc:04X}, got 0x{actual_crc:04X}"
            )
        if any(buf[4:8]):
            raise RuntimeError(
                f"Stage-2 cmd 0x{expected_cmd:02X} returned error: {buf[4:8].hex(' ')}"
            )

    def _s2_send(self, pkt: bytes, expected_cmd: int, timeout: float = 3.0) -> None:
        self.ser.write(pkt)
        self._s2_recv_ack(expected_cmd, timeout)

    def _s2_recv_data_response(self, expected_cmd: int,
                               timeout: float = READ_TIMEOUT) -> Tuple[int, bytes]:
        """
        Receive a variable-length stage-2 response (used for read commands).

        Wire format (UART_SendPacket @ 0x206bb0):
          [0x87][cmd_lo][cmd_hi][status][count_b0..b3 LE][data (count bytes)][CRC 2B LE]

        Returns (status, data).  Raises on sync/CRC errors.
        """
        prev = self.ser.timeout
        self.ser.timeout = timeout
        try:
            hdr = self.ser.read(8)
        finally:
            self.ser.timeout = prev

        if len(hdr) < 8:
            raise TimeoutError(
                f"Stage-2 response timeout for cmd 0x{expected_cmd:02X} "
                f"(got {len(hdr)}/8 header bytes)"
            )
        if hdr[0] != S2_SYNC:
            raise ValueError(f"Stage-2 response: bad sync 0x{hdr[0]:02X}")
        if hdr[1] != expected_cmd:
            raise ValueError(
                f"Stage-2 response cmd mismatch: "
                f"expected 0x{expected_cmd:02X}, got 0x{hdr[1]:02X}"
            )
        status     = hdr[3]
        data_len   = struct.unpack_from('<I', hdr, 4)[0]

        # Read data payload
        prev = self.ser.timeout
        self.ser.timeout = READ_TIMEOUT + data_len / (BAUDRATE / 10)
        try:
            data = self.ser.read(data_len) if data_len else b''
        finally:
            self.ser.timeout = prev

        if len(data) < data_len:
            raise TimeoutError(
                f"Stage-2 data truncated: expected {data_len}, got {len(data)}"
            )

        # Read and verify CRC (over header + data)
        prev = self.ser.timeout
        self.ser.timeout = 2.0
        try:
            crc_bytes = self.ser.read(2)
        finally:
            self.ser.timeout = prev

        if len(crc_bytes) < 2:
            raise TimeoutError("Stage-2 response: CRC bytes missing")

        expected_crc = crc16arc(hdr + data)
        actual_crc   = struct.unpack_from('<H', crc_bytes)[0]
        if expected_crc != actual_crc:
            raise ValueError(
                f"Stage-2 response CRC error (cmd 0x{expected_cmd:02X}): "
                f"expected 0x{expected_crc:04X}, got 0x{actual_crc:04X}"
            )
        return status, data

    # ── Stage-2: flash operations ─────────────────────────────────────────────

    def _probe_stage2(self) -> bool:
        """
        Check whether the 2nd-stage loader is already running by sending the
        flash-init packet and waiting briefly for a valid ACK.
        Returns True if the 2nd-stage loader responds, False otherwise.
        """
        print("  Probing for 2nd-stage loader...", end=" ", flush=True)
        self.ser.reset_input_buffer()
        self.ser.write(s2_build_init())
        try:
            self._s2_recv_ack(S2_CMD_INIT, timeout=0.5)
            print("already running")
            return True
        except Exception:
            print("not detected")
            return False

    def flash_connect(self) -> None:
        """Send stage-2 flash init command (0x10)."""
        print("  Flash connect...", end=" ", flush=True)
        pkt = s2_build_init()
        self.ser.write(pkt)
        self._s2_recv_ack(S2_CMD_INIT, timeout=3.0)
        print("OK")

    def flash_erase_sector(self, byte_addr: int) -> None:
        """Erase one 4 KB sector at byte_addr (must be 4 KB aligned)."""
        if byte_addr % SECTOR_SIZE:
            raise ValueError(f"Erase address 0x{byte_addr:X} not 4 KB-aligned")
        page_addr = byte_addr // PAGE_SIZE
        pkt = s2_build(S2_CMD_ERASE, page_addr, PAGES_PER_SECTOR)
        self._s2_send(pkt, S2_CMD_ERASE, ERASE_TIMEOUT)

    def flash_write_sector(self, byte_addr: int, data: bytes) -> None:
        """
        Write exactly SECTOR_SIZE bytes to flash at byte_addr.
        Caller must ensure the sector is erased first.
        """
        if len(data) != SECTOR_SIZE:
            raise ValueError(f"write_sector expects {SECTOR_SIZE} bytes, got {len(data)}")
        if byte_addr % SECTOR_SIZE:
            raise ValueError(f"Write address 0x{byte_addr:X} not 4 KB-aligned")
        page_addr = byte_addr // PAGE_SIZE
        pkt = s2_build(S2_CMD_WRITE, page_addr, PAGES_PER_SECTOR, data)
        self._s2_send(pkt, S2_CMD_WRITE, WRITE_TIMEOUT)

    def flash_verify_sector(self, byte_addr: int, data: bytes) -> None:
        """Verify flash sector against CRC16-ARC of data."""
        if byte_addr % SECTOR_SIZE:
            raise ValueError(f"Verify address 0x{byte_addr:X} not 4 KB-aligned")
        page_addr = byte_addr // PAGE_SIZE
        checksum  = crc16arc(data)
        pkt = s2_build(S2_CMD_VERIFY, page_addr, PAGES_PER_SECTOR,
                       struct.pack('<H', checksum))
        self._s2_send(pkt, S2_CMD_VERIFY, VERIFY_TIMEOUT)

    def flash_erase(self, byte_addr: int, length: int) -> None:
        """Erase flash region [byte_addr, byte_addr+length). Aligns up to sector boundaries."""
        if byte_addr % SECTOR_SIZE:
            raise ValueError(f"Erase address 0x{byte_addr:X} not 4 KB-aligned")
        num_sectors = (length + SECTOR_SIZE - 1) // SECTOR_SIZE
        print(f"  Erasing {num_sectors} sectors at 0x{byte_addr:08X}...")
        for i in range(num_sectors):
            addr = byte_addr + i * SECTOR_SIZE
            self.flash_erase_sector(addr)
            print(f"\r    [{(i+1)*100//num_sectors:3d}%] erased sector {i+1}/{num_sectors} @ 0x{addr:08X}",
                  end="", flush=True)
        print()

    def flash_write(self, byte_addr: int, data: bytes) -> None:
        """
        Write data to flash at byte_addr.
        Data is padded with 0xFF to the next 4 KB boundary.
        Erases each sector before writing, then verifies.
        """
        if byte_addr % SECTOR_SIZE:
            raise ValueError(f"Write address 0x{byte_addr:X} not 4 KB-aligned")

        # Pad to full sectors
        pad = (-len(data)) % SECTOR_SIZE
        if pad:
            data = data + b'\xFF' * pad

        num_sectors = len(data) // SECTOR_SIZE
        print(f"  Writing {len(data)} bytes ({num_sectors} sectors) to 0x{byte_addr:08X}...")

        for i in range(num_sectors):
            addr   = byte_addr + i * SECTOR_SIZE
            sector = data[i * SECTOR_SIZE:(i + 1) * SECTOR_SIZE]
            pct    = (i + 1) * 100 // num_sectors

            self.flash_erase_sector(addr)
            self.flash_write_sector(addr, sector)
            self.flash_verify_sector(addr, sector)

            print(f"\r    [{pct:3d}%] sector {i+1}/{num_sectors} @ 0x{addr:08X}  erase+write+verify",
                  end="", flush=True)
        print(f"\r    [100%] {num_sectors} sectors written and verified                    ")

    def flash_read(self, byte_addr: int, length: int,
                   decrypt: bool = True) -> bytes:
        """
        Read flash via CMD_Read_Flash (0x1033).

        The SoC AES-encrypts the response (fixed key, IV=0 per chunk).
        Pass decrypt=False to get the raw encrypted bytes.

        Reads are done in READ_CHUNK_PAGES × PAGE_SIZE = 4 KB chunks.
        Count must be a multiple of 16 (AES block size) — padded up automatically.
        """
        if byte_addr % PAGE_SIZE:
            raise ValueError(f"Read address 0x{byte_addr:X} not page-aligned")

        # Round length up to AES block size
        padded = (length + 15) & ~15

        result = bytearray()
        offset = 0
        chunk_bytes = READ_CHUNK_PAGES * PAGE_SIZE  # 4096

        num_chunks = (padded + chunk_bytes - 1) // chunk_bytes
        print(f"  Reading {length} bytes ({num_chunks} chunks) from 0x{byte_addr:08X}...")

        while offset < padded:
            this_len  = min(chunk_bytes, padded - offset)
            page_addr = (byte_addr + offset) // PAGE_SIZE
            page_cnt  = this_len // PAGE_SIZE

            pkt = s2_build(S2_CMD_READ, page_addr, page_cnt)
            self.ser.write(pkt)
            status, chunk = self._s2_recv_data_response(S2_CMD_READ)

            if status != 0:
                raise RuntimeError(
                    f"Flash read at 0x{byte_addr+offset:08X} failed, status=0x{status:02X}"
                )

            if decrypt:
                chunk = decrypt_flash_data(chunk)

            result.extend(chunk)
            offset += this_len
            pct = min(offset, padded) * 100 // padded
            print(f"\r    [{pct:3d}%] {offset // PAGE_SIZE} pages read", end="", flush=True)

        print(f"\r    [100%] {padded // PAGE_SIZE} pages read          ")
        return bytes(result[:length])   # trim padding

    def flash_reboot(self, to_bootloader: bool = False) -> None:
        """
        Reboot the SoC via CMD_Reboot (0x1041).
        to_bootloader=False → normal reboot (enters application)
        to_bootloader=True  → reboot into ROM bootloader mode
        """
        mode = 1 if to_bootloader else 0
        # addr_b0 carries the mode flag (see CMD_Reboot @ 0x206038)
        body = (bytes([S2_SYNC, S2_CMD_REBOOT, S2_CMD_HI, mode]))
        pkt = body + struct.pack('<H', crc16arc(body))
        print(f"  Rebooting SoC ({'to bootloader' if to_bootloader else 'normal'})...",
              end=" ", flush=True)
        self.ser.write(pkt)
        # SoC sends ACK then immediately reboots — short timeout is OK
        try:
            self._s2_recv_ack(S2_CMD_REBOOT, timeout=2.0)
            print("ACK received")
        except TimeoutError:
            print("no ACK (SoC may have rebooted before response)")
        time.sleep(0.2)   # let the SoC complete reset

    # ── UART passthrough ─────────────────────────────────────────────────────

    def passthrough(self) -> None:
        """
        Read everything the SoC sends and print it to stdout until Ctrl+C.
        Useful for watching UART log output after a reboot.
        Resets the serial timeout to non-blocking (0.05 s) so we can exit cleanly.
        """
        self.ser.timeout = 0.05
        self.ser.reset_input_buffer()
        print("--- UART passthrough active (Ctrl+C to exit) ---")
        line_buf = bytearray()
        try:
            while True:
                chunk = self.ser.read(256)
                if not chunk:
                    continue
                for b in chunk:
                    if b == 0x0A:                          # LF → flush line
                        try:
                            print(line_buf.decode('utf-8', errors='replace'),
                                  end='\n', flush=True)
                        except Exception:
                            print(line_buf.hex(' '), flush=True)
                        line_buf = bytearray()
                    elif b != 0x0D:                        # skip bare CR
                        line_buf.append(b)
        except KeyboardInterrupt:
            if line_buf:                                   # flush incomplete line
                print(line_buf.decode('utf-8', errors='replace'), flush=True)
            print("\n--- passthrough ended ---")

    # ── Top-level: run any action ─────────────────────────────────────────────

    def run(self, args: argparse.Namespace) -> None:
        enter_bl   = not args.no_reset
        rst_pin    = args.rst_pin
        bl_pin     = args.bl_pin
        rst_invert = args.rst_invert
        bl_invert  = args.bl_invert
        fw_path    = args.fw

        # For flash operations: first check if Stage-2 is already running so
        # we can skip the Stage-1 load entirely.
        stage2_live = False
        if args.action != 'load':
            stage2_live = self._probe_stage2()

        if not stage2_live:
            self._load_stage1(fw_path, enter_bl, rst_pin, rst_invert, bl_pin, bl_invert)

        if args.action == 'load':
            if args.passthrough:
                self.passthrough()
            return

        # Stage-2 actions: flash_connect was already ACK'd by the probe if
        # stage2_live, so only send it again when we came through Stage-1.
        if not stage2_live:
            self.flash_connect()

        if args.action == 'flash-write':
            data = Path(args.input).read_bytes()
            self.flash_write(args.addr, data)
            print(f"  Done — wrote {len(data)} bytes @ 0x{args.addr:08X}")
            if args.reboot:
                self.flash_reboot()
            if args.passthrough:
                self.passthrough()

        elif args.action == 'flash-erase':
            self.flash_erase(args.addr, args.length)
            print(f"  Done — erased 0x{args.length:X} bytes @ 0x{args.addr:08X}")
            if args.reboot:
                self.flash_reboot()
            if args.passthrough:
                self.passthrough()

        elif args.action == 'flash-read':
            decrypt = not args.no_decrypt
            data = self.flash_read(args.addr, args.length, decrypt=decrypt)
            out  = args.output or f"flash_0x{args.addr:08X}_{args.length}.bin"
            Path(out).write_bytes(data)
            print(f"  Saved {len(data)} bytes -> {out}"
                  + (" (decrypted)" if decrypt else " (raw encrypted)"))


# ── CLI ───────────────────────────────────────────────────────────────────────

def build_parser() -> argparse.ArgumentParser:
    _common = argparse.ArgumentParser(add_help=False)
    _common.add_argument('-p', '--port', required=True,
                         help='Serial port, e.g. COM3 or /dev/ttyUSB0')
    _common.add_argument('-f', '--fw', default='RTL8762E_FW_B.bin',
                         help='2nd-stage FW binary (default: RTL8762E_FW_B.bin)')
    _common.add_argument('--no-reset', action='store_true',
                         help='Skip pin-reset sequence (SoC already in BL mode)')
    _common.add_argument('--rst-pin', choices=['dtr', 'rts', 'none'], default='rts',
                         help='Serial pin driving RESET (active-low, default: rts)')
    _common.add_argument('--bl-pin',  choices=['dtr', 'rts', 'none'], default='dtr',
                         help='Serial pin driving BL_EN  (active-high, default: dtr)')
    _common.add_argument('--rst-invert', action='store_true',
                         help='Invert RST pin polarity')
    _common.add_argument('--bl-invert',  action='store_true',
                         help='Invert BL_EN pin polarity')

    p = argparse.ArgumentParser(
        description="RTL8762ESL UART Bootloader Tool",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__,
    )
    sub = p.add_subparsers(dest='action', required=True)

    load_p = sub.add_parser('load',
                            help='Load FW_ to RAM and launch 2nd-stage loader',
                            parents=[_common])
    load_p.add_argument('-pt', '--passthrough', action='store_true',
                        help='Enter UART passthrough after launch (show device logs)')

    fw_cmd = sub.add_parser('flash-write',
                             help='Erase + write + verify a binary to flash',
                             parents=[_common])
    fw_cmd.add_argument('--addr', required=True, type=lambda x: int(x, 0),
                        help='Flash byte address (must be 4 KB-aligned, e.g. 0x802000)')
    fw_cmd.add_argument('-i', '--input', required=True,
                        help='Binary file to write (no 0x200 header)')
    fw_cmd.add_argument('--reboot', action='store_true',
                        help='Reboot SoC after successful write')
    fw_cmd.add_argument('-pt', '--passthrough', action='store_true',
                        help='Enter UART passthrough after write/reboot (show device logs)')

    fe = sub.add_parser('flash-erase',
                        help='Erase flash region',
                        parents=[_common])
    fe.add_argument('--addr',   required=True, type=lambda x: int(x, 0),
                    help='Flash byte address (4 KB-aligned)')
    fe.add_argument('--length', required=True, type=lambda x: int(x, 0),
                    help='Number of bytes to erase')
    fe.add_argument('--reboot', action='store_true',
                    help='Reboot SoC after erase')
    fe.add_argument('-pt', '--passthrough', action='store_true',
                    help='Enter UART passthrough after erase/reboot')

    fr = sub.add_parser('flash-read',
                        help='Read flash region to file (AES-decrypted by default)',
                        parents=[_common])
    fr.add_argument('--addr',   required=True, type=lambda x: int(x, 0),
                    help='Flash byte address')
    fr.add_argument('--length', required=True, type=lambda x: int(x, 0),
                    help='Number of bytes to read')
    fr.add_argument('-o', '--output',
                    help='Output file (default: flash_<addr>_<len>.bin)')
    fr.add_argument('--no-decrypt', action='store_true',
                    help='Save raw encrypted bytes without AES decryption')

    return p


def main():
    args = build_parser().parse_args()
    print(f"RTL8762ESL Loader — port={args.port}  action={args.action}")
    try:
        with RTL8762Loader(args.port) as loader:
            loader.run(args)
    except (ValueError, RuntimeError) as e:
        sys.exit(f"Error: {e}")
    except TimeoutError as e:
        sys.exit(f"Timeout: {e}")
    except KeyboardInterrupt:
        sys.exit("\nAborted.")


if __name__ == '__main__':
    main()
