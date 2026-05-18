/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file      rtl876x_wdg.h
* \brief     header file of watch dog driver.
* \details
* \author    Lory_xu
* \date      2024-01-22
* \version   v0.1
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _RTL876X_WDG_H_
#define _RTL876X_WDG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"
#include "rtl876x_bitfields.h"

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    WDT         WDT
 *
 * \brief       Manage the WDT peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup    WDT_Exported_Types WDT Exported Types
 * \{
 * \ingroup     WDT
 */

typedef enum
{
    INTERRUPT_CPU = 0,
    RESET_ALL_EXCEPT_AON = 1,
    RESET_ALL = 3
} T_WDG_MODE;

/** End of group WDT_Exported_Types
  * \}
  */

#define BTAON_FAST_RESET_REASON     0x5

/*============================================================================*
 *                         Constants
 *============================================================================*/
/**
 * \defgroup    WDT_Exported_Constants WDT Exported Constants
 *
 * \ingroup     WDT
 */

/**
 * \defgroup    WDT_Reset_Reason Watch Dog Reset Reason
 * \brief       wdg reset reason introduction:
 * 1. If you want to get reset reason from aon 0x15, dividing three types:
 *   a) HW reset: aon reg 0x15 is cleared to 0, magic pattern on ram will change
 *   b) SW RESET_ALL: aon reg 0x15 is cleared to 0, but magic pattern on ram not change
 *   c) SW RESET_ALL_EXCEPT_AON: obtain reset reason by reading aon reg 0x15.
 * 2. Attention: don't use 0x1 as your reset reason when using RESET_ALL_EXCEPT_AON type! Because 0x1 is default value.
 * \{
 * \ingroup     WDT_Exported_Constants
 */
typedef enum
{
    RESET_REASON_HW                 = 0x0,  /* HW reset */
    RESET_REASON_WDG_TIMEOUT        = 0x1,
    RESET_REASON_POWERDOWN          = 0xCF,
    SW_RESET_APP_START              = 0xD0,
    SWITCH_HCI_MODE                 = 0xD1,
    SWITCH_TEST_MODE                = 0xD2,
    DFU_SWITCH_TO_OTA_MODE          = 0xD3,
    DFU_ACTIVE_RESET                = 0xD4,
    DFU_FAIL_RESET                  = 0xD5,
    UPPER_CMD_RESET,
    SINGLE_TONE_TIMEOUT_RESET,
    UART_CMD_RESET,
    RESET_REASON_FACTORY_RESET,
    RESET_REASON_LPC_TRIGGER,
    SW_RESET_APP_END                = 0xFF,
} T_SW_RESET_REASON;
/** \} */

typedef void (*APP_CB_WDG_RESET_TYPE)(T_WDG_MODE wdg_mode, T_SW_RESET_REASON reset_reason);
typedef bool (*BOOL_WDG_CB)(T_WDG_MODE wdg_mode, T_SW_RESET_REASON reset_reason);

/**
 * \defgroup    WDT_Exported_Variables Watch Dog Exported Variables
 * \{
 * \ingroup     WDT_Exported_Constants
 */
extern APP_CB_WDG_RESET_TYPE app_cb_wdg_reset;
extern BOOL_WDG_CB user_wdg_cb;
/** \} */

/** End of group WDT_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * \defgroup    WDT_Exported_Functions WDT Exported Functions
 * \{
 * \ingroup     WDT
 */

/**
   * \brief  Watch Dog Clock Enable.
   * \return None.
   */
extern void WDG_ClockEnable(void);

/**
   * \brief  Watch Dog Timer Config.
   * \param[in]  div_factor: 16Bit: 32.768k/(1+divfactor).
   * \param[in]  cnt_limit: 2^(cnt_limit+1) - 1 ; max 11~15 = 0xFFF.
   * \param[in]  wdg_mode: Select Watch Dog mode.
   *             \arg  0: interrupt CPU
   *             \arg  1: reset all except aon
   *             \arg  2: reset core domain
   *             \arg  3: reset all
   * \return None.
   */
extern void WDG_Config(uint16_t div_factor, uint8_t cnt_limit, T_WDG_MODE wdg_mode);

/**
   * \brief  Watch Dog Timer Enable.
   * \return None.
   */
extern void WDG_Enable(void);

/**
   * \brief  Watch Dog Timer Disable.
   * \return None.
   */
extern void WDG_Disable(void);

/**
   * \brief  Watch Dog Timer Restart.
   * \return None.
   */
extern void WDG_Restart(void);

/**
   * \brief  Watch Dog System Reset.
   * \param[in]  wdg_mode: 0: interrupt CPU
   *                   1: reset all except aon
   *                   2: reset core domain
   *                   3: reset all
   * \param[in] reset_reason: This parameter can refer to \ref WDT_Reset_Reason.
   * \return None.
   */
extern void WDG_SystemReset(T_WDG_MODE wdg_mode, T_SW_RESET_REASON reset_reason);


/**
   * \brief  Get watch Dog reset reason.
   * \return Watch Dog reset reason.
   *         This parameter can refer to \ref WDT_Reset_Reason.
   */
extern T_SW_RESET_REASON  reset_reason_get(void);

/** End of WDT_Exported_Functions
  * \}
  */

/** End of WDT
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif //_RTL876X_WDG_H_
