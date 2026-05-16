#ifndef _CSIS_DEF_H_
#define _CSIS_DEF_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#define SET_MEMBER_LOCK_EXIST          0x01
#define SET_MEMBER_SIZE_EXIST          0x02
#define SET_MEMBER_RANK_EXIST          0x04
#define SET_MEMBER_SIRK_NOTIFY_SUPPORT 0x10
#define SET_MEMBER_SIZE_NOTIFY_SUPPORT 0x20

#define ATT_ERR_CSIS_LOCK_DENIED              0x80
#define ATT_ERR_CSIS_LOCK_RELEASE_NOT_ALLOWED 0x81
#define ATT_ERR_CSIS_INVALID_LOCK_VALUE       0x82
#define ATT_ERR_CSIS_OOB_SIRK_ONLY            0x83
#define ATT_ERR_CSIS_LOCK_ALREADY_GRANTED     0x84

//coordinated set identification service
#define GATT_UUID_CSIS                        0x1846

#define CSIS_UUID_CHAR_SET_IRK                0x2B84
#define CSIS_UUID_CHAR_SIZE                   0x2B85
#define CSIS_UUID_CHAR_LOCK                   0x2B86
#define CSIS_UUID_CHAR_RANK                   0x2B87

#define CSI_SIRK_LEN           16

#define CSI_LOCK_DEFAULT_TIMEOUT 60000
#define CSIP_DISCOVERY_TIMEOUT   10000
typedef enum
{
    CSIS_NONE_LOCK  = 0,
    CSIS_UNLOCKED   = 0x01,
    CSIS_LOCKED     = 0x02,
} T_CSIS_LOCK;

typedef enum
{
    CSIS_SIRK_ENC = 0x00,
    CSIS_SIRK_PLN = 0x01,
} T_CSIS_SIRK_TYPE;

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
