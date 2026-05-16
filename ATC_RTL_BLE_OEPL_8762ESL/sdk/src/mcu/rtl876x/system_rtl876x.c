/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file    system_rtl8762c.c
   * @brief   system init file
   * @author  lory xu
   * @date    2017-11-9
   * @version v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include "version.h"
#include "rtl876x.h"
#include "patch_header_check.h"
#include "app_section.h"
#include "rom_uuid.h"
#include "app_define.h"
#include "core_cmFunc.h"
#include "mem_config.h"
#include "otp.h"
#include "platform_autoconf.h"
#include "rtl876x_wdg.h"
#include "overlay_mgr.h"
#include "flash_device.h"
#include "os_sched.h"
#include "os_sync.h"
#include "otp_config.h"
#include "test_mode.h"
#include "platform_utils.h"
#include "os_mem.h"
#include "dfu_flash.h"
#include "rtl876x_wdg.h"
#include "rtl876x_lib_platform.h"
#include "board.h"
#include "dlps.h"
#include "os_timer.h"
#include "trace.h"
#if (SUPPORT_NORMAL_OTA == 1)
#include "dfu_main.h"
#endif
#if (SYSTEM_TRACE_ENABLE == 1)
#include "system_trace.h"
#include "trace_config.h"
#endif

/** @defgroup  SYSTEM_INIT System Init
    * @brief Start up code for user application.
    * @{
    */
