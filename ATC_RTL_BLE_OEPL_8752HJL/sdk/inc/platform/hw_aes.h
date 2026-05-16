/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    hw_aes.h
  * @brief   HW AES APIs abstracted for encryption-related purposes.
  * @details AES encryption APIs which deliver HW implemented reliable and safe AES solutions.
  * @author  Lory Xu
  * @date    2017.5.17
  * @version v1.0
  * *************************************************************************************
  */
/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef HW_AES_H
#define HW_AES_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>

/** @defgroup  HW_AES    Hardware AES
    * @brief API sets for hardware AES engine
    * @{
    */


/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup HW_AES_Exported_Types HW AES Exported Types
  * @{
  */

/** @brief Encryption zone for image encryption feature. */
typedef enum
{
    ZONE0,
    ZONE1,
    ZONE_NOT_USED
} T_PROTECT_ZONE;

/** @brief AES mode definition for HW AES. */
typedef enum
{
    AES_MODE_NONE,
    AES_MODE_CBC,
    AES_MODE_ECB,
    AES_MODE_CFB,
    AES_MODE_OFB,
    AES_MODE_CTR
} T_HW_AES_MODE;

/** End of HW_AES_Exported_Types
  * @}
  */

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup HW_AES_Exported_Functions HW AES Exported Functions
    * @brief
    * @{
    */

/**
    * @brief  16 Bytes AES decryption on specified data
    *
    * @param[in]  input              specified encrypted data to be decrypted
    * @param[out]  output            output buffer to store plain data
    *
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    *
    */
extern bool hw_aes_decrypt_16byte(uint8_t *input, uint8_t *output);

/**
    * @brief  128 bit AES encryption via DMA on specified plain data and keys
    *
    * @param[in]  p_in              specified plain data to be encrypted
    * @param[out]  p_out            output buffer to store encrypted data
    * @param[in]  data_word_len     input buffer length
    * @param[in]  key               key buffer
    * @param[in]  iv                initialization vector for AES CBC mode
    * @param[in]  mode              AES mode specified by @ref T_HW_AES_MODE
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   HWAES_DMA_RX_CH_NUM and HWAES_DMA_TX_CH_NUM DMA channels are occupied by this function
    */
bool hw_aes_encrypt128_use_dma(uint32_t *p_in, uint32_t *p_out, uint16_t data_word_len,
                               uint32_t *key, uint32_t *iv, T_HW_AES_MODE mode);
/**
    * @brief  128 bit AES decryption via DMA on specified data and keys
    * @param[in]  p_in              specified encrypted data to be decrypted
    * @param[out]  p_out            output buffer to store plain data
    * @param[in]  data_word_len     input buffer length
    * @param[in]  key               key buffer
    * @param[in]  iv                initialization vector for AES CBC mode
    * @param[in]  mode              AES mode specified by @ref T_HW_AES_MODE
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   HWAES_DMA_RX_CH_NUM and HWAES_DMA_TX_CH_NUM DMA channels are occupied by this function
    */
bool hw_aes_decrypt128_use_dma(uint32_t *p_in, uint32_t *p_out, uint16_t data_word_len,
                               uint32_t *key, uint32_t *iv, T_HW_AES_MODE mode);
/**
    * @brief  256 bit AES encryption via DMA on specified plain data and keys.
    *
    * @param[in]  p_in              specified plain data to be encrypted
    * @param[out]  p_out            output buffer to store encrypted data
    * @param[in]  data_word_len     input buffer length
    * @param[in]  key               key buffer
    * @param[in]  iv                initialization vector for AES CBC mode
    * @param[in]  mode              AES mode specified by @ref T_HW_AES_MODE
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   HWAES_DMA_RX_CH_NUM and HWAES_DMA_TX_CH_NUM DMA channels are occupied by this function
    */
bool hw_aes_encrypt256_use_dma(uint32_t *p_in, uint32_t *p_out, uint16_t data_word_len,
                               uint32_t *key, uint32_t *iv, T_HW_AES_MODE mode);
/**
    * @brief  256 bit AES decryption via DMA on specified data and keys.
    *
    * @param[in]  p_in              specified encrypted data to be decrypted
    * @param[out]  p_out            output buffer to store plain data
    * @param[in]  data_word_len     input buffer length
    * @param[in]  key               key buffer
    * @param[in]  iv                initialization vector for AES CBC mode
    * @param[in]  mode              AES mode specified by @ref T_HW_AES_MODE
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   HWAES_DMA_RX_CH_NUM and HWAES_DMA_TX_CH_NUM DMA channels are occupied by this function
    */
bool hw_aes_decrypt256_use_dma(uint32_t *p_in, uint32_t *p_out, uint16_t data_word_len,
                               uint32_t *key, uint32_t *iv, T_HW_AES_MODE mode);

/**
 * @brief swap src byte order to dst
 *
 * @param[in] src: input buffer
 * @param[out] dst: output buffer
 * @param[in] len: byte length that want to convert
 */
void swap_buf(const uint8_t *src, uint8_t *dst, uint32_t len);

/** @} */ /* End of group HW_AES_Exported_Functions */
/** @} */ /* End of group HW_AES */
#ifdef __cplusplus
}
#endif

#endif
