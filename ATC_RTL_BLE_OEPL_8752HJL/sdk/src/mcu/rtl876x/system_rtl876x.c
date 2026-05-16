/**
*****************************************************************************************
*     Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file    system_rtl876x.c
   * @brief   system init file
   * @author  lory xu
   * @date    2024-05-20
   * @version v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2024 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <locale.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "version.h"
#include "rtl876x.h"
#include "patch_header_check.h"
#include "app_section.h"
#include "rom_uuid.h"
#include "app_define.h"
#include "trace.h"
#include "core_cmFunc.h"
#include "mem_config.h"
#include "otp.h"
#include "platform_autoconf.h"
#include "rtl876x_wdg.h"
#include "overlay_mgr.h"
#include "flash_nor_device.h"
#include "os_sched.h"
#include "otp_config.h"
#include "test_mode.h"
#include "platform_utils.h"
#include "os_mem.h"
#include "dfu_flash.h"
#include "rtl876x_rcc.h"
#include "rtl876x_nvic.h"
#include "board.h"
#include "dlps.h"
#include "os_timer.h"
#include "vector_table.h"
#include "flash_sec.h"
#include "rtl876x_lib_platform.h"
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
#define SHARE_CACHE_RAM_0K          0x82F70000
#define SHARE_CACHE_RAM_8K          0x2F2D0002
#define SHARE_CACHE_RAM_16K         0xA2AA0003

#define SHUTDOWN_DATARAM_4KB        (4 * 1024)
#define SHUTDOWN_DATARAM_8KB        (8 * 1024)
#define SHUTDOWN_DATARAM_32KB       (32 * 1024)
#define SHUTDOWN_DATARAM_40KB       (40 * 1024)
#define SHUTDOWN_DATARAM_72KB       (72 * 1024)

#define VTOR_RAM_ADDR               0x00200000 //!< vector table address in RAM.
#define APP_FAKE_PAYLOAD_LEN        0x100

#if (OTA_TEST == 1)
#undef VERSION_MAJOR
#undef VERSION_MINOR
#undef VERSION_REVISION
#undef VERSION_BUILDNUM

#if(OTA_TEST_BANK_SWITCH == 0) //test not bank switch
#if(OTA_TEST_IMAGE_VERSION == 0)
#define VERSION_MAJOR            1
#define VERSION_MINOR            0
#define VERSION_REVISION         0
#define VERSION_BUILDNUM         0
#else
#define VERSION_MAJOR            2
#define VERSION_MINOR            0
#define VERSION_REVISION         0
#define VERSION_BUILDNUM         0
#endif

#else //test bank switch
#if(APP_BANK == 0)
#define VERSION_MAJOR            1
#define VERSION_MINOR            0
#define VERSION_REVISION         0
#define VERSION_BUILDNUM         0
#else
#define VERSION_MAJOR            2
#define VERSION_MINOR            0
#define VERSION_REVISION         0
#define VERSION_BUILDNUM         0
#endif
#endif //end (OTA_TEST_BANK_SWITCH == 0)

#endif //end (OTA_TEST == 1)
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
#if (FEATURE_FLASH_SEC==1)
extern unsigned int Load$$LR$$LOAD_FLASH$$Length;
#endif
extern char Image$$FLASH_START_ADDR$$RO$$Base[];
extern char Load$$FLASH_START_ADDR$$RO$$Base[];
extern char Load$$FLASH_START_ADDR$$RO$$Length[];
#elif defined (__GNUC__)
#if (FEATURE_FLASH_SEC==1)
extern uint32_t *__flash_sec_base_addr__;
extern uint32_t *__flash_sec_region_size__;
#endif
#endif

extern void Reset_Handler(void);
extern uint32_t upperstack_fake_data;
extern void upperstack_loader(void);

BOOL_WDG_CB user_wdg_cb __attribute__((weak)) = NULL;
USER_CALL_BACK app_pre_main_cb __attribute__((weak)) = NULL ;
USER_CALL_BACK os_patch __attribute__((weak)) = NULL ;
USER_CALL_BACK flash_sec_init_app_cb __attribute__((weak)) = NULL ;

bool SystemInit(void) APP_FLASH_TEXT_SECTION;
void AppUpdateVectorTable(void) OVERLAY_SECTION_BOOT_ONCE;
void pre_main(void) APP_FLASH_TEXT_SECTION;
void wdg_system_reset_app_cb(T_WDG_MODE wdg_mode,
                             T_SW_RESET_REASON reset_reason) APP_FLASH_TEXT_SECTION;

uint32_t random_seed_value;

#if defined (__CC_ARM)
#pragma push
#pragma diag_suppress 1296 /* disable warning 1296(extened constant initialiser used)*/
#endif