/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup SYSTEM_INIT_Exported_Macros System Init Exported Macros
    * @brief
    * @{
    */
void show_sdk_lib_version(void);
void SystemInit(void);
#define SHARE_CACHE_RAM_0K          0x82F70000
#define SHARE_CACHE_RAM_4K          0x2F2D0002
#define SHARE_CACHE_RAM_8K          0xA2AA0003

#define SHUTDOWN_DATARAM_4KB        (4 * 1024)
#define SHUTDOWN_DATARAM_8KB        (8 * 1024)
#define SHUTDOWN_DATARAM_24KB       (24 * 1024)
#define SHUTDOWN_DATARAM_56KB       (56 * 1024)

#define VTOR_RAM_ADDR               0x00200000 //!< vector table address in RAM.
#define APP_FAKE_PAYLOAD_LEN        0x100

#define FreeRTOS_portBYTE_ALIGNMENT 8

/** End of SYSTEM_INIT_Exported_Macros
    * @}
    */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup SYSTEM_INIT_Exported_Types System Init Exported Types
    * @{
    */
typedef struct
{
    uint8_t ic_type;
    uint8_t rsvd0[3];
    uint32_t ram_size;
    uint32_t check_pattern;
    T_VERSION_FORMAT git_ver;
    uint8_t rsvd1[64];
} T_UPPERSTACK_HEADER;

typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK *pxNextFreeBlock;   /*<< The next free block in the list. */
    size_t xBlockSize : 28;                 /*<< The size of the free block. */
    size_t xRamType : 3;
    size_t xAllocateBit : 1;
} BlockLink_t;
/** End of SYSTEM_INIT_Exported_Types
    * @}
    */


/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @defgroup SYSTEM_INIT_Exported_Variables System Init Exported Variables
    * @{
    */
extern void GPIO0_Handler(void);
extern void GPIO1_Handler(void);
extern void GPIO2_Handler(void);
extern void GPIO3_Handler(void);
extern void GPIO4_Handler(void);
extern void GPIO5_Handler(void);
extern void GPIO6_Handler(void);
extern void GPIO7_Handler(void);
extern void GPIO8_Handler(void);
extern void GPIO9_Handler(void);
extern void GPIO10_Handler(void);
extern void GPIO11_Handler(void);
extern void GPIO12_Handler(void);
extern void GPIO13_Handler(void);
extern void GPIO14_Handler(void);
extern void GPIO15_Handler(void);
extern void GPIO16_Handler(void);
extern void GPIO17_Handler(void);
extern void GPIO18_Handler(void);
extern void GPIO19_Handler(void);
extern void GPIO20_Handler(void);
extern void GPIO21_Handler(void);
extern void GPIO22_Handler(void);
extern void GPIO23_Handler(void);
extern void GPIO24_Handler(void);
extern void GPIO25_Handler(void);
extern void GPIO26_Handler(void);
extern void GPIO27_Handler(void);
extern void GPIO28_Handler(void);
extern void GPIO29_Handler(void);
extern void GPIO30_Handler(void);
extern void GPIO31_Handler(void);

#if defined (__CC_ARM)
extern char Image$$ENCRYPTION_RAM_CODE$$Base[];
extern char Load$$ENCRYPTION_RAM_CODE$$Base[];
extern char Load$$ENCRYPTION_RAM_CODE$$Length[];

extern char Image$$FLASH_START_ADDR$$RO$$Base[];
extern char Load$$FLASH_START_ADDR$$RO$$Base[];
extern char Load$$FLASH_START_ADDR$$RO$$Length[];
#elif defined ( __GNUC__ )
extern uint32_t *__encryption_ram_code_exe_ad__;
extern uint32_t *__encryption_ram_code_load_ad__;
extern uint32_t *__encryption_ram_code_length__;
#endif
extern uint32_t upperstack_fake_data;

BOOL_WDG_CB user_wdg_cb __attribute__((weak)) = NULL;
USER_CALL_BACK app_pre_main_cb __attribute__((weak)) = NULL ;
USER_CALL_BACK os_patch __attribute__((weak)) = NULL ;

#if (LOG_UART_MULTIPLEXING == 1)

#include "log_uart_dma.h"
typedef enum
{
    LOG_UART = 0,
    DATA_UART
} UART_STATE;

typedef struct
{
    UART_STATE last_flag;
    UART_STATE current_flag;
} T_UART_FLAG;

PingpongBuffer MCU_Log_user;
PingpongBuffer *pMCU_PPB_user = &MCU_Log_user;

T_UART_FLAG uart_flag = {LOG_UART, LOG_UART};
T_UART_FLAG *uart_flag_p = &uart_flag;

PingpongBuffer *get_tx_buffer_app_cb(void);
T_UART_FLAG *get_uart_flag_app_cb(void);
void get_tx_rx_pin_app_cb(uint8_t *p_tx, uint8_t *p_rx);

#endif

#if (LOG_UART_MULTIPLEXING == 1)

//increase APP_CB_NUMBERS when add new app cb
#define APP_CB_NUMBERS    4

typedef enum
{
    APP_CB_ID_WDG_RESET = 0,
    APP_CB_ID_RX_BUFFER = 1,
    APP_CB_ID_UART_FLAG = 2,
    APP_CB_ID_TX_RX_PIN = 3,
    //add more app cb id here
} T_APP_CB_ID;

#else

//increase APP_CB_NUMBERS when add new app cb
#define APP_CB_NUMBERS    1

typedef enum
{
    APP_CB_ID_WDG_RESET = 0,
    //add more app cb id here
} T_APP_CB_ID;

#endif

typedef struct
{
    uint32_t app_cb_signature;
    uint32_t app_cb_numbers;
    uint32_t app_cb_addr[APP_CB_NUMBERS];
} T_APP_CB_TABLE;

const T_APP_CB_TABLE app_cb_table =
{
    .app_cb_signature = SIGNATURE_APP_CB,
    .app_cb_numbers = APP_CB_NUMBERS,
    .app_cb_addr[0] = (uint32_t)NULL,
#if (LOG_UART_MULTIPLEXING == 1)
    .app_cb_addr[1] = (uint32_t)get_tx_buffer_app_cb,
    .app_cb_addr[2] = (uint32_t)get_uart_flag_app_cb,
    .app_cb_addr[3] = (uint32_t)get_tx_rx_pin_app_cb,
#endif
    //add more cb here
};


#if FEATURE_ENCRYPTION
#if defined ( __CC_ARM ) || defined ( __GNUC__ )
#define ENC_ALIGN_SECTION __attribute__((aligned(16), used, section(".enc.dummy.align")));
const uint8_t enc_dummy_align[16] ENC_ALIGN_SECTION;
#else
//#define ENC_ALIGN_SECTION @ ".enc.dummy.align"
//#pragma data_alignment=16
//__root const uint8_t enc_dummy_align[16] ENC_ALIGN_SECTION;
#endif
#endif


#if defined (__CC_ARM)
#pragma push
#pragma diag_suppress 1296 /* disable warning 1296(extened constant initialiser used)*/
#endif

/**
* @brief: application header.
* @note: items in ENCRYPT_RAM_CODE macro is for encrytion solution only
*/

const T_IMG_HEADER_FORMAT img_header APP_FLASH_HEADER =
{
    .ctrl_header =
    {
        .ic_type = DEFINED_IC_TYPE,
        .secure_version = 0,
#if FEATURE_ENCRYPTION
        .ctrl_flag.flag_value.enc = 1,
        .ctrl_flag.flag_value.xip = 0,
        .ctrl_flag.flag_value.load_when_boot = 1,
        .ctrl_flag.flag_value.enc_key_select = ENC_KEY_OCEK_WITH_OEMCONST,
#else
        .ctrl_flag.flag_value.xip = 1,
        .ctrl_flag.flag_value.enc = 0,
        .ctrl_flag.flag_value.load_when_boot = 0,
        .ctrl_flag.flag_value.enc_key_select = 0,
#endif
        .ctrl_flag.flag_value.enc_load = 0,
        .ctrl_flag.flag_value.not_ready = 0,
        .ctrl_flag.flag_value.not_obsolete = 1,
        .ctrl_flag.flag_value.compressed_not_ready = 0,
        .ctrl_flag.flag_value.compressed_not_obsolete = 1,
#if (BOOT_INTEGRITY_CHECK_EN == 0)
        .ctrl_flag.flag_value.integrity_check_en_in_boot = 0,
#else
        .ctrl_flag.flag_value.integrity_check_en_in_boot = 1,
#endif
        .image_id = AppPatch,
        .payload_len = APP_FAKE_PAYLOAD_LEN,    //Will modify by build tool later
    },
    .uuid = DEFINE_rom_uuid,

#if FEATURE_ENCRYPTION
#if defined ( __ICCARM__ )
    .exe_base = (uint32_t)__section_begin(".app.encryption.text"),
    .load_base = (uint32_t)__section_begin(".app.encryption.text_init"),
    .load_len = (uint32_t)__section_size(".app.encryption.text"),
#elif defined (__CC_ARM)
    .load_base = (uint32_t)Load$$ENCRYPTION_RAM_CODE$$Base,
    .exe_base = (uint32_t)Image$$ENCRYPTION_RAM_CODE$$Base,
    .load_len = (uint32_t)Load$$ENCRYPTION_RAM_CODE$$Length,
#else
    .exe_base = (uint32_t) &__encryption_ram_code_exe_ad__, //(uint32_t)IMAGE_FLASH_START_ADDR,
    .load_base = (uint32_t) &__encryption_ram_code_load_ad__,
    .load_len = (uint32_t) &__encryption_ram_code_length__,
#endif
#else
#if defined ( __ICCARM__ )
    .load_base = 0,
    .exe_base = (uint32_t)SystemInit,
#elif defined (__CC_ARM)
    .load_base = (uint32_t)Load$$FLASH_START_ADDR$$RO$$Base,
    .exe_base = (uint32_t)Image$$FLASH_START_ADDR$$RO$$Base,
#else
    .load_base = 0,
    .exe_base = (uint32_t) SystemInit,
#endif
    .load_len = 0,  //0 indicates all XIP
#endif
#if (APP_BANK == 0)
    .image_base = BANK0_APP_ADDR,
#else
    .image_base = BANK1_APP_ADDR,
#endif

    .git_ver =
    {
        .ver_info.sub_version._version_major = VERSION_MAJOR,
        .ver_info.sub_version._version_minor = VERSION_MINOR,
        .ver_info.sub_version._version_revision = VERSION_REVISION,
        .ver_info.sub_version._version_reserve = VERSION_BUILDNUM % 32, //only 5 bit
        ._version_commitid = VERSION_GCID,
        ._customer_name = {CN_1, CN_2, CN_3, CN_4, CN_5, CN_6, CN_7, CN_8},
    },

#ifdef OTA_MAGIC_PATTERN
    .ota_magic_pattern = OTA_MAGIC_PATTERN,
#endif

    .app_cb_signature = SIGNATURE_APP_CB,
    .app_cb_table_base_address = (uint32_t) &app_cb_table
};
#if defined ( __ICCARM__   ) || defined ( __CC_ARM   )
const T_AUTH_HEADER_FORMAT auth_header APP_FLASH_HEADER =
{
#if defined ( __ICCARM__ )
    .header_mac = {0xFF},
    .payload_mac = {0xFF},
#else
    .header_mac = {[0 ... 15] = 0xFF},
    .payload_mac = {[0 ... 15] = 0xFF},
#endif
};
#elif defined ( __GNUC__   )
const T_AUTH_HEADER_FORMAT auth_header APP_FLASH_HEADER_AUTH =
{
    .header_mac = {[0 ... 15] = 0xFF},
    .payload_mac = {[0 ... 15] = 0xFF},
};
#endif
#if defined ( __CC_ARM   )
#pragma pop
#endif
extern void upperstack_loader(void);
void AppUpdateVectorTable(void);
void pre_main(void);
void wdg_system_reset_app_cb(T_WDG_MODE wdg_mode, T_SW_RESET_REASON reset_reason);
uint32_t random_seed_value;

/** End of SYSTEM_INIT_Exported_Variables
    * @}
    */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup SYSTEM_INIT_Exported_Functions System Init Exported Functions
    * @{
    */
void random_seed_init(void)
{
    random_seed_value = platform_random(0xFFFFFFFF);
}

void common_main(void)
{
#if (SUPPORT_FTL_IN_APP == 1)
    extern void ftl_patch_point_init(void);
    ftl_patch_point_init();

    extern uint32_t ftl_init(uint32_t u32PageStartAddr, uint8_t pagenum);
    ftl_init(flash_get_bank_addr(FLASH_FTL),
             flash_get_bank_size(FLASH_FTL) / OTP->PageSize);
#else
    extern uint32_t ftl_init(uint32_t u32PageStartAddr, uint8_t pagenum);
    ftl_init(flash_get_bank_addr(FLASH_FTL),
             flash_get_bank_size(FLASH_FTL) / OTP->PageSize);
#endif

    //add common system code here before enter user defined main function
#if (SUPPORT_NORMAL_OTA == 1)
    if (dfu_check_ota_mode_flag())
    {
        dfu_main();
        dfu_set_ota_mode_flag(false);
        goto START_SCHEDULER;
    }
#endif

    OTP->run_in_app = 1;

    random_seed_init();

#if (RUN_APP_IN_HCIMODE_ENABLE == 0)
    if (OTP->stack_en)
    {
        DBG_DIRECT("In SoC Mode");
    }
    else
    {
        DBG_DIRECT("WARNING: In HCI Mode, will not run APP Task");
        WDG_Disable();
        goto START_SCHEDULER;
    }
#endif

#if (LOG_UART_MULTIPLEXING == 1)
    PPB_Init(pMCU_PPB_user);
#endif

    //print sdk lib version
    show_sdk_lib_version();

#if (SYSTEM_TRACE_ENABLE == 1)
    extern void system_trace_init(void);
    system_trace_init();
#endif

#if defined ( __ICCARM__ )
    extern void __iar_program_start(const int);
    __iar_program_start(0);
#elif defined (__CC_ARM)
    extern int __main(void);
    __main();
#elif defined (__GNUC__)
    extern int main(void);
    main();
#endif

START_SCHEDULER:
    os_sched_start();
    return;
}


APP_FLASH_TEXT_SECTION
void ram_init(void)
{
#if defined ( __CC_ARM   )
    //copy data on ro
    extern char Image$$RAM_DATA_ON$$RO$$Base[];
    extern char Load$$RAM_DATA_ON$$RO$$Base[];
    extern unsigned int Image$$RAM_DATA_ON$$RO$$Length;

    memcpy(Image$$RAM_DATA_ON$$RO$$Base,
           Load$$RAM_DATA_ON$$RO$$Base,
           (unsigned int)&Image$$RAM_DATA_ON$$RO$$Length);

    //copy data on rw
    extern char Image$$RAM_DATA_ON$$RW$$Base[];
    extern char Load$$RAM_DATA_ON$$RW$$Base[];
    extern unsigned int Image$$RAM_DATA_ON$$RW$$Length;

    memcpy(Image$$RAM_DATA_ON$$RW$$Base,
           Load$$RAM_DATA_ON$$RW$$Base,
           (unsigned int)&Image$$RAM_DATA_ON$$RW$$Length);

    //clear data on zi
    extern char Image$$RAM_DATA_ON$$ZI$$Base[];
    extern unsigned int Image$$RAM_DATA_ON$$ZI$$Length;

    memset(Image$$RAM_DATA_ON$$ZI$$Base,
           0,
           (unsigned int)&Image$$RAM_DATA_ON$$ZI$$Length);
#elif defined ( __GNUC__ )
    //copy data on ro
    extern uint32_t *__ram_dataon_ro_start__;
    extern uint32_t *__ram_ro_load_ad__;
    extern uint32_t *__ram_dataon_ro_length__;

    memcpy(&__ram_dataon_ro_start__,
           &__ram_ro_load_ad__,
           (unsigned int)&__ram_dataon_ro_length__);
    //copy data on rw
    extern uint32_t *__ram_dataon_rw_start__;
    extern uint32_t *__ram_rw_load_ad__;
    extern uint32_t *__ram_dataon_rw_length__;

    memcpy(&__ram_dataon_rw_start__,
           &__ram_rw_load_ad__,
           (unsigned int)&__ram_dataon_rw_length__);

    //clear data on zi
    extern uint32_t *__ram_dataon_zi_start__;
    extern uint32_t *__ram_dataon_zi_length__;

    memset(&__ram_dataon_zi_start__,
           0,
           (unsigned int)&__ram_dataon_zi_length__);
#elif  defined (__ICCARM__)
    extern void __iar_data_init2(void);
    __iar_data_init2();
#endif

}

APP_FLASH_TEXT_SECTION
void ram_cache_init(void)
{
#if defined ( __CC_ARM   )
    //copy cache ro
    extern char Image$$CACHE_DATA_ON$$RO$$Base[];
    extern char Load$$CACHE_DATA_ON$$RO$$Base[];
    extern unsigned int Image$$CACHE_DATA_ON$$RO$$Length;

    memcpy(Image$$CACHE_DATA_ON$$RO$$Base,
           Load$$CACHE_DATA_ON$$RO$$Base,
           (unsigned int)&Image$$CACHE_DATA_ON$$RO$$Length);

    //copy share cache ram rw
    extern char Image$$CACHE_DATA_ON$$RW$$Base[];
    extern char Load$$CACHE_DATA_ON$$RW$$Base[];
    extern unsigned int Image$$CACHE_DATA_ON$$RW$$Length;

    memcpy(Image$$CACHE_DATA_ON$$RW$$Base,
           Load$$CACHE_DATA_ON$$RW$$Base,
           (unsigned int)&Image$$CACHE_DATA_ON$$RW$$Length);

    //clear share cache ram zi
    extern char Image$$CACHE_DATA_ON$$ZI$$Base[];
    extern unsigned int Image$$CACHE_DATA_ON$$ZI$$Length;

    memset(Image$$CACHE_DATA_ON$$ZI$$Base,
           0,
           (unsigned int)&Image$$CACHE_DATA_ON$$ZI$$Length);
#elif defined ( __GNUC__   )
    //copy share cache ram rw
    extern uint32_t *__cache_dataon_start__;
    extern uint32_t *__cache_data_load_ad__;
    extern uint32_t *__cache_dataon_length__;

    memcpy(&__cache_dataon_start__,
           &__cache_data_load_ad__,
           (unsigned int)&__cache_dataon_length__);
#endif
}


APP_FLASH_TEXT_SECTION
APP_MAIN_FUNC get_image_entry_addr(uint16_t image_id)
{

    uint32_t active_ota_bank_addr =  get_active_ota_bank_addr();
    if (!check_header_valid(active_ota_bank_addr, OTA))
    {
        return NULL;
    }

    T_OTA_HEADER_FORMAT *ota_header = (T_OTA_HEADER_FORMAT *)active_ota_bank_addr;
    uint32_t image_size =
        HAL_READ32((uint32_t)&ota_header->secure_boot_size, (image_id - SecureBoot) * 8);

    if (image_size == 0)
    {
        return NULL;
    }

    uint32_t header_addr = get_header_addr_by_img_id((T_IMG_ID)image_id);

    if (!check_header_valid(header_addr, (T_IMG_ID)image_id))
    {
        return NULL;
    }

    T_IMG_HEADER_FORMAT *header = (T_IMG_HEADER_FORMAT *)header_addr;

    if (header->ctrl_header.image_id != image_id || image_id >= IMAGE_MAX)
    {
        return NULL;
    }

    APP_MAIN_FUNC entry_func = (APP_MAIN_FUNC)(header->exe_base | 1);

    return entry_func;
}


void set_os_clock(OS_TICK os_tick) APP_FLASH_TEXT_SECTION;
void set_os_clock(OS_TICK os_tick)
{
    switch (os_tick)
    {
    case OS_TICK_10MS:
        OTP->os_tick_rate_HZ = 100;
        break;
    case OS_TICK_5MS:
        OTP->os_tick_rate_HZ = 200;
        break;
    case OS_TICK_2MS:
        OTP->os_tick_rate_HZ = 500;
        break;
    case OS_TICK_1MS:
        OTP->os_tick_rate_HZ = 1000;
        break;
    default:
        break;
    }
}

#if (SUPPORT_BRANCH_TO_ANOTHER_APP == 1)
#define ANOTHER_APP_IMG_ADDR         (get_temp_ota_bank_addr_by_img_id(AppPatch) + 4 * 1024)

#if defined ( __ICCARM__ )
#if FEATURE_ENCRYPTION
APP_MAIN_FUNC get_app_image_entry_addr(uint16_t image_id,
                                       uint32_t header_addr) APP_ENCRYPTION_TEXT_SECTION;
#else
APP_MAIN_FUNC get_app_image_entry_addr(uint16_t image_id,
                                       uint32_t header_addr) APP_FLASH_TEXT_SECTION;
#endif
#else
APP_MAIN_FUNC get_app_image_entry_addr(uint16_t image_id,
                                       uint32_t header_addr) APP_FLASH_TEXT_SECTION;
#endif

APP_MAIN_FUNC get_app_image_entry_addr(uint16_t image_id, uint32_t header_addr)
{
    /*check ic_type, not_ready, image_id, uuid*/
    if (!check_header_valid(header_addr, (T_IMG_ID)image_id))
    {
        return NULL;
    }

    T_IMG_HEADER_FORMAT *header = (T_IMG_HEADER_FORMAT *)header_addr;

    if (image_id <= OTA || image_id >= IMAGE_MAX)
    {
        return NULL;
    }

    APP_MAIN_FUNC entry_func = (APP_MAIN_FUNC)(header->exe_base | 1);

    return entry_func;
}
#endif

#if defined ( __ICCARM__ )
#if FEATURE_ENCRYPTION
void SystemInit(void) APP_ENCRYPTION_TEXT_SECTION;
#else
void SystemInit(void) APP_FLASH_TEXT_SECTION;
#endif
#else
void SystemInit(void) APP_FLASH_TEXT_SECTION;
#endif

void SystemInit(void)
{
    //hci mode check and bypass app
    if (check_hci_mode_flag() || (OTP->stack_en == 0))
    {
        return;
    }

#if (SUPPORT_BRANCH_TO_ANOTHER_APP == 1)
    APP_MAIN_FUNC entry_func = NULL;
    entry_func = (APP_MAIN_FUNC)get_app_image_entry_addr(AppPatch, ANOTHER_APP_IMG_ADDR);

    if (entry_func)
    {
        entry_func();

        return;
    }
#endif

    if (!check_hci_mode_flag())
    {
        Pinmux_Deinit_rom(P3_0);
        Pinmux_Deinit_rom(P3_1);
    }

    //init pre_main and main functions
    app_pre_main = (APP_MAIN_FUNC)pre_main;
    upperstack_entry = get_image_entry_addr(UpperStack);;
    app_main = (APP_MAIN_FUNC)common_main;
    app_cb_wdg_reset = (APP_CB_WDG_RESET_TYPE)wdg_system_reset_app_cb;

    /******** update otp here**********/
    //ram layout update based on App's setting
    OTP->appDataAddr = APP_GLOBAL_ADDR;
    update_ram_layout(APP_GLOBAL_SIZE, HEAP_DATA_ON_SIZE, SHARE_CACHE_RAM_SIZE);

#ifdef SHUTDOWN_DATARAM_SIZE
    if (SHUTDOWN_DATARAM_SIZE == 0)
    {
        set_dataram_to_shutdown(NO_DATARAM_SHUTDOWN);
    }
    else if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_4KB)
    {
        set_dataram_to_shutdown(LAST_4K_DATARAM_SHUTDOWN);
    }
    else if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_8KB)
    {
        set_dataram_to_shutdown(LAST_8K_DATARAM_SHUTDOWN);
    }
    else if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_24KB)
    {
        set_dataram_to_shutdown(LAST_24K_DATARAM_SHUTDOWN);
    }
    else if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_56KB)
    {
        set_dataram_to_shutdown(LAST_56K_DATARAM_SHUTDOWN);
    }
    else
    {
        DBG_DIRECT("Error! SHUTDOWN_DATARAM_SIZE is not match the size that can be set!");
    }
