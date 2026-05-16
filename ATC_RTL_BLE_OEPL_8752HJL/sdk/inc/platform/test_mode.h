/**
****************************************************************************************************
*               Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
****************************************************************************************************
* @file     test_mode.h
* @brief    test mode definition.
* @author   Serval Li
* @date     2017-08-29
* @version  v1.0
****************************************************************************************************
*/

#ifndef _TEST_MODE_H_
#define _TEST_MODE_H_

#include <stdint.h>
#include <stdbool.h>
#include "rtl876x.h"
#include "rtl876x_wdg.h"
#include "patch_header_check.h"
#include "flash_nor_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup TEST_MODE    Test Mode
  * @{
  */

/*============================================================================*
  *                                   Macros
  *============================================================================*/
/** @defgroup TEST_MODE_Exported_Macros Test Mode Exported Macros
  * @brief
  * @{
  */
/* ascii value of "test" */
#define TEST_MODE_FLAG_DEFAULT                        0x74657374
#define TEST_MODE_FLAG_DISABLE                        0x50245150

/* General Purpose FW register */
#define BTAON_FAST_TEST_MODE                          0x16
/** End of TEST_MODE_Exported_Macros
  * @}
  */

/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup TEST_MODE_Exported_Types Test Mode Exported Types
  * @brief
  * @{
  */

/* 0x16 (BTAON_FAST_TEST_MODE) */
typedef union
{
    uint16_t d16;
    struct
    {
        uint8_t rsvd               : 5;            /*!< bit[4:0]: reserved for ROM code */
        uint8_t test_mode          : 3;            /*!< bit[7:5]: test mode */
        uint8_t is_datatrans_patch : 1;            /*!< bit[8]: is_datatrans_patch */
        uint8_t rsvd1              : 7;            /*!< bit[15:9]: reserved */
    } s;
} T_BTAON_FAST_TEST_MODE_TYPE;

typedef enum
{
    NOT_TEST_MODE       = 0,
    DIRECT_TEST_MODE    = 1,
    SINGLE_TONE_MODE    = 2,
    AUTO_PAIR_WITH_FIX_ADDR_MODE    = 3,
    DATA_UART_TEST_MODE = 4,
    //add more
} T_TEST_MODE;
/** End of TEST_MODE_Exported_Types
  * @}
  */

/*============================================================================*
  *                                Functions
  *============================================================================*/
/** @defgroup TEST_MODE_Exported_Functions Test Mode Exported Functions
    * @brief
    * @{
    */
/**
 * @brief  Get test mode.
 *
 * @return The test mode.
 *
 */
static inline T_TEST_MODE get_test_mode(void)
{
    T_BTAON_FAST_TEST_MODE_TYPE aon;
    aon.d16 = btaon_fast_read_safe(BTAON_FAST_TEST_MODE);
    return (T_TEST_MODE)(aon.s.test_mode);
}

/**
 * @brief  Switch to certain test mode.
 *
 * @param[in]  test_mode test mode to switch
 *
 */

static inline void switch_to_test_mode(T_TEST_MODE test_mode)
{
    T_BTAON_FAST_TEST_MODE_TYPE aon;
    aon.d16 = btaon_fast_read_safe(BTAON_FAST_TEST_MODE);
    aon.s.test_mode = test_mode;
    btaon_fast_write_safe(BTAON_FAST_TEST_MODE, aon.d16);

    WDG_SystemReset(RESET_ALL_EXCEPT_AON, SWITCH_TEST_MODE);
}

/**
 * @brief  Reset the test mode.
 *
 */
static inline void reset_test_mode(void)
{
    T_BTAON_FAST_TEST_MODE_TYPE aon;
    aon.d16 = btaon_fast_read_safe(BTAON_FAST_TEST_MODE);
    aon.s.test_mode = NOT_TEST_MODE;
    btaon_fast_write_safe(BTAON_FAST_TEST_MODE, aon.d16);
}

/**
 * @brief  Check if test mode is enabled or not.
 *
 * @return status of test mode
 * @retval true    enabled
 * @retval false   disabled
 *
 */
static inline bool is_test_mode_enable(void)
{
    uint32_t start_addr = flash_nor_get_bank_addr(NOR_FLASH_BKP_DATA1);
    if (start_addr)
    {
        uint32_t r_data = 0;
        flash_nor_auto_read_locked(start_addr, &r_data);
        return r_data == TEST_MODE_FLAG_DEFAULT;
    }
    return false;
}

/**
 * @brief  Disable the test mode.
 *
 * @return status of disabling the test mode
 * @retval true    successful
 * @retval false   fail
 *
 */
static inline bool test_mode_disable(void)
{
    bool retval = false;
    uint32_t start_addr = flash_nor_get_bank_addr(NOR_FLASH_BKP_DATA1);
    if (start_addr)
    {
        uint32_t test_mode_flag_disable = TEST_MODE_FLAG_DISABLE;
        if (flash_nor_write_locked(start_addr, (uint8_t *)&test_mode_flag_disable,
                                   4) == FLASH_NOR_RET_SUCCESS)
        {
            retval = true;
        }
    }
    return retval;
}

/**
 * @brief  Modify the hci mode flag.
 *
 * @param[in]  enable Set or clear the hci mode flag.
 * \arg \c true    Set the hci mode flag.
 * \arg \c false    Clear the hci mode flag.
 *
 */
void set_hci_mode_flag(bool enable);

/**
 * @brief  Check the hci mode flag.
 *
 * @return value of hci mode flag
 * @retval true    hci mode flag is true.
 * @retval false   hci mode flag is false.
 *
 */
bool check_hci_mode_flag(void);

/**
 * @brief  Switch to hci mode.
 *
 */
static inline void switch_to_hci_mode(void)
{
    set_hci_mode_flag(true);
    WDG_SystemReset(RESET_ALL_EXCEPT_AON, SWITCH_HCI_MODE);
}

#if (BUILD_DATATRANS == 1)
/**
 * @brief  Modify the datatrans patch flag.
 *
 * @param[in]  enable Set or clear the datatrans patch flag.
 * \arg \c true    Set the datatrans patch flag.
 * \arg \c false    Clear the datatrans patch flag.
 *
 */
void set_datatrans_patch_flag(bool enable);

/**
 * @brief  Check the datatrans patch flag.
 *
 * @return value of datatrans patch flag
 * @retval true    datatrans patch flag is true.
 * @retval false   datatrans patch flag is false.
 *
 */
bool check_datatrans_patch_flag(void);
#endif

/** @} */ /* End of group TEST_MODE_Exported_Functions*/

/** @} */ /* End of group TEST_MODE*/
#ifdef __cplusplus
}
#endif

#endif /* _TEST_MODE_H_ */

