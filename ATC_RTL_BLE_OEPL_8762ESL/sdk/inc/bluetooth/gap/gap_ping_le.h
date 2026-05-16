/**
*********************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      gap_ping_le.h
* @brief    Head file for GAP PING
* @details
* @author
* @date      2020-06-18
* @version   v0.8
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef GAP_PING_LE_H
#define GAP_PING_LE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "upperstack_config.h"
#include "gap_le.h"
#include "gap_le_types.h"


/** @addtogroup GAP GAP Module
  * @{
  */

/** @addtogroup GAP_LE GAP LE Module
  * @{
  */

/** @addtogroup GAP_LE_PING GAP LE PING Module
  * @{
  */


/*============================================================================*
 *                         Macros
 *============================================================================*/
/** @defgroup GAP_LE_PING_Exported_Macros GAP LE AoX Exported Macros
  * @{
  */

/** @defgroup GAP_LE_PING_MSG_Types APP LE RF Msg Types
  * @{
  */
#define GAP_MSG_LE_PING_INFO                  0xD0
/** End of GAP_LE_PING_MSG_Types
  * @}
  */

/** @defgroup GAP_LE_PING_MSG_Opcodes APP LE PING Msg Opcodes
 * @{
 */
#define GAP_LE_PING_WRITE_AUTHEN_PAYLOAD_TIMEOUT                0x0000 //!<Response msg opcode for p_le_write_authen_payload_timeout_rsp
/**
  * @}
  */

/** End of GAP_LE_PING_Exported_Macros
  * @}
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_PING_Exported_Types GAP LE PING Exported Types
  * @brief
  * @{
  */
#if F_BT_LE_4_1_WRITE_AUTHEN_PAYLOAD_TIMEOUT_SUPPORT
/** @brief  Response for writing authenticated payload timeout.*/
typedef struct
{
    uint8_t         conn_id;
    uint16_t        cause;
} T_LE_WRITE_AUTHEN_PAYLOAD_TIMEOUT_RSP;
#endif

/** @brief  GAP LE PING Callback Data*/
typedef union
{
#if F_BT_LE_4_1_WRITE_AUTHEN_PAYLOAD_TIMEOUT_SUPPORT
    T_LE_WRITE_AUTHEN_PAYLOAD_TIMEOUT_RSP    *p_le_write_authen_payload_timeout_rsp;
#endif
} T_GAP_LE_PING_CB_DATA;

typedef struct
{
    uint16_t                      opcode;
    T_GAP_LE_PING_CB_DATA         data;
} T_GAP_LE_PING_CB;
/** End of GAP_LE_PING_Exported_Types
  * @}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup GAP_LE_PING_Exported_Functions GAP LE PING Exported Functions
  * @brief
  * @{
  */

#if F_BT_LE_4_1_WRITE_AUTHEN_PAYLOAD_TIMEOUT_SUPPORT
/**
 * @brief   Writes the authenticated payload timeout parameter for the connection identified by the conn_id parameter.
 *          Operation result will be returned by @ref app_gap_callback with cb_type @ref GAP_MSG_LE_WRITE_AUTHEN_PAYLOAD_TIMEOUT.
 *
 * @param[in] conn_id Connection ID
 * @param[in] authen_payload_timeout Maximum amount of time specified between packets authenticated by a valid MIC.
 *                                   Range: 0x0001 to 0xFFFF
 *                                   Time = N * 10 ms
 *                                   Time Range: 10 ms to 655350 ms
 * @return  Read request result.
 * @retval  GAP_CAUSE_SUCCESS: Send request success.
 * @retval  GAP_CAUSE_SEND_REQ_FAILED: Send request sent fail.
 * @retval  GAP_CAUSE_NON_CONN: Failed. No connection
 *
 * <b>Example usage</b>
 * \code{.c}
   void test()
   {
       le_write_authen_payload_timeout(conn_id, authen_payload_timeout);
   }

   T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
   {
       T_APP_RESULT result = APP_RESULT_SUCCESS;
       T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

            switch (cb_type)
      {
            ...
            case GAP_MSG_LE_PING_INFO:
                    {
                            APP_PRINT_INFO0("GAP_MSG_LE_PING_INFO");
                            T_GAP_LE_PING_CB *p_le_ping_cb;
                            p_le_ping_cb  = (T_GAP_LE_PING_CB *)p_data->p_le_cb_data;

                            switch (p_le_ping_cb->opcode)
                            {
                            case GAP_LE_PING_WRITE_AUTHEN_PAYLOAD_TIMEOUT:
                                    APP_PRINT_INFO2("GAP_LE_PING_WRITE_AUTHEN_PAYLOAD_TIMEOUT: conn_id %d, cause 0x%x",
                                                                    p_le_ping_cb->data.p_le_write_authen_payload_timeout_rsp->conn_id,
                                                                    p_le_ping_cb->data.p_le_write_authen_payload_timeout_rsp->cause);
                                    break;

                            default:
                                    APP_PRINT_ERROR1("app_gap_ping_callback: unhandled cb_type 0x%x", cb_type);
                                    break;
                            }

                            break;
                    }
       ...
            }
   }
 * \endcode
 */
T_GAP_CAUSE le_write_authen_payload_timeout(uint8_t conn_id, uint16_t authen_payload_timeout);
#endif

/** End of GAP_LE_PING_Exported_Functions
  * @}
  */

/** End of GAP_LE_PING
  * @}
  */

/** End of GAP_LE
  * @}
  */

/** End of GAP
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* GAP_PING_LE_H */