#endif

#ifdef SUPPORT_SINGLE_BANK_OTA_USER_DATA
    OTP->bkp_data1_addr = USER_DATA_START_ADDR;
    OTP->bkp_data1_size = USER_DATA_MAX_SIZE;
#endif

#if (WRITE_HARDFAULT_RECORD_TO_FLASH_ENABLE > 0) && (SYSTEM_TRACE_ENABLE == 1) && (TRACE_HARDFAULT == 1)
    patch_hardfault_save_to_flash_init();
#endif

#if ((SYSTEM_TRACE_ENABLE == 1) && ((WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE > 0 && TRACE_WDG_TIMEOUT == 1) || (TRACE_TASK_HANG_USE_WDG_ISR == 1)))
    patch_wdg_timeout_reason_save_to_flash_init();
#endif


    //sw timer config
#ifdef TIMER_MAX_NUMBER
    //define TIMER_MAX_NUMBER in otp_config.h
    OTP->timerMaxNumber = TIMER_MAX_NUMBER;
#endif

    //timer task stack size config
#ifdef TIMER_TASK_STACK_SIZE
    //define TIMER_TASK_STACK_SIZE in otp_config.h
    OTP->timer_task_stack_size = TIMER_TASK_STACK_SIZE;
#endif

    //flash config
    /*config enable flash block proect depending on flash layout and flash id*/
