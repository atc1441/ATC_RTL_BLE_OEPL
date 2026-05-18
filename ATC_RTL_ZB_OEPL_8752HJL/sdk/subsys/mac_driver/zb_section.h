/**************************************************************************//**
 * @file     zb_section.h
 * @brief    define memory section name for zigbee.
 *
 * @version  V1.00
 * @date     2023-05-06
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2023 Realtek Corporation. All rights reserved.
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
#ifndef _ZB_SECTION_H_
#define _ZB_SECTION_H_

#if !defined(SECTION)
#define SECTION(_name) __attribute__ ((__section__(_name)))
#endif

#ifdef RT_PATCH
#define SECTION_ZB_ROM_TEXT
#define SECTION_ZB_ROM_RODATA
#define SECTION_ZB_ROM_DATA
#define SECTION_ZB_ROM_BSS
#else
#define SECTION_ZB_ROM_TEXT         SECTION(".rom.zb.text")
#define SECTION_ZB_ROM_RODATA       SECTION(".rom.zb.rodata")
#define SECTION_ZB_ROM_DATA         SECTION(".rom.zb.data")
#define SECTION_ZB_ROM_BSS          SECTION(".rom.zb.bss")
#endif

#define SECTION_ZB_PATCH_TEXT       SECTION(".patch.zb.text")
#define SECTION_ZB_PATCH_RODATA     SECTION(".patch.zb.rodata")
#define SECTION_ZB_PATCH_DATA       SECTION(".patch.zb.data")
#define SECTION_ZB_PATCH_BSS        SECTION(".patch.zb.bss")

#define SECTION_ZB_MP_ROM_TEXT      SECTION(".rom.zb_mp.text")
#define SECTION_ZB_MP_ROM_RODATA    SECTION(".rom.zb_mp.rodata")
#define SECTION_ZB_MP_ROM_DATA      SECTION(".rom.zb_mp.data")
#define SECTION_ZB_MP_ROM_BSS       SECTION(".rom.zb_mp.bss")

#define SECTION_ZB_MP_PATCH_TEXT    SECTION(".patch.zb_mp.text")
#define SECTION_ZB_MP_PATCH_RODATA  SECTION(".patch.zb_mp.rodata")
#define SECTION_ZB_MP_PATCH_DATA    SECTION(".patch.zb_mp.data")
#define SECTION_ZB_MP_PATCH_BSS     SECTION(".patch.zb_mp.bss")

#define SECTION_ZB_IPC_ROM_TEXT     SECTION(".rom.zb_ipc.text")
#define SECTION_ZB_IPC_ROM_RODATA   SECTION(".rom.zb_ipc.rodata")
#define SECTION_ZB_IPC_ROM_DATA     SECTION(".rom.zb_ipc.data")
#define SECTION_ZB_IPC_ROM_BSS      SECTION(".rom.zb_ipc.bss")

#define SECTION_ZB_IPC_IMG_HDR      SECTION(".zb_ipc.img_hdr")
#define SECTION_ZB_IPC_TEXT         SECTION(".zb_ipc.text")
#define SECTION_ZB_IPC_ITCM         SECTION(".zb_ipc.itcm.text")
#define SECTION_ZB_IPC_RODATA       SECTION(".zb_ipc.rodata")
#define SECTION_ZB_IPC_DATA         SECTION(".zb_ipc.data")
#define SECTION_ZB_IPC_BSS          SECTION(".zb_ipc.bss")
#define SECTION_ZB_IPC_SHARE_MEM    SECTION(".zb_ipc_share.bss")
#define SECTION_ZB_IPC_BUF_DESC     SECTION(".zb_ipc_buf_desc.rodata")

#define SECTION_ZB_PM_ROM_TEXT      SECTION(".rom.zb_pm.text")
#define SECTION_ZB_PM_ROM_RODATA    SECTION(".rom.zb_pm.rodata")
#define SECTION_ZB_PM_ROM_DATA      SECTION(".rom.zb_pm.data")
#define SECTION_ZB_PM_ROM_BSS       SECTION(".rom.zb_pm.bss")

#endif // _ZB_SECTION_H_
