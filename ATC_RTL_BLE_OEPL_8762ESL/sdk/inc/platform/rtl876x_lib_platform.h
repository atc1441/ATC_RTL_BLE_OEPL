/**
************************************************************************************************************
*               Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
************************************************************************************************************
* @file     rtl876x_lib_platform.h
* @brief
* @details
* @author
* @date
* @version
*************************************************************************************************************
*/

#ifndef _RTL876X_LIB_PLATFORM_H_
#define _RTL876X_LIB_PLATFORM_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mem_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    READ_SUCCESS          = 0,
    READ_NOT_FIND_CONFIG  = 1,
    READ_NOT_FIND_BDADDR  = 2,
    READ_NOT_FIND_XTAL_OFFSET = 3,
    READ_FAIL             = 4,
} T_READ_BDADDR_RESULT;

typedef enum
{
    EFUSE_UPDATE_SUCCESS = 0,
    EFUSE_UPDATE_READ_PROTECTED,
    EFUSE_UPDATE_WRITE_MAC_FAIL,
    EFUSE_UPDATE_SPACE_EXHAUSTED,
    EFUSE_UPDATE_READ_FAIL,
} T_EFUSE_UPDATE_RESULT;

typedef enum
{
    DSN = 0,
    HWREV,
    PSN,
    SKUID,
    IRID,
    PRO_PID
} T_FLASH_READ_PROVISION;

typedef enum
{
    BT_MAC = 0, //BD address
    BT_MAX_LE_LINK_NUM,
    BT_LE_MASTER_LINK_NUM,
    BT_LE_SLAVE_LINK_NUM,
} T_READ_PARAMETER;

/**
  * @brief  another log buffer API, the fmt string will occupy flash space
  */
void log_buffer_retarget(uint32_t info, const char *fmt, ...);

#define DBG_BUFFER_RETARGET(...)     do {\
        log_buffer_retarget(COMBINE_TRACE_INFO(TYPE_BEE2, SUBTYPE_DIRECT, 0, 0), __VA_ARGS__);\
    } while (0)

/*============================================================================*
 *                         Log Retarget Sample Usage
 *============================================================================*/
/**
 * @brief   retarget other library code log to bee2 buffer log print.
 * @note    If enable, increase code size because more log will occupy flash space
 *
* <b>Example usage</b>
 * \code{.c}

//enable retarget other library code log to bee2 buffer log print or not
#define ENABLE_RETARGET_LOG     1

#if ENABLE_RETARGET_LOG
#define LogDebug(...) DBG_BUFFER_RETARGET(__VA_ARGS__)
#else
#define LogDebug(...)
#endif

void test_log_retarget(void)
{
    char str[] = {'a','b','c', '\0'};
    float data1 = 3.14125;
    uint32_t data2 = 1024;
    LogDebug("test");
    LogDebug("string:%s", str);
    LogDebug("float:%f", data1);
    LogDebug("uint:%d", data2);
}
 * \endcode
 */
/*============================================================================*/

/**
  * @brief  peek max free block size of specified ram type heap
  */
size_t os_mem_max_free_block_peek(RAM_TYPE ram_type);


void show_sdk_lib_version(void);

/**
  * @brief  read bd addr set in config file
  * @param  p_bdaddr: buffer to save bd addr
  * @retval read result
  *     @arg 0: read success
  *     @arg 1: no config file in flash
  *     @arg 2: no bd setting found
  */
T_READ_BDADDR_RESULT read_config_bdaddr(uint8_t *p_bdaddr);

/**
  * @brief  read mp config parameter
  * @param  type: the item of config file
    * @param  p_data: the return data
  *     @arg true: read success
  *     @arg false: read fail
  */
T_READ_BDADDR_RESULT read_config_item(T_READ_PARAMETER type, uint8_t *p_data);

/**
  * @brief  read xtal offset set in config file
  * @param  p_xtal_offset: buffer to save xtal offset
  * @retval read result
  *     @arg 0: read success
  *     @arg 1: no config file in flash
  *     @arg 3: no xtal offset found
  */