#if (FLASH_BLOCK_PROTECT_ENABLE == 1)
    OTP->bp_enable = 1;
#else
    OTP->bp_enable = 0;
#endif
    OTP->delay_10us_after_toggle_cs = AFTER_TOGGLE_CS_DELAY;

#if (CHK_IMG_INTEGRITY_USE_AUTO_MODE_ACCESS_FLASH == 1)
    OTP->image_split_read = 0; //use flash auto mode to check image integrity
#endif


    //os config
    /*config enable check task stack overflow*/
#if (CHECK_STACK_OVERFLOW_ENABLE == 1)
    OTP->checkForStackOverflow = 1;
#else
    OTP->checkForStackOverflow = 0;
#endif


    //platform config
    /*config enable platform assert*/
#if (PLATFORM_ASSERT_ENABLE == 1)
    OTP->enableASSERT = 1;
#else
    OTP->enableASSERT = 0;
#endif

    /*Print all log in log buffer before entering DLPS */
#if (CHECK_LOG_BUFFER_BEFORE_DLPS_ENABLE == 1)
    OTP->printAllLogBeforeEnterDLPS = 1;
#else
    OTP->printAllLogBeforeEnterDLPS = 0;
#endif

    /*config enable log or not*/
#if (CONFIG_LOG_FUNCTION_ENABLE == 1)
    OTP->logDisable = 0;
