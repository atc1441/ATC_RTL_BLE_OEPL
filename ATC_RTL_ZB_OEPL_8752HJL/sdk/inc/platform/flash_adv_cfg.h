/**
****************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file      flash_adv_cfg.h
* @brief
* @note      flash advanced functions
* @author    Grace
* @date      2018-04-19
* @version   v0.1
* **************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _FLASH_ADV_CFG_H_
#define _FLASH_ADV_CFG_H_

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>
#include "flash_device.h"

#ifdef  __cplusplus
extern  "C" {
#endif  // __cplusplus

/** @defgroup  FLASH_DEVICE    Flash Device
    * @{
    */

/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Types Flash Device Exported Types
    * @brief
    * @{
    */

/** End of FLASH_DEVICE_Exported_Types
  * @}
  */
/*************************************************************************************************
  *                                   Functions
*************************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Functions Flash Device Exported Functions
    * @brief
    * @{
    */

/**
* @brief    get block protect level
* @param[in] bp_lv  a set of BPx ~ BP0
* @return   result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_get_block_protect_locked(uint8_t *bp_lv);

/**
* @brief    set block protect by map
* @param[in]    bp_lv a set of BPx ~ BP0
* @return   result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_set_block_protect_locked(uint8_t bp_lv);

/**
* @brief    only unlock preferred section by address
* @param[in]    unlock_addr address section to be unlocked
* @param[in]    old_bp_lv before unlock
* @return   result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_sw_protect_unlock_by_addr_locked(uint32_t unlock_addr, uint8_t *old_bp_lv);

/**
* @brief    Read vendor_id to switch callback
* @return   Result
* @retval   true if successful
* @retval   false if not supported
*/
bool flash_otp_init(void);

/**
* @brief    Safe version of flash_otp_erase
* @param[in]    type    Erase type
* @param[in]    addr    Address to erase when erasing block or sector
* @return   Result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_otp_erase_locked(T_ERASE_TYPE type, uint32_t addr);

/**
* @brief    Safe version of flash_otp_write
* @param[in]    start_addr    Start_addr address where it is going to be flashed in flash
* @param[in]    data_len      Data length to be programmed
* @param[out]    data          Data buffer to be programmed into
* @return   Result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_otp_write_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
* @brief    Safe version of flash_otp_read
* @param[in]    start_addr    Start_addr address where it is going to be read in flash
* @param[in]    data_len      Data length to be read
* @param[out]    data          Data buffer to be read into
* @return   Result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_otp_read_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
* @brief    Safe version of flash_otp_enter
* @return   Result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_otp_enter_locked(void);

/**
* @brief    Safe version of flash_otp_exit
* @return   Result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_otp_exit_locked(void);

/**
* @brief    Safe version of flash_otp_set_lb
* @return   Result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_otp_set_lb_locked(void);

/**
* @brief    Safe version of flash_otp_get_lb
* @return   Result
* @retval   true if successful
* @retval   false if failed
*/
bool flash_otp_get_lb_locked(void);

/**
 * @brief Read SFDP data
 * @param[in] start_addr    Start_addr address in SFDP table
 * @param[in] data_len      Data length to be read
 * @param[out] data          Data buffer to be read into
 * @return   Result
 * @retval   true if successful
 * @retval   false if failed
 */
bool flash_sfdp_read_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
 * @brief The quick user mode read (use split read to improve read speed)
 * @param[in] start_addr    Start_addr address where it is going to be read in flash (4 byte aligned)
 * @param[in] data_len      Data length to be read
 * @param[out] data          Data buffer to be read into
 * @return   Result
 * @retval   true if successful
 * @retval   false if failed
 */
bool flash_read_quick_locked(uint32_t start_addr, uint32_t data_len, uint8_t *data);

/**
 * @brief  read flash ID
 * @param[in]  void
 * @return flash ID
*/
uint32_t flash_get_id(void);

/** @} */ /* End of group FLASH_DEVICE_Exported_Functions */


/** @} */ /* End of group FLASH_DEVICE */

#ifdef  __cplusplus
}
#endif // __cplusplus

#endif // _FLASH_ADV_CFG_H_