T_READ_BDADDR_RESULT read_config_xtal_offset(uint8_t *p_xtal_offset);

/**
  * @brief  update bd address set in config file
  * @param  p_cfg_read_addr: the address of read config file
    * @param  p_cfg_write_addr: the address of write config file
    * @param  p_bdaddr: buffer to the updated bd address
  * @retval update result
  *     @arg true: update success
  *     @arg false: update fail
  */
bool update_bdaddr(uint8_t *p_cfg_read_addr, uint8_t *p_cfg_write_addr, uint8_t *p_bdaddr);

/**
  * @brief  update xtal offset set in config file
  * @param  p_cfg_read_addr: the address of read config file
    * @param  p_cfg_write_addr: the address of write config file
    * @param  xtal: the updated xtal offset
  * @retval update result
  *     @arg true: update success
  *     @arg false: update fail
  */
bool update_xtal_offset(uint8_t *p_cfg_read_addr, uint8_t *p_cfg_write_addr, uint8_t xtal);

/**
  * @brief Write MAC address to config, this is mainly used on production line.
  * @param[in] p_mac_addr         The buffer hold MAC address (48 bits).
  * @return Write MAC to config fail or success.
  *     @retval true              Write MAC to config success.
  *     @retval false             Write MAC to config fails or not write existed MAC.
  */
bool UpdateMAC(uint8_t *p_mac_addr);

/**
  * @brief  update tx power in config file
  * @param[in] tx_power index for power. tx_power = power*2
                       tx_power   Power
                       0x10      8 dBm
                       0x8       4 dBm
                       0x6       3 dBm
                       0x0       0 dBm
                       0xD8      -20 dBm
  * @return update tx power to config fail or success.
  *     @retval true             update tx power to config success.
  *     @retval false            update tx power to config fails or not write existed tx power.
  */
bool UpdateTxPower(uint8_t tx_power);

/**
  * @brief Write MAC address to eFuse, this is mainly used on production line.
  *        Because eFuse space limitation, only write MAC to eFuse once supported.
  * @param[in] p_mac_addr         The buffer hold MAC address (48 bits).
  * @return Write MAC to config fail or success.
  *     @retval EFUSE_UPDATE_SUCCESS         Write MAC to eFuse success.
  *     @retval EFUSE_UPDATE_READ_PROTECTED  Can not update eFuse while it is read protected.
  *     @retval EFUSE_UPDATE_WRITE_MAC_FAIL  Write MAC to eFuse fails.
  *     @retval EFUSE_UPDATE_SPACE_EXHAUSTED eFuse space is exhausted.
  */
T_EFUSE_UPDATE_RESULT update_mac_to_efuse(uint8_t *p_mac_addr);

/**
  * @brief Write 40M XTAL calibration data to config, this is mainly used on production line.
  * @param[in] xtal               The value of 40M XTAL calibration data
  * @return Write calibration data to config fail or success.
  *     @retval true              Success.
  *     @retval false             Fail.
  */
bool WriteXtalToConfig(uint8_t xtal);

/**
  * @brief Write 40M XTAL calibration data to Efuse, this is mainly used on production line.
  * @param[in] xtal               The value of 40M XTAL calibration data
  * @return Write calibration data to Efuse fail or success.
  *     @retval true              Success.
  *     @retval false             Fail.
  * @note The Efuse space is limited, please don't call this function more than 5 five times.
  */
bool WriteXtalToEfuse(uint8_t xtal);


/**
  * @brief allow customers to write provision data to specific flash region
  * @param data input data
  * @param region input region
  * @ret   true if success
  */
bool flash_provision_write(uint8_t *data, T_FLASH_READ_PROVISION region);

/**
  * @brief allow customers to read provision data from specific flash region
  * @param data output data
  * @param region output region
  * @ret   true if success
  */
bool flash_provision_read(uint8_t *data, T_FLASH_READ_PROVISION region);

/**
 * @brief  get 14 bytes EUID
 * @param  none
 * @return uint8_t *, the pointer to a copy of EUID.
  */
uint8_t *get_ic_euid(void);


#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_LIB_PLATFORM_H_ */