#else
    OTP->logDisable = 1;
#endif

    /*config log baudrate*/
#ifdef LOG_BAUD_RATE
    OTP->logBaudRate = LOG_BAUD_RATE;
#endif

#if (FTL_REAL_LOGIC_ADDR_SIZE > 0)
    if (FTL_REAL_LOGIC_ADDR_SIZE <= (((((FMC_PAGE_SIZE / 8) - 1) * ((OTP->ftl_size / FMC_PAGE_SIZE) -
                                                                    1)) - 1) << 2))
    {
        OTP->ftl_real_logic_addr_size = FTL_REAL_LOGIC_ADDR_SIZE;
    }
    else
    {
        DBG_DIRECT("ERROR! FTL logic addr size is too large!");
    }
#endif

#if (FTL_BT_STORAGE_SPACE_SIZE > 0)
    if (FTL_BT_STORAGE_SPACE_SIZE * 1024 <= OTP->ftl_real_logic_addr_size)
    {
        OTP->ftl_app_logical_addr_base = FTL_BT_STORAGE_SPACE_SIZE;
    }
    else
    {
        DBG_DIRECT("ERROR! FTL BT storage space size is too large!");
    }
#endif

    /*if app don't use CTC, can't switch power domain to avoid leakage*/
#if ( USE_CTC_DLPS == 1)
    OTP->ldo_311_aux_power_domain = PON_DOMAIN;
#endif

    /*to fix bug need disable dump callstack info before WDG_SystemReset, default enable */
#if (DUMP_INFO_BEFORE_RESET_DISABLE == 1)
    OTP->dump_info_before_reset = 0;
#endif

    /*Debug: config enable write hardfault record to flash*/
#if (WRITE_HARDFAULT_RECORD_TO_FLASH_ENABLE > 0)
    OTP->write_info_to_flash_when_hardfault = WRITE_HARDFAULT_RECORD_TO_FLASH_ENABLE;
    OTP->HardFault_Record_BegAddr = HARDFAULT_RECORD_BEG_ADDR;
    OTP->HardFault_Record_EndAddr = HARDFAULT_RECORD_END_ADDR;
    OTP->HardFault_Record_CFG = HARDFAULT_RECORD_CFG;
#endif

    /*before wdg system reset, write reset reason to specific flash addr*/
#if (WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE > 0)
    OTP->write_info_to_flash_before_reset = WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE;
    OTP->reboot_record_address = REBOOT_REASON_RECORD_ADDRESS;
    OTP->reboot_record_item_limit_power_2 = REBOOT_REASON_RECORD_LIMIT_POWERT2;
#endif

    /*config enable swd pinmux*/
#if (SWD_PINMUX_ENABLE == 1)
    OTP->SWD_ENABLE = 1;
#else
    OTP->SWD_ENABLE = 0;
#endif

    /*config enable watch dog in rom*/
#if (ROM_WATCH_DOG_ENABLE == 1)
    OTP->wdgEnableInRom = 1;
#else
    OTP->wdgEnableInRom = 0;
#endif

    /*config watch dog mode in rom, defualt 4s timeout and reset all*/
#if (ROM_WATCH_DOG_ENABLE == 1)
    OTP->wdgConfigDivfactor = ROM_WATCH_DOG_CFG_DIV_FACTOR;
    OTP->wdgConfigCntLimit = ROM_WATCH_DOG_CFG_CNT_LIMIT;
#if (TRACE_TASK_HANG_USE_WDG_ISR == 1)
    OTP->wdgMode = INTERRUPT_CPU;
#else
    OTP->wdgMode = ROM_WATCH_DOG_MODE;
#endif
#endif

    /*config enable write hardfault record to flash*/
#if (ENABLE_WRITE_HARDFAULT_RECORD_TO_FLASH > 0)
    OTP->write_info_to_flash_when_hardfault = ENABLE_WRITE_HARDFAULT_RECORD_TO_FLASH;
    OTP->HardFault_Record_BegAddr = HARDFAULT_RECORD_BEG_ADDR;
    OTP->HardFault_Record_EndAddr = HARDFAULT_RECORD_END_ADDR;
    OTP->HardFault_Record_CFG = HARDFAULT_RECORD_CFG;
#endif

#if (ENABLE_FLASH_READ_TURN_OFF_RF > 0)
    OTP->read_turn_on_off_rf = ENABLE_FLASH_READ_TURN_OFF_RF;
#endif


    //app config
    OTP->ota_timeout_total = OTA_TIMEOUT_TOTAL;
    OTP->ota_timeout_wait4_conn = OTA_TIMEOUT_WAIT4_CONN;
    OTP->ota_timeout_wait4_image_transfer = OTA_TIMEOUT_WAIT4_IMAGE_TRANS;
    OTP->ota_timeout_ctittv = OTA_TIMEOUT_CTITTV;

    /*config default value because extended in app*/
    OTP->ota_timeout_wait_packet = 5;
#if OTA_TIMEOUT_WAIT_PACKET
    OTP->ota_timeout_wait_packet = OTA_TIMEOUT_WAIT_PACKET;
#endif

#if ROM_OTA_LINKLOSS_RST
    OTP->ota_link_loss_reset = 1;
#endif

#ifdef OS_TICK_TIME
    set_os_clock(OS_TICK_TIME);
#endif

    /*config bt stack parameters in rom*/
#ifdef BT_STACK_CONFIG_ENABLE
    bt_stack_config_init();
#endif

//add more otp config here

    ram_init();

    if (os_patch)
    {
        os_patch();
    }
}

