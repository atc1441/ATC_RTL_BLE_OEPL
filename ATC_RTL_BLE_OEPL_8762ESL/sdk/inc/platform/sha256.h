/*********************************************************************
* Filename:   sha256.h
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the corresponding SHA1 implementation.
*********************************************************************/

/*
 This code is released into the public domain free of any restrictions.
 The author requests acknowledgement if the code is used, but does not require it.
 This code is provided free of any liability and without any quality claims by the author.
 https://github.com/B-Con/crypto-algorithms
 */

#ifndef SHA256_H
#define SHA256_H

/*************************** HEADER FILES ***************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/** @defgroup  SHA256_API Sha256
    * @brief APIs for using SHA256
    * @{
    */

/*============================================================================*
 *                              Macros
 *============================================================================*/
/** @defgroup SHA256_API_Exported_Macros SHA256 APIs Exported Macros
    * @{
    */

#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

#define SHA256_USE_DYNAMIC_ALLOC 0

/** End of SHA256_API_Exported_Macros
    * @}
    */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup SHA256_API_Exported_Types SHA256 APIs Exported Types
    * @{
    */

typedef unsigned char BYTE;             // 8-bit byte
typedef unsigned int  WORD;             // 32-bit word, change to "long" for 16-bit machines

/**  @brief Context structure to store SHA256 algorithm intermediate information */
typedef struct
{
    BYTE data[64];
    WORD datalen;
    unsigned long long bitlen;
    WORD state[8];
} SHA256_CTX;

/** End of SHA256_API_Exported_Types
    * @}
    */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup SHA256_API_Exported_Functions SHA256 APIs Exported Functions
    * @brief
    * @{
    */

void SHA256_Init(SHA256_CTX *ctx);
void SHA256_Update(SHA256_CTX *ctx, const BYTE data[], size_t len);
void SHA256_Final(SHA256_CTX *ctx, BYTE hash[]);
void SHA256(const void *in, size_t len, uint8_t *result);
bool SHA256_Alloc(SHA256_CTX **ctx);
void SHA256_Free(SHA256_CTX *ctx);

/** End of SHA256_API_Exported_Functions
    * @}
    */


/** End of SHA256_API
    * @}
    */

#endif   // SHA256_H

