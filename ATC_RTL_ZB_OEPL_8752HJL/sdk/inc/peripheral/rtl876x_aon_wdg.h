/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_aon_wdg.h
* @brief     header file of aon watch dog driver.
* @details
* @author    Serval Li
* @date      2024-01-18
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _RTL876X_AON_WDG_H_
#define _RTL876X_AON_WDG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup AON_WATCH_DOG AON_WATCH_DOG
  * @brief AON Watch Dog driver module
  * @{
  */

/** @defgroup AON_WATCH_DOG_Exported_Functions AON WATCH DOG Exported Functions
  * @{
  */

/**
 * \brief  Configure initialization parameters.
 * \param[in]  reset_level: Specify the reset level.
 *     This parameter can be 0 or 1.
 *     \arg 0: Reset whole chip except partial AON and RTC.
 *     \arg 1: Reset whole chip.
 * \param[in]  comp: Set period in ms.
 * \param[in]  cnt_ctl: Specify whether to continue counting in low power mode.
 *     This parameter can be 0 or 1.
 *     \arg 0: Stop count in low power mode.
 *     \arg 1: Continue count in low power mode.
 * \param[in]  cnt_reload: Specify whether to reload counter when exiting low power mode.
 *     This parameter can be 0 or 1.
 *     \arg 0: Not reload counter when exiting low power mode.
 *     \arg 1: Reload counter when exiting low power mode.
 * \return None.
 */
void AON_WDG_Config(uint8_t reset_level, uint32_t comp, uint8_t cnt_ctl, uint8_t cnt_reload);

/**
 * \brief  Configure Reset Level.
 * \param[in]  reset_level: Specify the reset level.
 *     This parameter can be 0 or 1.
 *     \arg 0: Reset whole chip except partial AON and RTC.
 *     \arg 1: Reset whole chip.
 * \return None.
 */
void AON_WDG_ConfigResetLevel(uint8_t reset_level);

/**
 * \brief  Configure comp period of the AON Watch Dog.
 * \param[in]  comp: Set period in ms.
 * \return None.
 */
void AON_WDG_ConfigComp(uint32_t comp);

/**
 * \brief  Configure whether to continue counting in low power mode or not.
 * \param[in]  cnt_ctl: Specify whether to continue counting in low power mode.
 *     This parameter can be 0 or 1.
 *     \arg 0: Stop count in low power mode.
 *     \arg 1: Continue count in low power mode.
 * \return None.
 */
void AON_WDG_ConfigCntCtl(uint8_t cnt_ctl);

/**
 * \brief  Configure whether to reload the counter when exiting low power mode or not.
 * \param[in]  cnt_reload: Specify whether to reload counter when exiting low power mode.
 *     This parameter can be 0 or 1.
 *     \arg 0: Not reload counter when exiting low power mode.
 *     \arg 1: Reload counter when exiting low power mode.
 * \return None.
 */
void AON_WDG_ConfigCntReload(uint8_t cnt_reload);

/**
 * \brief  Enable the AON Watch Dog.
 * \return None.
 */
void AON_WDG_Enable(void);

/**
 * \brief  Disable the AON Watch Dog.
 * \return None.
 */
void AON_WDG_Disable(void);

/**
 * \brief  Restart the AON Watch Dog.
 * \return None.
 */
void AON_WDG_Restart(void);

/**
 * \brief  Reset the system of the AON Watch Dog.
 * \return None.
 */
void AON_WDG_SystemReset(void);

/**
 * \brief  AON Watch Dog initialization.
 * \param[in]  reset_level: Specify the reset level.
 *     This parameter can be 0 or 1.
 *     \arg 0: Reset whole chip except partial AON and RTC.
 *     \arg 1: Reset whole chip.
 * \param[in]  timeout_second: Specify the timeout period in seconds, the maximum value is 65s
 * \return None.
 */
static inline void aon_wdg_init(uint8_t reset_level, uint8_t timeout_second)
{
    uint32_t comp = (timeout_second * 1000) & 0x3FFFF;  //only 18 bit
    AON_WDG_Config(reset_level, comp, 1, 1);
}

/**
 * \brief  Enable the AON Watch Dog.
 * \return None.
 * \note   For APP use.
 */
static inline void aon_wdg_enable(void)
{
    AON_WDG_Enable();
}

/**
 * \brief  Restart and disable the AON Watch Dog.
 * \return None.
 * \note   For APP use.
 */
static inline void aon_wdg_disable(void)
{
    AON_WDG_Restart();
    AON_WDG_Disable();
}


/**
  * \}
  */

/**
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_AON_WDG_H_ */