void print_reset_reason(void) APP_FLASH_TEXT_SECTION;
void print_reset_reason(void)
{
    uint32_t wdg_reset_pc = (btaon_fast_read_safe(0x1c2) << 17) | (btaon_fast_read_safe(0x1c0) << 1);
    uint32_t wdg_reset_lr = (btaon_fast_read_safe(0x1c6) << 16) | (btaon_fast_read_safe(0x1c4));
    uint32_t wdg_reset_psr = (btaon_fast_read_safe(0x1ca) << 16) | (btaon_fast_read_safe(0x1c8));

    uint32_t aon_wdg_reset_pc = (btaon_fast_read_safe(0x1ce) << 17) | (btaon_fast_read_safe(
                                                                           0x1cc) << 1);
    uint32_t aon_wdg_reset_lr = (btaon_fast_read_safe(0x1d2) << 16) | (btaon_fast_read_safe(0x1d0));
    uint32_t aon_wdg_reset_psr = (btaon_fast_read_safe(0x1d6) << 16) | (btaon_fast_read_safe(0x1d4));

    if (wdg_reset_lr == 0)
    {
        T_SW_RESET_REASON sw_reset_type = reset_reason_get();

        if (sw_reset_type == RESET_REASON_POWERDOWN)
        {
            BOOT_PRINT_INFO0("[Debug Info]RESET Reason: Wakeup From Powerdown");
        }
        else
        {
            BOOT_PRINT_INFO0("[Debug Info]RESET Reason: HW");
        }

        uint32_t wdg_reg_backup = WDG->WDG_CTL;

#if (TRACE_TASK_HANG_USE_WDG_ISR == 1)
        T_WDG_MODE reset_mode = (T_WDG_MODE)((wdg_reg_backup >> 29) & 0x3);
        if (INTERRUPT_CPU == reset_mode)
        {
            NVIC_DisableIRQ(WDG_IRQn);
        }
#endif

        //trigger wdg reset interrupt to update wdg_reset_lr (register is Read-Only, can't write directly)
        WDG_ClockEnable();
        WDG_Config(1, 0, INTERRUPT_CPU);
        WDG_Enable();
        platform_delay_ms(1);    //add delay to make lr as a stable value (0x7bb5 for RTL8762E A-cut)

        WDG_Disable();
        platform_delay_us(150);  //wait for WDG is disabled

#if (TRACE_TASK_HANG_USE_WDG_ISR == 1)
        if (INTERRUPT_CPU == reset_mode)
        {
            NVIC_ClearPendingIRQ(WDG_IRQn);
            NVIC_EnableIRQ(WDG_IRQn);
        }
#endif
        WDG->WDG_CTL = wdg_reg_backup;
    }
    else
    {
        T_SW_RESET_REASON sw_reset_type = reset_reason_get();

        //fix reset reason error when watchdog timeout and ROM_WATCH_DOG_MODE is RESET_ALL
        if (sw_reset_type == RESET_REASON_HW)
        {
            sw_reset_type = RESET_REASON_WDG_TIMEOUT;
        }

        BOOT_PRINT_INFO1("[Debug Info]RESET Reason: SW, TYPE 0x%x", sw_reset_type);
        BOOT_PRINT_INFO3("[Debug Info]WDG: pc %x, lr %x, psr %x", wdg_reset_pc, wdg_reset_lr,
                         wdg_reset_psr);
        BOOT_PRINT_INFO3("[Debug Info]AON WDG: pc %x, lr %x, psr %x", aon_wdg_reset_pc, aon_wdg_reset_lr,
                         aon_wdg_reset_psr);
    }
}

APP_FLASH_TEXT_SECTION
void pre_main(void)
{
    __disable_irq();

    print_reset_reason();

    ram_cache_init();
#if defined ( __CC_ARM   )
    load_overlay(OVERLAY_SCENARIO_BOOT_ONCE);
#endif
    setlocale(LC_ALL, "C");


    BOOT_PRINT_ERROR2("SDK Ver: %s, Build Time: %s",
                      TRACE_STRING(VERSION_BUILD_STR),
                      TRACE_STRING(BUILDING_TIME));

    AppUpdateVectorTable();



    if (app_pre_main_cb)
    {
        app_pre_main_cb();
    }

    return;
}

APP_FLASH_TEXT_SECTION
void wdg_system_reset_app_cb(T_WDG_MODE wdg_mode, T_SW_RESET_REASON reset_reason)
{
    //do something necessary before watch dog reset
    if (user_wdg_cb)
    {
        if (user_wdg_cb(wdg_mode, reset_reason))
        {
            return;
        }
    }

}

__attribute__((weak)) void show_sdk_lib_version(void)
{
}

/**
 * @brief  update vector table in app
 * @param  none
 * @return none
  */
OVERLAY_SECTION_BOOT_ONCE
void AppUpdateVectorTable(void)
{
#if defined ( __CC_ARM )
    extern uint32_t Load$$RAM_VECTOR_TABLE$$RO$$Base;
    extern uint32_t Image$$RAM_VECTOR_TABLE$$RO$$Length;
#elif defined ( __GNUC__ )
    extern uint32_t *__ram_vector_load_ad__;
    extern uint32_t *__ram_vector_table_length__;
#endif
    extern void Default_Handler(void);
    extern void ROM_Default_Handler(void);

    const char *SysException[] =
    {
        "InitialSP", "Reset", "NMI", "HardFault", "MemManage", "BusFault", "UsageFault", "Rsvd",
        "Rsvd", "Rsvd", "Rsvd", "SVC", "DebugMon", "Rsvd", "PendSV", "SysTick"
    };
    const char *ExtIrq[] =
    {
        "System", "WDG", "BTMAC", "TIM3", "TIM2", "Platform", "I2S0_TX", "I2S0_RX", "Timer4-5",
        "GPIO4", "GPIO5", "UART1", "UART0", "RTC", "SPI0", "SPI1", "I2C0", "I2C1", "ADC",
        "Peripheral", "GDMA0 Channel0", "GDMA0 Channel1", "GDMA0 Channel2", "GDMA0 Channel3",
        "Enhanced_Timer0", "Enhanced_Timer1", "GPIO_Group3", "GPIO_Group2", "IR", "GPIO_Group1",
        "GPIO_Group0", "Rsvd", "TIM4", "TIM5", "SPI_Flash", "Qdecode",
        "Keyscan", "SPI2W", "LPCOMP", "PTA_Mailbox", "CAP_Touch", "TRNG"
    };

    IRQ_Fun *pRamVector    = (IRQ_Fun *)VTOR_RAM_ADDR;
#if defined ( __ICCARM__ )
#pragma section = ".intvec"
    IRQ_Fun *pAppVector    = (IRQ_Fun *)__section_begin(".intvec");
    uint32_t AppVectorSize = (uint32_t)__section_size(".intvec");
    DBG_DIRECT("pAppVector = 0x%x, AppVectorSize = %d", pAppVector, AppVectorSize);
#elif defined (__CC_ARM)
    IRQ_Fun *pAppVector    = (IRQ_Fun *)&Load$$RAM_VECTOR_TABLE$$RO$$Base;
    uint32_t AppVectorSize = (uint32_t)&Image$$RAM_VECTOR_TABLE$$RO$$Length;
#elif defined (__GNUC__)
    IRQ_Fun *pAppVector    = (IRQ_Fun *)(&__ram_vector_load_ad__);
    uint32_t AppVectorSize = (uint32_t)&__ram_vector_table_length__;
#endif
    uint32_t i             = 0;

    if (SCB->VTOR != VTOR_RAM_ADDR)
    {
        RamVectorTableInit(VTOR_RAM_ADDR);
    }

    /* Update APP defined handlers */
    for (i = 0; i < AppVectorSize / 4; ++i)
    {
        if (i == 0 || i == 1) //skip __initial_sp and reset_handler remap
        {
            continue;
        }

        if (((pAppVector[i] != Default_Handler) && (pAppVector[i] != 0)) ||
            ((pAppVector[i] == Default_Handler) && (pRamVector[i] == (IRQ_Fun)ROM_Default_Handler)))
        {
            if (i < System_VECTORn)
            {
                OS_PRINT_WARN1("Warning! %s is updated by APP!", TRACE_STRING(SysException[i]));
            }
            else
            {
                OS_PRINT_WARN1("Warning! ISR %s is updated by APP!",
                               TRACE_STRING(ExtIrq[i - System_VECTORn]));
            }

            pRamVector[i] = pAppVector[i];
        }
    }

    __DMB();
    __DSB();
}
/**
 * @brief  GPIO Group3 Handler
 * @param  none
 * @return none
  */
