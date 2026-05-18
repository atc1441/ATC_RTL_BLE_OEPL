/**************************************************************************//**
 * @file     cmd_shell.c
 * @brief    Define the shell command table and implement the shell command
 *           task.
 * @version  V1.00
 * @date     2016-07-20
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2016 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "shell.h"
#include "cmd_shell_rom.h"
#include "stdio_port.h"

#define SHELL_CMD_BUF_SIZE       80
#define SHELL_CMD_LIST_SIZE      10
#define SHELL_CMD_HIST_SIZE      256

#define SECTION_SHELL_CMD_TEXT
#define SECTION_SHELL_CMD_RODATA
#define SECTION_SHELL_CMD_STUBS
#define SECTION_ROM_TEMP_BSS

//extern hal_uart_adapter_t stdio_uart;
extern const shell_command_entry_t rom_cmd_table[];

SECTION_ROM_TEMP_BSS shell_command_t shell_cmd_hdl_rom;
SECTION_ROM_TEMP_BSS char shell_cmd_buffer_rom[SHELL_CMD_BUF_SIZE];
SECTION_ROM_TEMP_BSS char shell_cmd_hist_rom[SHELL_CMD_HIST_SIZE];

SECTION_SHELL_CMD_RODATA const char prompt_str_rom[] = "$>";

BOOL _shell_init_with_stdio_port(shell_command_t *pshell_cmd);

SECTION_SHELL_CMD_STUBS const cmd_shell_func_stubs_t cmd_shell_stubs =
{
    .shell_cmd_hdl = &shell_cmd_hdl_rom,
    .shell_init = _shell_init,
    .shell_init_with_stdio_port = _shell_init_with_stdio_port,
    .shell_set_cmd_buf = _shell_set_cmd_buf,
    .shell_set_hist_buf = _shell_set_hist_buf,
    .shell_set_prompt = _shell_set_prompt,
    .shell_set_cmd_table = _shell_set_cmd_table,
    .shell_set_cmd_list = _shell_set_cmd_list,
    .shell_register = _shell_register,
    .shell_unregister_all = _shell_unregister_all,
    .shell_task = _shell_task,
    .shell_parse_one_cmd = _shell_parse_one_cmd,
    .rom_cmd_table = rom_cmd_table
};

SECTION_SHELL_CMD_TEXT
void shell_cmd_task_init(void)
{
    // assign the buffer for input command
    _shell_set_cmd_buf(&shell_cmd_hdl_rom, shell_cmd_buffer_rom, SHELL_CMD_BUF_SIZE);

    // assign the buffer for command history
    _shell_set_hist_buf(&shell_cmd_hdl_rom, shell_cmd_hist_rom, SHELL_CMD_HIST_SIZE);

    // hook the putc & getc functions
    _shell_init(&shell_cmd_hdl_rom, _stdio_port_getc, _stdio_port_putc, NULL);
    _shell_set_cmd_table(&shell_cmd_hdl_rom, rom_cmd_table);
//    _shell_set_prompt (&shell_cmd_hdl_rom, prompt_str_rom);

}

SECTION_SHELL_CMD_TEXT
void shell_cmd_task(void)
{
    _shell_task(&shell_cmd_hdl_rom);
}

SECTION_SHELL_CMD_TEXT
BOOL _shell_init_with_stdio_port(shell_command_t *pshell_cmd)
{
    return _shell_init(pshell_cmd, _stdio_port_getc, _stdio_port_putc, NULL);
}

SECTION_SHELL_CMD_TEXT
void rom_cmd_shell_set_prompt(void)
{
    _shell_set_prompt(&shell_cmd_hdl_rom, prompt_str_rom);
}

SECTION_SHELL_CMD_TEXT
void rom_shell_set_cmd_list(shell_command_entry_t *cmd_list, unsigned int list_size)
{
    _shell_set_cmd_list(&shell_cmd_hdl_rom, cmd_list, list_size);
}

SECTION_SHELL_CMD_TEXT
void rom_shell_unregister_all(void)
{
    _shell_unregister_all(&shell_cmd_hdl_rom);
}

SECTION_SHELL_CMD_TEXT
BOOL rom_shell_register(shell_program_t program, const char *cmd_string, const char *help_string)
{
    return _shell_register(&shell_cmd_hdl_rom, program, cmd_string, help_string);
}

