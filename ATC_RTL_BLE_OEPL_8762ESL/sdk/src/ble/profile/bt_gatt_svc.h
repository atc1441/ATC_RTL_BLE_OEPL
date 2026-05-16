#ifndef _BT_GATT_SVC_H_
#define _BT_GATT_SVC_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "profile_server.h"
#include "gatt.h"

/** @defgroup BT_GATT_SVC Bluetooth GATT Service
  * @brief Bluetooth GATT Service
  * @{
  */

/*============================================================================*
 *                              Types
 *============================================================================*/
/** @defgroup BT_GATT_SERVICE_Exported_Types Bluetooth GATT Service Exported Types
  * @{
  */
/**
 * @brief Bluetooth GATT service characteristic UUID
 */
typedef struct
{
    uint16_t index;
    uint8_t  uuid_size;
    union
    {
        uint16_t char_uuid16;
        uint8_t char_uuid128[16];
    } uu;
} T_CHAR_UUID;

/** End of BT_GATT_SERVICE_Exported_Types
  * @}
  */

/*============================================================================*
 *                              Functions
 *============================================================================*/
/** @defgroup BT_GATT_SERVICE_Exported_Functions Bluetooth GATT Service Exported Functions
  * @{
  */

/**
 * @brief    Find service characteristic uuid by attribute index.
 * @param[in]  p_srv     Pointer to service table: @ref T_ATTRIB_APPL.
 * @param[in]  index     Attribute index of characteristic.
 * @param[in]  attr_num  Total attribute number of service.
 * @return   The characteristic uuid.  @ref T_CHAR_UUID.
 */
T_CHAR_UUID gatt_svc_find_char_uuid_by_index(const T_ATTRIB_APPL *p_srv, uint16_t index,
                                             uint16_t attr_num);

/**
 * @brief    Find service characteristic attribute index by uuid.
 * @param[in]  p_srv      Pointer to service table. @ref T_ATTRIB_APPL.
 * @param[in]  char_uuid  Service characteristic uuid.
 * @param[in]  attr_num   Total attribute number of service.
 * @param[in,out]  index  Attribute index of characteristic.
 * @return   The result of finding service characteristic attribute index.
 * @retval   true  Success.
 * @retval   false Failed.
 */
uint16_t gatt_svc_find_char_index_by_uuid16(const T_ATTRIB_APPL *p_srv, uint16_t char_uuid16,
                                            uint16_t attr_num);

/** End of BT_GATT_SERVICE_Exported_Functions
  * @}
  */
/** End of BT_GATT_SVC
  * @}
  */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif
