/**
*********************************************************************************************************
*               Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      rtl876x_hw_sha256.h
* @brief
* @details
* @author    eason li
* @date      2018-09-04
* @version   v0.1
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef __RTL876X_HW_SHA256_H
#define __RTL876X_HW_SHA256_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdbool.h>
#include "rtl876x.h"

/** @defgroup  HW_SHA256 Hardware SHA256
    * @brief APIs for Hardware SHA256
    * @{
    */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup HW_SHA256_Exported_Types Hardware SHA256 Exported Types
  * @brief
  * @{
  */
typedef struct
{
    uint32_t total[2];          /*!< The number of Bytes processed.  */
    uint32_t state[8];          /*!< The intermediate digest state.  */
    uint8_t buffer[64];         /*!< The data block being processed. */
    int is224;                  /*!< unused, just align mbedtls structure */
} HW_SHA256_CTX;

typedef enum
{
    HW_SHA256_CPU_MODE,
    HW_SHA256_DMA_MODE
} HW_SHA256_ACCESS_MODE;


/** End of HW_SHA256_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup  HW_SHA256_Exported_Functions Hardware SHA256 Exported Functions
    * @brief
    * @{
    */
/**
  * @brief   Wrapped interface to generate sha256 data.
  *
  * @param[in]  input input data
  * @param[in]  byte_len data length
  * @param[in]  result generated sha256 data
  * @param[in]  mode cpu mode/dma mode
  *
  * @return status of generating sha256 data
  * @retval true    successful
  * @retval false   fail
  *
  */
extern bool (*hw_sha256)(uint8_t *input, uint32_t byte_len, uint32_t *result, int mode);

/**
  * @brief  Enable the hw_sha256 module
  */
void hw_sha256_init(void);

/**
  * @brief  Initialize and configure the hw_sha256 module
  *
  * @param[in]  ctx sha256 configuration
  * @param[in]  iv initial vector
  *
  */
extern void (*hw_sha256_start)(HW_SHA256_CTX *ctx, uint32_t *iv);

/**
  * @brief   Use the hw_sha256 module to generate sha256 data.
  *
  * @param[in]  ctx sha256 configuration
  * @param[in]  input input data
  * @param[in]  byte_len data length
  *
  * @return status of generating sha256 data
  * @retval true    successful
  * @retval false   fail
  *
  */
extern bool (*hw_sha256_cpu_update)(HW_SHA256_CTX *ctx, uint8_t *input, uint32_t byte_len);

/**
  * @brief  Process finishing operation of the hw_sha256 module
  *
  * @param[in]  ctx sha256 configuration
  * @param[out]  result calculation result
  *
  * @return status of processing finishing operation
  * @retval true    successful
  * @retval false   fail
  */
extern bool (*hw_sha256_finish)(HW_SHA256_CTX *ctx, uint32_t *result);

/** End of HW_SHA256_Exported_Functions
  * @}
  */

/** End of HW_SHA256
  * @}
  */
#ifdef __cplusplus
}
#endif
#endif /*__RTL876X_HW_SHA256_H*/
