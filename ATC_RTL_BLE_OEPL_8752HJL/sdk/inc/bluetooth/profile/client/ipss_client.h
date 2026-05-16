/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     ipss_client.h
  * @brief    Source file for client of internet protocol support service.
  * @details  Global data and function implementation.
  * @author   Jeff_Zheng
  * @date     2017-12-05
  * @version  v1.0
  * *************************************************************************************
  */


#ifndef _IPSS_CLIENT_H_
#define _IPSS_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C" {
#endif

/** @defgroup IPSS_CLIENT Internet Protocol Support Service Client
  * @brief IPSS client
  * @details

    IPSS client is used to discover Internet Protocol Support Service, this file can be used if an application needs to find IPSS.

    Applications shall register the IPSS client during initialization through the @ref ipss_add_client function.

    Applications can start discovering the IPSS service through the @ref ipss_find_srv_dcl function.

  * @{
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup IPSS_CLIENT_Exported_Functions IPSS Client Exported Functions
  * @brief
  * @{
  */


/**
  * @brief Add IPS service client.
  *
  * @param[in] p_func  Pointer of APP callback function to handle specific client module data.
  * @return Client ID.
  */
uint8_t ipss_add_client(void *p_func);

/**
  * @brief Discover IPS service by service UUID.
  *
  * @param[in] conn_id  Connection ID.
  * @return Operation result.
  * @retval true Operation success.
  * @retval false Operation failure.
  */
bool ipss_find_srv_dcl(uint8_t conn_id);

/** @} End of IPSS_CLIENT_Exported_Functions */

/** @} End of IPSS_CLIENT */

#ifdef  __cplusplus
}
#endif

#endif