DATA_RAM_FUNCTION
void GPIO_Group3_Handler(void)
{
    uint32_t GPIOIrqStatus = GPIO->INTSTATUS;

    //Check exact IRQ function
    if (GPIOIrqStatus & BIT3)
    {
        GPIO3_Handler();
    }
    if (GPIOIrqStatus & BIT7)
    {
        GPIO7_Handler();
    }
    if (GPIOIrqStatus & BIT11)
    {
        GPIO11_Handler();
    }
    if (GPIOIrqStatus & BIT15)
    {
        GPIO15_Handler();
    }
    if (GPIOIrqStatus & BIT19)
    {
        GPIO19_Handler();
    }
    if (GPIOIrqStatus & BIT23)
    {
        GPIO23_Handler();
    }
    if (GPIOIrqStatus & BIT27)
    {
        GPIO27_Handler();
    }
    if (GPIOIrqStatus & BIT31)
    {
        GPIO31_Handler();
    }
}
/**
 * @brief  GPIO Group2 Handler
 * @param  none
 * @return none
  */
DATA_RAM_FUNCTION
void GPIO_Group2_Handler(void)
{
    uint32_t GPIOIrqStatus = GPIO->INTSTATUS;

    //Check exact IRQ function
    if (GPIOIrqStatus & BIT2)
    {
        GPIO2_Handler();
    }
    if (GPIOIrqStatus & BIT6)
    {
        GPIO6_Handler();
    }
    if (GPIOIrqStatus & BIT10)
    {
        GPIO10_Handler();
    }
    if (GPIOIrqStatus & BIT14)
    {
        GPIO14_Handler();
    }
    if (GPIOIrqStatus & BIT18)
    {
        GPIO18_Handler();
    }
    if (GPIOIrqStatus & BIT22)
    {
        GPIO22_Handler();
    }
    if (GPIOIrqStatus & BIT26)
    {
        GPIO26_Handler();
    }
    if (GPIOIrqStatus & BIT30)
    {
        GPIO30_Handler();
    }
}
/**
 * @brief  GPIO Group1 Handler
 * @param  none
 * @return none
  */
DATA_RAM_FUNCTION
void GPIO_Group1_Handler(void)
{
    uint32_t GPIOIrqStatus = GPIO->INTSTATUS;

    //Check exact IRQ function
    if (GPIOIrqStatus & BIT1)
    {
        GPIO1_Handler();
    }
    if (GPIOIrqStatus & BIT9)
    {
        GPIO9_Handler();
    }
    if (GPIOIrqStatus & BIT13)
    {
        GPIO13_Handler();
    }
    if (GPIOIrqStatus & BIT17)
    {
        GPIO17_Handler();
    }
    if (GPIOIrqStatus & BIT21)
    {
        GPIO21_Handler();
    }
    if (GPIOIrqStatus & BIT25)
    {
        GPIO25_Handler();
    }
    if (GPIOIrqStatus & BIT29)
    {
        GPIO29_Handler();
    }
}
/**
 * @brief  GPIO Group0 Handler
 * @param  none
 * @return none
  */
DATA_RAM_FUNCTION
void GPIO_Group0_Handler(void)
{
    uint32_t GPIOIrqStatus = GPIO->INTSTATUS;

    //Check exact IRQ function
    if (GPIOIrqStatus & BIT0)
    {
        GPIO0_Handler();
    }
    if (GPIOIrqStatus & BIT8)
    {
        GPIO8_Handler();
    }
    if (GPIOIrqStatus & BIT12)
    {
        GPIO12_Handler();
    }
    if (GPIOIrqStatus & BIT16)
    {
        GPIO16_Handler();
    }
    if (GPIOIrqStatus & BIT20)
    {
        GPIO20_Handler();
    }
    if (GPIOIrqStatus & BIT24)
    {
        GPIO24_Handler();
    }
    if (GPIOIrqStatus & BIT28)
    {
        GPIO28_Handler();
    }
}

#if (RETARGET_PRINTF_METHOD == 0)
int __2printf(const char *fmt, ...)
{
    return 0;
}
#endif

void *malloc(size_t size)
{
    return os_mem_alloc(RAM_TYPE_DATA_ON, size);
}

void *calloc(size_t n, size_t size)
{
    return os_mem_zalloc(RAM_TYPE_DATA_ON, n * size);
}

/*only for FreeRTOS*/
void *realloc(void *ptr, size_t size)
{
    void *new_ptr;
    BlockLink_t *pxLink;
    size_t old_size, new_size = 0;

    if (ptr == NULL)
    {
        return os_mem_alloc(RAM_TYPE_DATA_ON, size);
    }

    if (size == 0)
    {
        os_mem_free(ptr);
        ptr = NULL;
        return NULL;
    }

    pxLink = (BlockLink_t *)((uint8_t *)ptr - sizeof(BlockLink_t));
    old_size = pxLink->xBlockSize;

    if ((old_size - sizeof(BlockLink_t)) ==
        (size + (FreeRTOS_portBYTE_ALIGNMENT - (size & (FreeRTOS_portBYTE_ALIGNMENT - 1)))))
    {
        return ptr;
    }

    os_sched_suspend();
    os_mem_free(ptr);
    new_ptr = os_mem_alloc(RAM_TYPE_DATA_ON, size);
    if (ptr != new_ptr)
    {
        pxLink = (BlockLink_t *)((uint8_t *)new_ptr - sizeof(BlockLink_t));
        new_size = pxLink->xBlockSize;
        memcpy(new_ptr, ptr, (old_size < new_size) ? (old_size - sizeof(BlockLink_t)) : (new_size - sizeof(
                    BlockLink_t)));
    }
    os_sched_resume();

    return new_ptr;
}