/**
* @brief: Application Image Header
* @note: None
*/
const T_IMG_HEADER_FORMAT img_header APP_FLASH_HEADER =
{
#if defined ( __ICCARM__ )
    .auth =
    {
        .image_mac = {0xFF},
    },
#else
    .auth =
    {
        .image_mac = {[0 ... 15] = 0xFF},
    },
#endif

    .ctrl_header =
    {
        .ic_type = IMG_IC_TYPE,
        .secure_version = 0,
        .ctrl_flag.flag_value.xip = 1,
        .ctrl_flag.flag_value.enc = 0,
        .ctrl_flag.flag_value.load_when_boot = 0,
        .ctrl_flag.flag_value.enc_key_select = 0,
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
    .uuid = DEFINE_symboltable_uuid,

#if defined ( __ICCARM__ )
    .load_dst = 0,
    .exe_base = (uint32_t)SystemInit,
#elif defined (__CC_ARM)
    .load_dst = (uint32_t)Load$$FLASH_START_ADDR$$RO$$Base,
    .exe_base = (uint32_t)Image$$FLASH_START_ADDR$$RO$$Base,
#else
    .load_dst = 0,
    .exe_base = (uint32_t)Reset_Handler,
#endif
    .load_len = 0,  //0 indicates all XIP

#if (BUILD_BANK == 0)
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
#if (FEATURE_FLASH_SEC==1)
    .flash_sec_cfg =
    {
        .ctrl_flag.enable = 1,
        .ctrl_flag.mode = 1,
        .ctrl_flag.key_select = ENC_KEY_OCEK_WITH_OEMCONST,
#if defined (__CC_ARM)
        .base_addr = (uint32_t) Load$$FLASH_START_ADDR$$RO$$Base,
        .region_size = (uint32_t) &Load$$LR$$LOAD_FLASH$$Length - 4096,
#elif defined (__GNUC__)
        .base_addr = (uint32_t) &__flash_sec_base_addr__,
        .region_size = (uint32_t) &__flash_sec_region_size__,
#endif
        .iv_high = {0x12, 0x34, 0x56, 0x78},
        .iv_low = {0x11, 0x22, 0x33, 0x44},
    },
#endif
};

#if defined (__CC_ARM)
#pragma pop
#endif


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

/* Flash-Sec Module supports storing up to 8 keys.
   For Security, these 8 keys need to be locked. */
void flash_sec_lock_all_keys(void) APP_FLASH_GAP_TEXT_SECTION;
void flash_sec_lock_all_keys(void)
{
    for (int key_index = 0; key_index < 8; key_index++)
    {
        flash_sec_lock_key(key_index);
    }
}

void common_main(void)
{
#if (SUPPORT_FTL_IN_APP == 1)
    extern void ftl_patch_point_init(void);
    ftl_patch_point_init();

    extern uint32_t ftl_init(uint32_t u32PageStartAddr, uint8_t pagenum);
    ftl_init(flash_nor_get_bank_addr(NOR_FLASH_FTL),
             flash_nor_get_bank_size(NOR_FLASH_FTL) / OTP->PageSize);
#else
    extern uint32_t ftl_init(uint32_t u32PageStartAddr, uint8_t pagenum);
    ftl_init(flash_nor_get_bank_addr(NOR_FLASH_FTL),
             flash_nor_get_bank_size(NOR_FLASH_FTL) / FLASH_NOR_SECTOR_SIZE); // OTP->PageSize);
#endif
    //add common system code here before enter user defined main function
#if (SUPPORT_NORMAL_OTA == 1)
    if (dfu_check_ota_mode_flag())
    {
        dfu_main();
        dfu_set_ota_mode_flag(false);

        os_sched_start();
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


#if (SYSTEM_TRACE_ENABLE == 1)
    extern void system_trace_init(void);
    system_trace_init();
#endif

    if (flash_sec_init_app_cb)
    {
        flash_sec_init_app_cb();
    }

    flash_sec_lock_all_keys();

#endif

    /*config bt stack parameters in rom*/
#ifdef BT_STACK_CONFIG_ENABLE
    bt_stack_config_init();
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

void ram_init(void) APP_FLASH_TEXT_SECTION;
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

#if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_32KB)
    //copy data on ro
    extern char Image$$LAST_8K_RAM_ON$$RO$$Base[];
    extern char Load$$LAST_8K_RAM_ON$$RO$$Base[];
    extern unsigned int Image$$LAST_8K_RAM_ON$$RO$$Length;

    memcpy(Image$$LAST_8K_RAM_ON$$RO$$Base,
           Load$$LAST_8K_RAM_ON$$RO$$Base,
           (unsigned int)&Image$$LAST_8K_RAM_ON$$RO$$Length);

    //copy data on rw
    extern char Image$$LAST_8K_RAM_ON$$RW$$Base[];
    extern char Load$$LAST_8K_RAM_ON$$RW$$Base[];
    extern unsigned int Image$$LAST_8K_RAM_ON$$RW$$Length;

    memcpy(Image$$LAST_8K_RAM_ON$$RW$$Base,
           Load$$LAST_8K_RAM_ON$$RW$$Base,
           (unsigned int)&Image$$LAST_8K_RAM_ON$$RW$$Length);

    //clear data on zi
    extern char Image$$LAST_8K_RAM_ON$$ZI$$Base[];
    extern unsigned int Image$$LAST_8K_RAM_ON$$ZI$$Length;

    memset(Image$$LAST_8K_RAM_ON$$ZI$$Base,
           0,
           (unsigned int)&Image$$LAST_8K_RAM_ON$$ZI$$Length);
#endif

#if (BUFFER_ON_APP_GLOBAL_SIZE != 0)
    //copy buffer on ro
    extern char Image$$RAM_BUFFER_ON$$RO$$Base[];
    extern char Load$$RAM_BUFFER_ON$$RO$$Base[];
    extern unsigned int Image$$RAM_BUFFER_ON$$RO$$Length;

    memcpy(Image$$RAM_BUFFER_ON$$RO$$Base,
           Load$$RAM_BUFFER_ON$$RO$$Base,
           (unsigned int)&Image$$RAM_BUFFER_ON$$RO$$Length);

    //copy buffer on rw
    extern char Image$$RAM_BUFFER_ON$$RW$$Base[];
    extern char Load$$RAM_BUFFER_ON$$RW$$Base[];
    extern unsigned int Image$$RAM_BUFFER_ON$$RW$$Length;

    memcpy(Image$$RAM_BUFFER_ON$$RW$$Base,
           Load$$RAM_BUFFER_ON$$RW$$Base,
           (unsigned int)&Image$$RAM_BUFFER_ON$$RW$$Length);

    //clear buffer on zi
    extern char Image$$RAM_BUFFER_ON$$ZI$$Base[];
    extern unsigned int Image$$RAM_BUFFER_ON$$ZI$$Length;

    memset(Image$$RAM_BUFFER_ON$$ZI$$Base,
           0,
           (unsigned int)&Image$$RAM_BUFFER_ON$$ZI$$Length);
#endif
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

void ram_cache_init(void) APP_FLASH_TEXT_SECTION;
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

APP_MAIN_FUNC get_image_entry_addr(uint16_t image_id) APP_FLASH_TEXT_SECTION;
APP_MAIN_FUNC get_image_entry_addr(uint16_t image_id)
{
    uint32_t active_ota_bank_addr = get_active_ota_bank_addr();
    if (check_header_valid(active_ota_bank_addr, OTA) != IMG_CHECK_PASS)
    {
        return NULL;
    }
    uint32_t image_size = *get_image_size_in_bank(active_ota_bank_addr, (T_IMG_ID)image_id);

    if (image_size == 0)
    {
        return NULL;
    }

    uint32_t header_addr = get_header_addr_by_img_id((T_IMG_ID)image_id);

    if (check_header_valid(header_addr, (T_IMG_ID)image_id) != IMG_CHECK_PASS)
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

bool SystemInit(void)
{
    //hci mode check and bypass app
    if (check_hci_mode_flag() || (OTP->stack_en == 0))
    {
        return true;
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

#ifdef BUFFER_RAM_HEAP_SIZE
    //define BUFFER_HEAP_SIZE in mem_config.h
    OTP->heapBufferONSize = BUFFER_RAM_HEAP_SIZE;
#endif

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
    else if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_32KB)
    {
        set_dataram_to_shutdown(MIDDLE_32K_DATARAM_SHUTDOWN);
    }
    else if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_40KB)
    {
        set_dataram_to_shutdown(LAST_40K_DATARAM_SHUTDOWN);
    }
    else if (SHUTDOWN_DATARAM_SIZE == SHUTDOWN_DATARAM_72KB)
    {
        set_dataram_to_shutdown(LAST_72K_DATARAM_SHUTDOWN);
    }
    else
    {
        DBG_DIRECT("Error! SHUTDOWN_DATARAM_SIZE is not match the size that can be set!");
    }
#endif

#ifdef OS_TICK_TIME
    set_os_clock(OS_TICK_TIME);
#endif

#ifdef SUPPORT_SINGLE_BANK_OTA_USER_DATA
    OTP->bkp_data1_addr = USER_DATA_START_ADDR;
    OTP->bkp_data1_size = USER_DATA_MAX_SIZE;
#endif

#if ((SYSTEM_TRACE_ENABLE == 1) && (TRACE_HEAP_EN == 1))
    //dumpMemoryUsage to 1, side effect is can't enter powerdown and will wake up from dlps per 60s.
    OTP->dumpMemoryUsage = 1;
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

#if 0
    //flash config
    /*config enable flash block proect depending on flash layout and flash id*/
#if (FLASH_BLOCK_PROTECT_ENABLE == 1)
    OTP->bp_enable = 1;
#else
    OTP->bp_enable = 0;
#endif
    OTP->delay_10us_after_toggle_cs = AFTER_TOGGLE_CS_DELAY;

    OTP->image_split_read = 0; //use flash auto mode to check image
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
    RCC_PeriphClockCmd(APBPeriph_UART1, APBPeriph_UART1_CLOCK, (FunctionalState)DISABLE);
#endif

    /*config log baudrate*/
#ifdef LOG_BAUD_RATE
    //define LOG_BAUD_RATE in otp_config.h
    OTP->logBaudRate = LOG_BAUD_RATE;
#endif

#ifdef LOG_RAM_TYPE
    //define LOG_RAM_TYPE in otp_config.h
    OTP->log_ram_type = LOG_RAM_TYPE;
#endif

#if (FTL_REAL_LOGIC_ADDR_SIZE > 0)
    if (FTL_REAL_LOGIC_ADDR_SIZE <= (((((FLASH_NOR_SECTOR_SIZE / 8) - 1) * ((
                                                                                OTP->ftl_size / FLASH_NOR_SECTOR_SIZE) -
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

    /*to fix bug need disable dump callstack info before WDG_SystemReset, default enable */
#if (DUMP_INFO_BEFORE_RESET_DISABLE == 1)
    OTP->dump_info_before_reset = 0;
#endif

    /*before wdg system reset, write reset reason to specific flash addr*/
#if (WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE > 0)
    OTP->write_info_to_flash_before_reset = WRITE_REASON_TO_FLASH_BEFORE_RESET_ENABLE;
    OTP->reboot_record_address = REBOOT_REASON_RECORD_ADDRESS;
    OTP->reboot_record_item_limit_power_2 = REBOOT_REASON_RECORD_LIMIT_POWERT2;
#endif

    /*config enable swd pinmux*/
#ifdef SWD_PINMUX_ENABLE
#if (SWD_PINMUX_ENABLE == 0)
    swd_pin_disable();
#endif
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

    /*use os tick as log timestamp instead of TIM7*/
    OTP->log_timestamp_src = LOG_TIMESTAMP_OS;

    /*config enable write hardfault record to flash*/
#if (WRITE_HARDFAULT_RECORD_TO_FLASH_ENABLE > 0)
    OTP->write_info_to_flash_when_hardfault = WRITE_HARDFAULT_RECORD_TO_FLASH_ENABLE;
    OTP->HardFault_Record_BegAddr = HARDFAULT_RECORD_BEG_ADDR;
    OTP->HardFault_Record_EndAddr = HARDFAULT_RECORD_END_ADDR;
    OTP->HardFault_Record_CFG = HARDFAULT_RECORD_CFG;
#endif

#if (ENABLE_FLASH_READ_TURN_OFF_RF > 0)
    OTP->read_turn_on_off_rf = ENABLE_FLASH_READ_TURN_OFF_RF;
#endif

#if (DISABLE_AES_OTA == 1)
    OTP->ota_with_encryption_data = 0;
#else
    OTP->ota_with_encryption_data = 1;
#endif

    OTP->bkp_data1_addr = USER_DATA1_ADDR;
    OTP->bkp_data1_size = USER_DATA1_SIZE;

    OTP->bkp_data2_addr = USER_DATA2_ADDR;
    OTP->bkp_data2_size = USER_DATA2_SIZE;

    //app config
    OTP->ota_timeout_total = OTA_TIMEOUT_TOTAL;
    OTP->ota_timeout_wait4_conn = OTA_TIMEOUT_WAIT4_CONN;
    OTP->ota_timeout_wait4_image_transfer = OTA_TIMEOUT_WAIT4_IMAGE_TRANS;
    OTP->ota_timeout_ctittv = OTA_TIMEOUT_CTITTV;

#if ROM_OTA_LINKLOSS_RST
    OTP->ota_link_loss_reset = 1;
#endif

//add more otp config here

    ram_init();

    if (os_patch)
    {
        os_patch();
    }

    return true;
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
        BOOT_PRINT_INFO0("[Debug Info]RESET Reason: HW");

        // In boot stage, reset_reason is set to 0x1 (RESET_REASON_WDG_TIMEOUT),
        // which is not accurate for hardware resets.
        // Therefore, we need to override it with RESET_REASON_HW.
        extern T_SW_RESET_REASON reset_reason;
        reset_reason = RESET_REASON_HW;

        uint32_t wdg_reg_backup = WDG->WDG_CTL;

        T_WDG_MODE reset_mode = (T_WDG_MODE)((wdg_reg_backup >> 29) & 0x3);

        // Disable wdg interrupt and Peripheral interrupt. Make sure not to enter the wdg isr, but just to trigger WDG reset.
        if (INTERRUPT_CPU == reset_mode)
        {
            PERIPHINT->EN &= ~BIT(WDT_IRQn - Peripheral_First_IRQn);
            NVIC_DisableIRQ(Peripheral_IRQn);
        }

        //trigger wdg reset interrupt to update wdg_reset_lr (register is Read-Only, can't write directly)
        WDG_ClockEnable();
        WDG_Config(1, 0, INTERRUPT_CPU);
        WDG_Enable();
        platform_delay_ms(1);    //add delay to make lr as a stable value (0x7bb5 for RTL8762E A-cut)

        WDG_Disable();
        platform_delay_us(150);  //wait for WDG is disabled

        // Clear pending bit. Enable wdg interrupt and Peripheral interrupt.
        if (INTERRUPT_CPU == reset_mode)
        {
            HAL_WRITE32(PERI_INT_REG_BASE, 0, PERIPHINT->STATUS);
            NVIC_ClearPendingIRQ(Peripheral_IRQn);
            NVIC_EnableIRQ(Peripheral_IRQn);
            PERIPHINT->EN |= BIT(WDT_IRQn - Peripheral_First_IRQn);
        }

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

    /* WDG interrupt init.*/
    if (OTP->wdgEnableInRom)
    {
        if (OTP->wdgMode == INTERRUPT_CPU)
        {
            NVIC_InitTypeDef nvic_init_struct;
            nvic_init_struct.NVIC_IRQChannel = WDT_IRQn;
            nvic_init_struct.NVIC_IRQChannelCmd = (FunctionalState)ENABLE;
            nvic_init_struct.NVIC_IRQChannelPriority = OTP->wdgIP;;
            NVIC_Init(&nvic_init_struct);
        }
    }
}

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
/**
 * @brief  update vector table in app
 * @param  none
 * @return none
  */
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
        "System", "TMETER", "BTMAC", "TIM3", "TIM2", "BTMAC_WRAP_AROUND", "I2S0_TX", "I2S0_RX", "Timer4-5",
        "ZIGBEE", "PF_RTC", "UART0", "UART1", "I8080", "SPI0", "SPI1", "I2C0", "I2C1", "ADC",
        "Peripheral", "GDMA0 Channel0", "GDMA0 Channel1", "GDMA0 Channel2", "GDMA0 Channel3",
        "Enhanced_Timer0", "Enhanced_Timer1", "GPIO_Group3", "GPIO_Group2", "IR", "GPIO_Group1",
        "GPIO_Group0", "UART2", "TIM4", "TIM5", "SPIC0", "Qdecode", "Keyscan", "SPI2W", "LPCOMP",
        "PTA_Mailbox", "SPIC1", "SHA256", "Platform", "TRNG", "FLASH_SEC", "RTC", "WDT"
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
void GPIO_Group3_Handler(void) DATA_RAM_FUNCTION;
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
void GPIO_Group2_Handler(void) DATA_RAM_FUNCTION;
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
void GPIO_Group1_Handler(void) DATA_RAM_FUNCTION;
void GPIO_Group1_Handler(void)
{
    uint32_t GPIOIrqStatus = GPIO->INTSTATUS;

    //Check exact IRQ function
    if (GPIOIrqStatus & BIT1)
    {
        GPIO1_Handler();
    }
    if (GPIOIrqStatus & BIT5)
    {
        GPIO5_Handler();
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
void GPIO_Group0_Handler(void) DATA_RAM_FUNCTION;
void GPIO_Group0_Handler(void)
{
    uint32_t GPIOIrqStatus = GPIO->INTSTATUS;

    //Check exact IRQ function
    if (GPIOIrqStatus & BIT0)
    {
        GPIO0_Handler();
    }
    if (GPIOIrqStatus & BIT4)
    {
        GPIO4_Handler();
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

void *malloc(size_t size)
{
    return os_mem_alloc(RAM_TYPE_DATA_ON, size);
}

void *calloc(size_t n, size_t size)
{
    return os_mem_zalloc(RAM_TYPE_DATA_ON, n * size);
}

void *realloc(void *ptr, size_t size)
{
    if (ptr)
    {
        os_mem_free(ptr);
    }

    return os_mem_alloc(RAM_TYPE_DATA_ON, size);
}

void free(void *ptr)
{
    os_mem_free(ptr);
}

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
 * #define RTL8752HRF              0x04
 * #define RTL8771HTV              0x03
 * #define RTL8752HGF              0x02
 * #define RTL8752HJL              0x39
 * #define RTL8752HFL              0x0B
 * #define RTL8752HMF              0x07
 * #define RTL8752HMF-VI           0x09
 * #define RTL8752HJF              0x05
 * #define R528JO                  0x06
 * #define RTL8752HZF              0x08
 * #define RTL8752HKF              0x0A
 */
uint8_t get_ic_type(void)
{
    return *(uint8_t *)0x0020114a;
}

/**
Implement the minimal functionality required to allow libc to link when using gcc build.
Referance: https://sourceware.org/newlib/libc.html#Stubs
*/
#if defined( __GNUC__ ) && !defined( __CC_ARM )
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

/* Improve GCC performance by using ROM memset */
DATA_RAM_FUNCTION
void *memset(void *s, int c, size_t n)
{
    extern void *__aeabi_memset(void *s, size_t n, int c);
    return  __aeabi_memset(s, n, c);
}
#endif

#if (ENABLE_FULL_FEATURED_DIRECT_LOG == 1)
#include "os_sync.h"
#define LOG_FORMAT_VERSION            0
#define LOG_MESSAGE_SYNC_CODE         0x7E
#define MAX_LOG_MESSAGE_LEN           252
#define MAX_ARGUMENT_SIZE             20

#define GET_TYPE(info)                  (uint8_t)((info) >> 24)
#define GET_SUBTYPE(info)               (uint8_t)((info) >> 16)
#define GET_MODULE(info)                (uint8_t)((info) >> 8)
#define GET_LEVEL(info)                 (uint8_t)((info) >> 0)
#define GET_MODULE_LOWSTACK(str_index)  (uint8_t)((str_index) >> 16)

#define DiagPutChar                 LogUartTxChar

extern uint8_t log_seq_num;
extern bool is_log_init;
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
     * 4:    version
     * 5:    reserved
     * 6:    type
     * 7:    subtype
     * 8-11:  Timestamp
     */

    if (!is_log_init  == 1 || OTP->logDisable == 1)
    {
        return;
    }

    timestamp = log_timestamp_get();

    l_msg[0] = LOG_MESSAGE_SYNC_CODE;
    l_msg[4] = (uint8_t)LOG_FORMAT_VERSION;
    l_msg[6] = GET_TYPE(info);
    l_msg[7] = GET_SUBTYPE(info);
    memcpy(&l_msg[8], &timestamp, 4);
    l_msg[12] = GET_MODULE(info);

    log_length = 12;

    va_start(ap, fmt);

    fmt_length = vsnprintf(l_msg + 12, MAX_LOG_MESSAGE_LEN - 12, fmt, ap);

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

    for (i = 0; i < log_length; ++i)
    {
        DiagPutChar(l_msg[i]);
    }
}
#endif

/** @} */ /* End of group SYSTEM_INIT_Exported_Functions */
/** @} */ /* End of group SYSTEM_INIT */

