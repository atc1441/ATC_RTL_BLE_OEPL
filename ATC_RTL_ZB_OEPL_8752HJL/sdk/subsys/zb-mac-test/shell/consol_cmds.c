/**************************************************************************//**
 * @file     consol_cmds.c
 * @brief    Some commands implementation for the shell command. It provides
 *           some basic memory write and dump commands.
 * @version  V1.00
 * @date     2016-05-30
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2016 Realtek Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 ******************************************************************************/
#include "shell.h"
#include "dbg_printf.h"
#include "strproc.h"

#define SECTION_CCMD_TEXT
#define SECTION_CCMD_DATA
#define SECTION_CCMD_RODATA
#define SECTION_CCMD_BSS

extern void dump_for_one_bytes(u8 *pdata, u32 len);
extern void hal_misc_rst_by_wdt_rtl8710c(void);
#define _hal_misc_rst_by_wdt    hal_misc_rst_by_wdt_rtl8710c

s32 cmd_dump_byte(u32 argc, u8 *argv[]);
s32 cmd_dump_helfword(u32 argc, u8 *argv[]);
s32 cmd_dump_word(u32 argc, u8 *argv[]);
s32 cmd_write_byte(u32 argc, u8 *argv[]);
s32 cmd_write_word(u32 argc, u8 *argv[]);
s32 cmd_ubec_console(u32 argc, u8 *argv[]);

SECTION_CCMD_RODATA
const shell_command_entry_t rom_cmd_table[] =
{
    {
        (const char *)"DB", (shell_program_t)cmd_dump_byte, (const char *)"\tDB <Address, Hex> <Len, Dec>: \r\n"
        "\tDump memory byte or Read Hw byte register\r\n"
    },
    {
        (const char *)"DHW", (shell_program_t)cmd_dump_helfword, (const char *)"\tDHW <Address, Hex> <Len, Dec>: \r\n"
        "\tDump memory helf-word or Read Hw helf-word register;\r\n"
    },
    {
        (const char *)"DW", (shell_program_t)cmd_dump_word, (const char *)"\tDW <Address, Hex> <Len, Dec>:\r\n"
        "\tDump memory word or Read Hw word register; \r\n"
    },
    {
        (const char *)"EB", (shell_program_t)cmd_write_byte, (const char *)"\tEB <Address, Hex> <Value, Hex>: \r\n"
        "\tWrite memory byte or Write Hw byte register \r\n"
        "\tSupports multiple byte writting by a single command \r\n"
        "\tEx: EB Address Value0 Value1\r\n"
    },
    {
        (const char *)"EW", (shell_program_t)cmd_write_word, (const char *)"\tEW <Address, Hex> <Value, Hex>: \r\n"
        "\tWrite memory word or Write Hw word register \r\n"
        "\tSupports multiple word writting by a single command \r\n"
        "\tEx: EW Address Value0 Value1\r\n"
    },
    {(const char *)NULL, (shell_program_t)NULL, (const char *)NULL}     // end of table
};

SECTION_CCMD_RODATA
const u32 mem_access_abandon_range[] =
{
    /*  start addr      end addr    */
//    0x40000810,     0x40000814,
//    0x50000810,     0x50000814,

    /* end of list */
    0xFFFFFFFF,     0xFFFFFFFF
};

SECTION_CCMD_TEXT
void dump_for_one_bytes(u8 *pdata, u32 len)
{
    u8 *pbuf = pdata;
    u32 length = len;
    u32 line_idx = 0;
    u32 byte_idx;
    u32 offset;

    dbg_printf("\r\n [Addr]   .0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .A .B .C .D .E .F\r\n");

    while (line_idx < length)
    {
        dbg_printf("%08X: ", (pbuf + line_idx));

        if ((line_idx + 16) < length)
        {
            offset = 16;
        }
        else
        {
            offset = length - line_idx;
        }

        for (byte_idx = 0; byte_idx < offset; byte_idx++)
        {
            dbg_printf("%02x ", pbuf[line_idx + byte_idx]);
        }

        for (byte_idx = 0; byte_idx < (16 - offset); byte_idx++)   //a last line
        {
            dbg_printf("   ");
        }

        dbg_printf("    ");        //between byte and char

        for (byte_idx = 0;  byte_idx < offset; byte_idx++)
        {
            if (' ' <= pbuf[line_idx + byte_idx]  && pbuf[line_idx + byte_idx] <= '~')
            {
                dbg_printf("%c", pbuf[line_idx + byte_idx]);
            }
            else
            {
                dbg_printf(".");
            }
        }

        dbg_printf("\n\r");
        line_idx += 16;
    }

}

