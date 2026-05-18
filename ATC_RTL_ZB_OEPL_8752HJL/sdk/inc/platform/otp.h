/**
****************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      otp.h
* @brief
* @date      2023-03-24
* @version   v1.0
* **************************************************************************************************
*/

#ifndef _OTP_H_
#define _OTP_H_

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    OS_TICK_10MS       = 0,
    OS_TICK_5MS        = 1,
    OS_TICK_2MS        = 2,
    OS_TICK_1MS        = 3,
} OS_TICK;

/**
 * @struct EFUSE_RAM_CONFIG
 * @brief RAM EFuse settings.
 *
 * Refer to EFUSE[0x].
 */
typedef struct RAM_CONFIG_
{
    uint16_t DS_cache_8_0: 9;
    uint16_t DS_lepa: 1;
    uint16_t SD_lepa: 1;
    uint16_t DS_lemst: 1;
    uint16_t SD_lemst: 1;
    uint16_t LS_snps_1_0: 2;
    uint16_t RSVD0: 1;

    uint16_t SD_cache_8_0: 9;
    uint16_t RSVD1: 7;

    uint16_t LS_retsram_4_0: 5;  // bit0-4: 32k + 32k + 32k + 4k +4k
    uint16_t DS_retsram_4_0: 5;
    uint16_t SD_retsram_4_0: 5;
    uint16_t RSVD2: 1;

    uint16_t LS_buffer: 1;
    uint16_t DS_buffer: 1;
    uint16_t SD_buffer: 1;
    uint16_t LS_audio: 1;
    uint16_t DS_audio: 1;
    uint16_t SD_audio: 1;
    uint16_t LS_rom: 1;
    uint16_t RSVD4: 1;
    uint16_t LS_zigbee_32x32_3_0: 4;
    uint16_t DS_zigbee_32x32_3_0: 4;

} __attribute__((packed)) RAM_CONFIG;

enum
{
    MEMCFG_ACTIVE,   // Bee3plus iso0 not control data ram, buffer ram and cache ram
    MEMCFG_LOWPOWER,
    MEMCFG_MAX
};

