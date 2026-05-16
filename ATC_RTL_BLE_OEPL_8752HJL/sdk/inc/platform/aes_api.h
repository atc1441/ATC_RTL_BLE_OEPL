/**
*****************************************************************************************
*     Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    aes_api.h
  * @brief   AES APIs abstracted for encryption related.
  * @details AES encryption APIs which delivers HW/SW implemented reliable and safe AES solution.
  * @author  Abel
  * @date    2017.5.17
  * @version v1.0
  */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _AES_API_H_
#define _AES_API_H_

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>


/** @defgroup AES_API AES
  * @brief API sets for AES encryption implementation
  * @{
  */

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup AES_API_Exported_Functions AES Exported Functions
    * @{
    */

/**
    * @brief  128 bit AES ECB encryption on specified plaintext and keys
    *
    * @param[in] plaintext    specified plain text to be encrypted
    * @param[in] key          keys to encrypt the plaintext
    * @param[out] encrypted    output buffer to store encrypted data
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    */
bool aes128_ecb_encrypt(uint8_t plaintext[16], const uint8_t key[16], uint8_t *encrypted);

/**
    * @brief  128 bit AES ECB decryption on specified encrypted data and keys
    *
    * @param[in] input    specified encrypted data to be decrypted
    * @param[in] key          keys to decrypt the data
    * @param[out] output    output buffer to store plain data
    *
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
*/
bool aes128_ecb_decrypt(uint8_t *input, const uint8_t *key, uint8_t *output);

/**
    * @brief  128 bit AES ECB encryption on specified plaintext and keys
    *
    * @param[in]  plaintext    specified plain text to be encrypted
    * @param[in]  key          keys to encrypt the plaintext
    * @param[out]  encrypted    output buffer to store encrypted data
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
    */
bool aes128_ecb_encrypt_msb2lsb(uint8_t plaintext[16], const uint8_t key[16],
                                uint8_t *encrypted);
/**
    * @brief  128 bit AES ECB decryption on specified encrypted data and keys
    *
    * @param[in]  input    specified encrypted data to be decrypted
    * @param[in] key          keys to decrypt the data
    * @param[out]  output    output buffer to store plain data
    *
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
*/
bool aes128_ecb_decrypt_msb2lsb(uint8_t *input, const uint8_t *key, uint8_t *output);

/**
    * @brief  256 bit AES ECB encryption on specified plaintext and keys
    *
    * @param[in]  plaintext    specified plain text to be encrypted
    * @param[in]  key          keys to encrypt the plaintext
    * @param[out]  encrypted    output buffer to store encrypted data
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
    */
bool aes256_ecb_encrypt(uint8_t plaintext[16], const uint8_t key[32], uint8_t *encrypted);
/**
    * @brief  256 bit AES ECB decryption on specified encrypted data and keys
    *
    * @param[in]  input    specified encrypted data to be decrypted
    * @param[in]  key          keys to decrypt the data
    * @param[out]  output    output buffer to store plain data
    *
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   least significant octet of encrypted data corresponds to encrypted[0]
*/
bool aes256_ecb_decrypt(uint8_t *input, const uint8_t *key, uint8_t *output);
/**
    * @brief  256 bit AES ECB encryption on specified plaintext and keys
    *
    * @param[in] plaintext    specified plain text to be encrypted
    * @param[in]  key          keys to encrypt the plaintext
    * @param[out]  encrypted    output buffer to store encrypted data
    *
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
    */
bool aes256_ecb_encrypt_msb2lsb(uint8_t plaintext[16], const uint8_t key[32],
                                uint8_t *encrypted);
/**
    * @brief  256 bit AES ECB decryption on specified encrypted data and keys
    *
    * @param[in]  input    specified encrypted data to be decrypted
    * @param[in]  key          keys to decrypt the data
    * @param[out]  output    output buffer to store plain data
    *
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    *
    * @note   most significant octet of encrypted data corresponds to encrypted[0]
    */
bool aes256_ecb_decrypt_msb2lsb(uint8_t *input, const uint8_t *key, uint8_t *output);

/** @} */ /* End of group AES_API_Exported_Functions */

/** @} */ /* End of group AES_API */


#endif