SECTION_CCMD_TEXT
s32 _cmd_mem_range_validate(u32 addr, u32 len)
{
    u32 i;
    u32 end_addr;
    s32 valid = 1;

    i = 0;
    if (len <= 4)
    {
        while (mem_access_abandon_range[i] != 0xFFFFFFFF)
        {
            if ((addr >= mem_access_abandon_range[i]) && (addr <= mem_access_abandon_range[i + 1]))
            {
                valid = 0;
                break;
            }
            i = i + 2;
        }
    }
    else
    {
        end_addr = addr + len;
        while (mem_access_abandon_range[i] != 0xFFFFFFFF)
        {
            if (((addr >= mem_access_abandon_range[i]) && (addr <= mem_access_abandon_range[i + 1])) ||
                ((end_addr >= mem_access_abandon_range[i]) && (end_addr <= mem_access_abandon_range[i + 1])))
            {
                valid = 0;
                break;
            }
            i = i + 2;
        }
    }

    return valid;
}

SECTION_CCMD_TEXT
s32 cmd_dump_byte(u32 argc, u8 *argv[])
{
    u32 src;
    u32 len;

    if (argc < 1)
    {
        dbg_printf("Wrong argument number!\r\n");
        return FALSE;
    }

    src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);

    if (argc > 1)
    {
        if (!argv[1])
        {
            len = 16;
        }
        else
        {
            len = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
        }
    }
    else
    {
        len = 16;
    }

    dump_for_one_bytes((u8 *)src, len);
    dbg_printf("Done\r\n");
    return _TRUE ;
}

SECTION_CCMD_TEXT
s32 cmd_dump_helfword(u32 argc, u8  *argv[])
{
    u32 src;
    u32 len;
    u32 i;

    if (argc < 1)
    {
        dbg_printf("Wrong argument number!\r\n");
        return _FALSE;
    }

    if (argv[0])
    {
        src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
    }
    else
    {
        dbg_printf("Wrong argument number!\r\n");
        return _FALSE;
    }

    if (argc > 1)
    {
        if (!argv[1])
        {
            len = 1;
        }
        else
        {
            len = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
        }
    }
    else
    {
        len = 1;
    }

    while ((src) & 0x01)
    {
        src++;
    }

    for (i = 0; i < len; i += 4, src += 16)
    {
        dbg_printf("%08X:  %04X    %04X    %04X    %04X    %04X    %04X    %04X    %04X\r\n",
                   src, *(u16 *)(src), *(u16 *)(src + 2),
                   *(u16 *)(src + 4), *(u16 *)(src + 6),
                   *(u16 *)(src + 8), *(u16 *)(src + 10),
                   *(u16 *)(src + 12), *(u16 *)(src + 14));
    }
    dbg_printf("Done\r\n");
    return _TRUE;

}

SECTION_CCMD_TEXT
s32 cmd_dump_word(u32 argc, u8  *argv[])
{
    u32 src;
    u32 len;
    u32 i;

    if (argc < 1)
    {
        dbg_printf("Wrong argument number!\r\n");
        return _FALSE;
    }

    if (argv[0])
    {
        src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
    }
    else
    {
        dbg_printf("Wrong argument number!\r\n");
        return _FALSE;
    }

    if (argc > 1)
    {
        if (!argv[1])
        {
            len = 1;
        }
        else
        {
            len = _strtoul((const char *)(argv[1]), (char **)NULL, 10);
        }
    }
    else
    {
        len = 1;
    }

    while ((src) & 0x03)
    {
        src++;
    }

    dbg_printf("\r\n");
    for (i = 0; i < len; i += 4, src += 16)
    {
        dbg_printf("%08X:    %08X", src, *(u32 *)(src));
        dbg_printf("    %08X", *(u32 *)(src + 4));
        dbg_printf("    %08X", *(u32 *)(src + 8));
        dbg_printf("    %08X\r\n", *(u32 *)(src + 12));
    }
    dbg_printf("Done\r\n");
    return _TRUE;
}

SECTION_CCMD_TEXT
s32 cmd_write_byte(u32 argc, u8  *argv[])
{
    u32 src, i;
    u8 value;

    src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);

    for (i = 0; i < argc - 1; i++, src++)
    {
        if (_cmd_mem_range_validate(src, 1))
        {
            value = _strtoul((const char *)(argv[i + 1]), (char **)NULL, 16);
            dbg_printf("0x%08X = 0x%02X\r\n", src, value);
            *(volatile u8 *)(src) = value;
        }
        else
        {
            dbg_printf("Addr(0x%x). in abandon range!\r\n", src);
        }
    }
    dbg_printf("Done\r\n");
    return 0;
}

SECTION_CCMD_TEXT
s32 cmd_write_word(u32 argc, u8  *argv[])
{
    u32 src;
    u32 value, i;

    src = _strtoul((const char *)(argv[0]), (char **)NULL, 16);
    while ((src) & 0x03)
    {
        src++;
    }

    for (i = 0; i < (argc - 1); i++, src += 4)
    {
        if (_cmd_mem_range_validate(src, 4))
        {
            value = _strtoul((const char *)(argv[i + 1]), (char **)NULL, 16);
            dbg_printf("0x%08X = 0x%08X\r\n", src, value);
            *(volatile u32 *)(src) = value;
        }
        else
        {
            dbg_printf("Addr(0x%x). in abandon range!\r\n", src);
        }
    }
    dbg_printf("Done\r\n");
    return 0;
}
