# EL097R2CRN 9.7" BWR / RTL8762ESL

This project adds a separate build for the **SoluM EL097R2CRN 9.7" BWR**
electronic shelf label. It intentionally lives next to
`ATC_RTL_BLE_OEPL_8762ESL` so the existing ELO58R2CRN 5.85" build remains
unchanged.

## Status

Hardware tested on an EL097R2CRN tag with RTL8762ESL.

Verified:

- BLE advertising and connections
- OpenEPaperLink AP image updates
- direct Home Assistant BLE image updates
- OEPL hardware type `0x2E` (`SOLUM_M3_BWR_97`)
- 960 x 672 BWR image rendering
- ZLIB compressed 2bpp image transfer/rendering
- boot/status screen orientation
- DLPS after display refresh
- 60 second AON watchdog without unexpected resets

Measured with a DMM on the tag supply:

- during EPD refresh: approximately 5-25 mA
- after `DRAW done` / in low-power operation: approximately 0.02-0.08 mA

The DMM values are intended as an order-of-magnitude check, not a high-speed
current profile.

## Hardware

| Item | EL097R2CRN |
|---|---|
| SoC | Realtek RTL8762ESL |
| Display | 9.7" BWR |
| Resolution | 960 x 672 |
| OEPL hardware type | `0x2E` |
| EPD RAM planes | Black/white (`0x24`) + red (`0x26`) |
| BUSY polarity | HIGH = busy |
| External image flash | 512 KiB |

### EPD pinout

| Signal | RTL8762 pin | Notes |
|---|---|---|
| RESET | P0_0 | active LOW |
| D/C | P0_1 | LOW command, HIGH data |
| CS | P0_2 | active LOW |
| CLK | P0_4 | software SPI clock |
| EPD_PWR | P0_5 | HIGH = display power on |
| MOSI / SDIO | P0_6 | software SPI data |
| BS | P3_2 | LOW = SPI mode |
| BUSY | P3_3 | HIGH = busy |

The display command sequence and address window were reconstructed from the
original EL097R2CRN firmware. The application uses one continuous 960 x 672
controller RAM rather than the dual-panel write path of the existing 5.85"
target.

## Display refresh and DLPS

A full refresh takes about 24 seconds on the tested panel.

Entering RTL8762 DLPS while the EL097R2CRN is physically refreshing caused the
EPD BUSY signal to remain HIGH indefinitely. For this target, DLPS therefore
remains globally enabled but is vetoed while `g_epd_refreshing` is true. As soon
as BUSY goes LOW and `epd_refresh_done()` completes, normal DLPS operation is
allowed again.

This keeps the low-power behavior for the normal idle period while avoiding
interruption of the 9.7" EPD refresh.

## Watchdog

The AON watchdog is enabled with a **60 second timeout**.

It is restarted:

- on normal DLPS exit, as in the existing RTL8762ESL architecture
- immediately before a physical EPD refresh

The pre-refresh restart gives the approximately 24 second refresh enough margin
while DLPS is intentionally blocked.

## External image-flash layout

The 9.7" BWR raw image requires:

```text
960 * 672 / 8 * 2 = 161280 bytes
```

The target therefore uses one `0x28000` (163840 byte) image slot.

```text
0x00000 - 0x27FFF   image slot
0x28000 - 0x44FFF   unused/reserved
0x45000 - ...       OTA staging area
0x7F000              optional flash self-test sector (self-test disabled)
```

## Building

This target deliberately reuses the SDK and vendor tools already stored in the
existing `ATC_RTL_BLE_OEPL_8762ESL` project. This avoids duplicating the Realtek
SDK in the repository.

From the repository root:

```bash
cd ATC_RTL_BLE_OEPL_8762ESL_97/gcc
make
```

The output application is:

```text
bin/ATC_RTL_BLE_OEPL_8762_97.bin
```

The tested development build used GNU Arm Embedded Toolchain
10.3-2021.10 / GCC 10.3.1.

If a non-default toolchain location is used:

```bash
make GCC_PATH=/path/to/arm-none-eabi/bin
```

## Flashing

### Important: initial installation

The application must be used with the matching RTL8762E system image from the
existing RTL8762ESL project.

An app-only flash over the original SoluM system image was not sufficient:
BLE advertising stopped with HCI cause `0x109` (maximum number of connections).
Writing the matching project system blob fixed BLE operation.

For the initial installation:

```bash
cd ATC_RTL_BLE_OEPL_8762ESL_97/gcc
make flash-all-pt COM_PORT=COM3
```

The Makefile writes:

```text
0x801000  matching data_0x801000.bin from ATC_RTL_BLE_OEPL_8762ESL
0x832000  ATC_RTL_BLE_OEPL_8762_97.bin
```

For later application-only updates:

```bash
make flash-pt COM_PORT=COM3
```

which writes only the application at `0x832000`.

## Compatibility tested

The resulting tag was tested successfully with:

- OpenEPaperLink AP
- Home Assistant OpenEPaperLink integration using direct BLE
- BWR/ZLIB image payloads at 960 x 672

## Notes

This directory is intentionally a separate hardware target instead of adding a
large set of conditional `#ifdef` branches to the existing ELO58R2CRN project.
The existing 5.85" source and build remain untouched.