/* OTP start addr: 0x00200138 */
typedef struct otp_struct
{
    /****************boot config****************/
    /* start addr: 0x00200138, size: 0x7c */
    uint32_t SWD_ENABLE : 1;
    uint32_t boot_cfg_rsvd0 : 31;

    uint8_t boot_cfg_rsvd1[40];
    uint32_t ota_bank0_addr;
    uint32_t ota_bank0_size;
    uint32_t ota_bank1_addr;
    uint32_t ota_bank1_size;
    uint32_t ftl_addr;
    uint32_t ftl_size;
    uint32_t ota_tmp_addr;
    uint32_t ota_tmp_size;
    uint32_t bkp_data1_addr;
    uint32_t bkp_data1_size;
    uint32_t bkp_data2_addr;
    uint32_t bkp_data2_size;
    uint32_t HardFault_Record_CFG;
    uint32_t HardFault_Record_BegAddr;
    uint32_t HardFault_Record_EndAddr;
    uint8_t boot_cfg_rsvd2[20];

    /****************platform config****************/
    /* start addr: 0x002001b4, size: 0xc4 */
    uint64_t platform_cfg_rsvd0[4];

    uint16_t stack_en : 1;
    uint16_t platform_cfg_rsvd1 : 1;
    uint16_t run_in_app : 1;
    uint16_t ftl_init_in_rom : 1;
    uint16_t platform_cfg_rsvd2 : 12;

    uint32_t logPin : 6;                 /* default = P0_3 */
    uint32_t logChannel : 2;             /* LogChannel_TypeDef: default is LOG_CHANNEL_LOG1_UART */
    uint32_t logBaudRate : 5;            /* UartBaudRate_TypeDef: default is BAUD_RATE_2000000 */
    uint32_t platform_cfg_rsvd3 : 4;
    uint32_t logDisable : 1;             /* Disable all DBG_DIRECT and DBG_BUFFER log */
    uint32_t dump_info_before_reset : 1;
    uint32_t write_info_to_flash_before_reset : 1;
    uint32_t platform_cfg_rsvd4 : 1;
    uint32_t log_ram_type : 1;
    uint32_t ftl_mapping_table_ram_type : 1;
    uint32_t ftl_enable_write_skip : 1; /*if enabled, if read value equals write value will skip*/
    uint32_t ftl_logic_addr_map_bit_num : 3; /*4bit by step, default value is 3 means 12bit*/
    uint32_t platform_cfg_rsvd5 : 5;

    uint32_t aes_key[8];                 /* for OTA encryption/decryption */
    uint32_t reboot_record_address;      // 0x002001fa

    uint32_t reboot_record_item_limit_power_2 : 4;   /* max number of reboot records (2^n), 0x002003c0*/
    uint32_t platform_cfg_rsvd6: 12;
    uint32_t wdgIP   : 1;                /* default = 0 */
    uint32_t resetWhenHardFault : 1;
    uint32_t log_ram_size : 3;
    uint32_t hardfault_print_buf_log : 1;
    uint32_t platform_cfg_rsvd7: 8;
    uint32_t log_timestamp_src : 2;

    uint32_t platform_cfg_rsvd8: 13;
    uint32_t systick_clk_src: 1;         /* Select systick timer clock source */
    uint32_t platform_cfg_rsvd9: 12;
    uint32_t ftl_use_mapping_table: 1;
    uint32_t ftl_app_logical_addr_base: 3;
    uint32_t ftl_only_gc_in_idle: 1;
    uint32_t write_info_to_flash_when_hardfault: 1;

    uint32_t systick_ext_clk_freq;          //0x00200206
    uint32_t share_cache_ram_reg;           //0x0020020a
    uint32_t ftl_real_logic_addr_size;      /* FTL real logic address size */
    uint32_t os_tick_rate_HZ;

    uint16_t platform_cfg_rsvd10;

    uint32_t ota_use_randon_address: 1;
    uint32_t ota_adv_with_image_version: 1;
    uint32_t ota_with_encryption_data: 1;
    uint32_t ota_with_encryption_use_aes256: 1;
    uint32_t ota_with_encryption_aes_type: 2;
    uint32_t ota_resvd: 1;
    uint32_t ota_link_loss_reset: 1;
    uint32_t ota_use_buffer_check: 1;
    uint32_t ota_rsvd: 23;
    uint8_t ota_rst_tgt_name[8];
    uint8_t ota_timeout_total;   //0x00200224
    uint8_t ota_timeout_wait4_conn;
    uint8_t ota_timeout_wait4_image_transfer;
    uint8_t ota_timeout_ctittv;
    uint8_t ota_adv_random_address[6];

    RAM_CONFIG ram_cfg[MEMCFG_MAX];

    /****************end platform config****************/

    uint8_t rsvd1[58];

    /*****************os_cfg *********************/
    /* start addr: 0x00200278,padding 2 bytes to ensure 4 bytes align */
    uint8_t getStackHighWaterMark : 1;         /* 0 for release version, 1 for debug version */
    uint8_t checkForStackOverflow : 1;         /* 0 for release version, 1 for debug version */
    uint8_t printAllLogBeforeEnterDLPS : 1;    /* 0 for release version, 1 for debug version */
    uint8_t dumpMemoryWhenHardFaultOrWDG : 1;  /* 0 for release version, 1 for debug version */
    uint8_t dumpMemoryUsage : 1;               /* 0 for release version, 1 for debug version */
    uint8_t enableASSERT: 1;                   /* 0 for release version, 1 for debug version */
    uint8_t printDLPSCheckLog: 1;              /* default = 0 */
    uint8_t os_cfg_rsvd0: 1;

    uint8_t wdgConfigDivfactor;               //addr=0x00200379, default = 31
    uint8_t wdgConfigCntLimit : 4;            /* refer to WDG_Config(), default = 15 */
    uint8_t wdgEnableInRom : 1;               /* 1 for release version, 0 for debug version */
    uint8_t wdgResetInRom : 1;                /* 1 for release version, 0 for debug version */
    uint8_t wdgMode : 2;                      /* 0: interrupt CPU,     1: reset all but aon
                                                 2: reset core domain, 3: reset all */

    uint8_t timerMaxNumber;                   /* default = 0x20 */
    uint8_t timerQueueLength;                 /* default = 0x20 */
    uint8_t UseAliOS  : 1;
    uint8_t reserved  : 7;
    uint16_t dyn_malloc_task_stack_size;

    uint32_t appDataAddr;                     //0x00200280
    uint32_t appDataSize;                     //0x00200284
    uint32_t heapDataONSize;                  //0x00200288
    uint32_t heapBufferONSize;                //0x0020028c

    uint16_t idle_task_stack_size;            /* measured in bytes, default 256 * 4 bytes */
    uint16_t timer_task_stack_size;           /* measured in bytes, default 256 * 4 bytes */
    uint16_t lower_task_stack_size;           /* measured in bytes, default 768 * 4 bytes */

    uint8_t os_cfg_padding[2];
    /****************end os_cfg *******************/
}  __attribute__((packed)) T_OTP_CFG;
/*end otp, addr = 0x00200298*/

#define OTP_STRUCT_BASE             0x00200138UL
#define OTP                         ((T_OTP_CFG *) OTP_STRUCT_BASE)

#ifdef __cplusplus
}
#endif

#endif  /* _OTP_H_ */