void free(void *ptr)
{
    os_mem_free(ptr);
}

#if (ENABLE_FULL_FEATURED_DIRECT_LOG == 1)
/*for full featured direct log print*/
#define LOG_MESSAGE_SYNC_CODE       0x7E
#define MAX_LOG_MESSAGE_LEN         128
#define GET_TYPE(info)              (uint8_t)((info) >> 24)
#define GET_SUBTYPE(info)           (uint8_t)((info) >> 16)

extern uint8_t log_seq_num;
extern uint32_t log_timestamp_get(void);
extern void LogUartTxChar(const uint8_t ch);
void log_direct_app(uint32_t info, const char *fmt, ...)
{
    char     l_msg[MAX_LOG_MESSAGE_LEN];
    uint16_t log_length;
    int16_t  fmt_length;
    uint32_t timestamp;
    va_list  ap;
    uint16_t i;
    uint32_t s;

    /**
     * Byte: Description
     * 0:    Sync(0x7E)
     * 1:    Length
     * 2:    SeqNum
     * 3:    CheckSum
     * 4-5:  Timestamp
     * 6:    Type
     * 7:    SubType: SUBTYPE_DIRECT
     * 8:    Format String
     */

    if (!is_log_init || OTP->logDisable == 1)
    {
        return;
    }

    timestamp = log_timestamp_get();

    l_msg[0] = LOG_MESSAGE_SYNC_CODE;
    l_msg[4] = (uint8_t)timestamp;
    l_msg[5] = (uint8_t)(timestamp >> 8);
    l_msg[6] = GET_TYPE(info);
    l_msg[7] = GET_SUBTYPE(info);

    log_length = 8;

    va_start(ap, fmt);
#if 1
    fmt_length = vsnprintf(l_msg + 8, MAX_LOG_MESSAGE_LEN - 8, fmt, ap);
#else  //save ROM code space, 20200907 lory
    fmt_length = VSnprintf(l_msg + 8, MAX_LOG_MESSAGE_LEN - 8, fmt, ((const int *)&fmt) + 1);
#endif
    if (fmt_length < 0) /* error occurred */
    {
        fmt_length = 0;
    }
    else if (log_length + fmt_length > MAX_LOG_MESSAGE_LEN) /* truncated */
    {
        log_length = MAX_LOG_MESSAGE_LEN;

    }
    else
    {
        log_length += fmt_length;
    }
    va_end(ap);

    l_msg[1] = log_length;

    s = os_lock();
    l_msg[2] = log_seq_num++;
    os_unlock(s);

    l_msg[3] = l_msg[0] ^ l_msg[1] ^ l_msg[2];

#ifdef FOR_SIMULATION
    for (i = 8; i < log_length; ++ i)
    {
        *((volatile uint8_t *)(FAKE_UART_ADDRESS)) = l_msg[i];
    }
    *((volatile uint8_t *)(FAKE_UART_ADDRESS)) = '\n';
#else
    for (i = 0; i < log_length; ++i)
    {
        LogUartTxChar(l_msg[i]);
    }
#endif
}
#endif

/**
 * @brief  get cpu clock
 * @param  none
 * @return uint32, for example 40000000 is 40M, 20000000 is 20M.
  */
uint32_t get_cpu_clock(void)
{
    extern uint32_t SystemCpuClock;
    return SystemCpuClock;
}

/**
 * @brief  get ic type
 * @param  none
 * @return uint8_t, ic type:
 * #define RTL8762ESF              0x1D
 * #define RTL8762EGF              0x18
 * #define RTL8762ERF              0x28
 * #define RTL8762EJF              0x29
 * #define RTL8762EMF              0x39
 * #define RS625MF                 0x49
 * #define RTL8762EKF              0x5A
 * #define RTL8762EKO              0x6C
 */
uint8_t get_ic_type(void)
{
    return *(uint8_t *)0x00202cef;
}

void *active_timer_handle;
void set_active_timer_callback(void *timer_handle)
{
    lps_mode_resume();
    os_timer_stop(&active_timer_handle);
    os_timer_delete(&active_timer_handle);
    active_timer_handle = NULL;
}
void set_boot_active_time(uint32_t active_time_ms)
{
    if (active_timer_handle == NULL)
    {
        lps_mode_pause();
        bool retval = os_timer_create(&active_timer_handle, "SetActiveTimer",  0, \
                                      active_time_ms, false, set_active_timer_callback);
        if (retval)
        {
            os_timer_start(&active_timer_handle);
        }
    }
    else
    {
        os_timer_restart(&active_timer_handle, active_time_ms);
    }
}

#if defined ( __CC_ARM   )
#elif defined ( __GNUC__   )
#include <sys/stat.h>
#include <errno.h>
#undef errno
extern int errno;

int __attribute__((weak)) _close(int file)
{
    return -1;
}

int __attribute__((weak)) _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int __attribute__((weak)) _getpid(void)
{
    return 1;
}

int __attribute__((weak)) _isatty(int file)
{
    return 1;
}

int __attribute__((weak)) _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

int __attribute__((weak)) _lseek(int file, int ptr, int dir)
{
    return 0;
}

int __attribute__((weak)) _read(int file, char *ptr, int len)
{
    return 0;
}

int __attribute__((weak)) _write(int file, char *ptr, int len)
{
    return len;
}

DATA_RAM_FUNCTION
void *memset(void *s, int c, size_t n)
{
    extern void *__aeabi_memset(void *s, size_t n, int c);
    return  __aeabi_memset(s, n, c);
}
#endif

#if (LOG_UART_MULTIPLEXING == 1)

APP_FLASH_TEXT_SECTION
PingpongBuffer *get_tx_buffer_app_cb(void)
{
    return pMCU_PPB_user;
}

APP_FLASH_TEXT_SECTION
T_UART_FLAG *get_uart_flag_app_cb(void)
{
    return uart_flag_p;
}

APP_FLASH_TEXT_SECTION
void get_tx_rx_pin_app_cb(uint8_t *p_tx, uint8_t *p_rx)
{
    *p_tx = UART_TX_PIN;
    *p_rx = UART_RX_PIN;
}

void data_print_buffer(const uint8_t *source, uint16_t size)
{
    PPB_Write(pMCU_PPB_user, source, size);
}

#endif

/** @} */ /* End of group SYSTEM_INIT_Exported_Functions */
/** @} */ /* End of group SYSTEM_INIT */

