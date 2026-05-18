/**
*****************************************************************************************
*     Copyright(c) 2023, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file flash_sec.h
  * @brief flash security encryption support
  * @details
  * @author zhiyuan_tang
  * @date
  * @version
  ***************************************************************************************
    * @attention
  * <h2><center>&copy; COPYRIGHT 2023 Realtek Semiconductor Corporation</center></h2>
  ***************************************************************************************
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/

#ifndef _FLASH_SEC_H_
#define _FLASH_SEC_H_

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup FLASH_SEC Flash Security
  * @brief API Sets for flash security support
  * @{
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup FLASH_SEC_Exported_Functions Flash Security Exported Functions
  * @brief
  * @{
  */

/**
  * @brief  Init a flash-sec-encrypted image.
  *
  * @param[in]  img_addr: image base address
  * @param[in]  region_index: the flash-sec region index (total 8 regions, index from 0 to 7)
  *
  * @return init result
  *     @retval true: init success
  *     @retval false: init fail
  */
bool flash_sec_init_for_image(uint32_t img_addr, uint8_t region_index);

/**
  * @brief  Lock the flash-sec key by index.
  *
  * @param[in]  key_index: the flash-sec key index (total 8 keys, index from 0 to 7)
  *
  */
void flash_sec_lock_key(uint8_t key_index);

/** End of FLASH_SEC_Exported_Functions
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* _FLASH_SEC_H_ */
