/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_keyscan.h
* \brief    The header file of the peripheral KEYSCAN driver.
* \details  This file provides all KEYSCAN firmware functions.
* \author   tifnan_ge
* \date     2024-01-22
* \version  v1.0
* *********************************************************************************************************
*/

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _RTL876X_KEYSCAN_H_
#define _RTL876X_KEYSCAN_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    KEYSCAN     KEYSCAN
 *
 * \brief       Manage the KEYSCAN peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"


/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup    KEYSCAN_Exported_Types  KEYSCAN Exported Types
 *
 * \ingroup     KEYSCAN
 */

/**
 * \brief       KEYSCAN initialize parameters
 *
 * \ingroup     KEYSCAN_Exported_Types
 */
typedef struct
{
    uint16_t rowSize;           /*!< Specify Keyscan Row Size.
                                        This parameter can be a value <=12. */
    uint16_t colSize;           /*!< Specify Keyscan Column Size.
                                        This parameter can be a value <=20. */
    uint32_t detectPeriod;      /*!< <b>Deprecated</b> */
    uint16_t timeout;           /*!< <b>Deprecated</b> */
    uint16_t scanInterval;      /*!< Specify Keyscan scan interval. scan interval = delay clock * scanInterval*/
    uint32_t debounceEn;        /*!< Enable or disable debounce.
                                     This parameter can be a value of \ref KEYSCAN_Debounce_Config. */
    uint32_t scantimerEn;       /*!< Enable or disable scan timer.
                                     This parameter can be a value of \ref KEYSCAN_Scan_Interval_En. */
    uint32_t detecttimerEn;     /*!< Enable or disable detect timer.
                                     This parameter can be a value of \ref KEYSCAN_Release_Detect_Timer_En. */
    uint16_t debounceTime;      /*!< <b>Deprecated</b> */
    uint32_t detectMode;        /*!< Specify Key Detect mode.
                                     This parameter can be a value of \ref KEYSCAN_Press_Detect_Mode */
    uint32_t fifoOvrCtrl;       /*!< Specify Keyscan fifo over flow control.
                                     This parameter can be a value of \ref KEYSCAN_FIFO_Overflow_Control */
    uint16_t maxScanData;       /*!< <b>Deprecated</b> */
    uint32_t scanmode;          /*!< Specify Keyscan mode.
                                     This parameter can be a value of \ref KEYSCAN_Scan_Mode. */
    uint16_t clockdiv;          /*!< Specify Keyscan clock divider. scan clock = system clock/(clockdiv+1). */
    uint8_t delayclk;           /*!< Specify Keyscan delay clock divider. delay clock = scan clock/(delayclk+1)*/
    uint16_t fifotriggerlevel;  /*!< Specify Keyscan fifo threshold to trigger interrupt KEYSCAN_INT_THRESHOLD.*/
    uint8_t debouncecnt;        /*!< Specify Keyscan debounce time, debounce time = delay clock * debouncecnt. */
    uint8_t releasecnt;         /*!< Specify Keyscan release time, release time = delay clock * releasecnt. */
    uint8_t keylimit;           /*!< Specify max scan data allowable in each scan. 0 means no limit. */
    uint32_t manual_sel;        /*!< Specify trigger mode in manual mode.
                                     This parameter can be a value of \ref KEYSCAN_Manual_Mode. */
} KEYSCAN_InitTypeDef;

/** End of KEYSCAN_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/
/**
 * \defgroup    KEYSCAN_Exported_Constants  KEYSCAN Exported Constants
 *
 * \ingroup     KEYSCAN
 */

/**
 * \defgroup    KEYSCAN_Config KEYSCAN Config
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define IS_KeyScan_PERIPH(PERIPH) ((PERIPH) == KEYSCAN)
/** \} */

/**
 * \defgroup    KEYSCAN_Row_Number KEYSCAN Row Number
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define IS_KEYSCAN_ROW_NUM(ROW) ((ROW) <= 12)
/** \} */

/**
 * \defgroup    KEYSCAN_Column_Number KEYSCAN Column Number
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define IS_KEYSCAN_COL_NUM(COL) ((COL) <= 20)
/** \} */

/**
 * \defgroup    KEYSCAN_Debounce_Time KEYSCAN Debounce Time
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define IS_KEYSCAN_MAX_SCAN_DATA(DATA_NUM) ((DATA_NUM) <= 26) //0 means no limit
/** \} */

/**
 * \defgroup    KEYSCAN_Scan_Mode KEYSCAN Scan Mode
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define KeyScan_Manual_Scan_Mode        ((uint32_t)(0x0 << 30))
#define KeyScan_Auto_Scan_Mode          ((uint32_t)(0x1 << 30))

#define IS_KEYSCAN_SCAN_MODE(MODE)    (((MODE) == KeyScan_Detect_Mode_Edge) || ((MODE) == KeyScan_Detect_Mode_Level))
/** \} */

/**
 * \defgroup    KEYSCAN_Manual_Mode KEYSCAN Manual Mode
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define KeyScan_Manual_Sel_Bit          ((uint32_t)(0x0 << 11))
#define KeyScan_Manual_Sel_Key          ((uint32_t)(0x1 << 11))
/** \} */

/**
 * \defgroup    KEYSCAN_FIFO_Overflow_Control KEYSCAN FIFO Overflow Control
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */

#define KeyScan_FIFO_OVR_CTRL_DIS_ALL   ((uint32_t)(0x0 << 28))   //discard all the new scan data when FIFO is full
#define KeyScan_FIFO_OVR_CTRL_DIS_LAST  ((uint32_t)(0x1 << 28))   //discard the oldest scan data when FIFO is full

#define IS_KEYSCAN_FIFO_OVR_CTRL(CTRL)  (((CTRL) == KeyScan_FIFO_OVR_CTRL_DIS_ALL) || ((CTRL) == KeyScan_FIFO_OVR_CTRL_DIS_LAST))
/** \} */

/**
 * \defgroup    KEYSCAN_Debounce_Config KEYSCAN Debounce Config
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define KeyScan_Debounce_Enable              ((uint32_t)0x1 << 31)
#define KeyScan_Debounce_Disable             ((uint32_t)0x0 << 31)

#define IS_KEYSCAN_DEBOUNCE_EN(EN) (((EN) == KeyScan_Debounce_Enable) || ((EN) == KeyScan_Debounce_Disable))    //0 means no limit
/** \} */

/**
 * \defgroup    KEYSCAN_Scan_Interval_En   KEYSCAN Scan Interval Enable
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define KeyScan_ScanInterval_Enable              ((uint32_t)(0x1 << 30))
#define KeyScan_ScanInterval_Disable             ((uint32_t)(0x0 << 30))

#define IS_KEYSCAN_SCANINTERVAL_EN(EN) (((EN) == KeyScan_ScanInterval_Enable) || ((EN) == KeyScan_ScanInterval_Disable))    //0 means no limit
/** \} */

/**
 * \defgroup    KEYSCAN_Release_Detect_Timer_En     KEYSCAN Release Detect Timer Enable
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define KeyScan_Release_Detect_Enable              ((uint32_t)(0x1 << 29))
#define KeyScan_Release_Detect_Disable             ((uint32_t)(0x0 << 29))

#define IS_KEYSCAN_RELEASE_DETECT_EN(EN) (((EN) == KeyScan_Release_Detect_Enable) || ((EN) == KeyScan_Release_Detect_Disable))    //0 means no limit
/** \} */

/**
 * \defgroup    KEYSCAN_Press_Detect_Mode KEYSCAN Press Detect Mode
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */

#define KeyScan_Detect_Mode_Edge        ((uint32_t)(0x0 << 29))
#define KeyScan_Detect_Mode_Level       ((uint32_t)(0x1 << 29))

#define IS_KEYSCAN_DETECT_MODE(MODE)    (((MODE) == KeyScan_Detect_Mode_Edge) || ((MODE) == KeyScan_Detect_Mode_Level))
/** \} */

/**
 * \defgroup    KEYSCAN_Interrupt_Definition    KEYSCAN Interrupt Definition
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define KEYSCAN_INT_THRESHOLD                    ((uint16_t)(0x1 << 4))
#define KEYSCAN_INT_OVER_READ                    ((uint16_t)(0x1 << 3))
#define KEYSCAN_INT_SCAN_END                     ((uint16_t)(0x1 << 2))
#define KEYSCAN_INT_FIFO_NOT_EMPTY               ((uint16_t)(0x1 << 1))
#define KEYSCAN_INT_ALL_RELEASE                  ((uint16_t)(0x1 << 0))

#define IS_KEYSCAN_CONFIG_IT(IT) (((IT) == KEYSCAN_INT_ALL_RELEASE) || \
                                  ((IT) == KEYSCAN_INT_FIFO_NOT_EMPTY) || \
                                  ((IT) == KEYSCAN_INT_SCAN_END) || \
                                  ((IT) == KEYSCAN_INT_OVER_READ) || \
                                  ((IT) == KEYSCAN_INT_THRESHOLD))
/** \} */

/**
 * \defgroup    KEYSCAN_Flag_Definition   KEYSCAN Flag Definition
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define KEYSCAN_FLAG_FIFOLIMIT                       ((uint32_t)(0x1 << 20))
#define KEYSCAN_INT_FLAG_THRESHOLD                   ((uint32_t)(0x1 << 19))
#define KEYSCAN_INT_FLAG_OVER_READ                   ((uint32_t)(0x1 << 18))
#define KEYSCAN_INT_FLAG_SCAN_END                    ((uint32_t)(0x1 << 17))
#define KEYSCAN_INT_FLAG_FIFO_NOT_EMPTY              ((uint32_t)(0x1 << 16))
#define KEYSCAN_INT_FLAG_ALL_RELEASE                 ((uint32_t)(0x1 << 15))
#define KEYSCAN_FLAG_DATAFILTER                      ((uint32_t)(0x1 << 3))
#define KEYSCAN_FLAG_OVR                             ((uint32_t)(0x1 << 2))
#define KEYSCAN_FLAG_FULL                            ((uint32_t)(0x1 << 1))
#define KEYSCAN_FLAG_EMPTY                           ((uint32_t)(0x1 << 0))

#define IS_KEYSCAN_FLAG(FLAG)       ((((FLAG) & (~(uint32_t)0x1F800F)) == 0x00) && ((FLAG) != (uint32_t)0x00))
#define IS_KEYSCAN_CLEAR_FLAG(FLAG) (((FLAG) == KEYSCAN_FLAG_FIFOLIMIT) || \
                                     ((FLAG) == KEYSCAN_FLAG_DATAFILTER) || \
                                     ((FLAG) == KEYSCAN_FLAG_OVR))
/** \} */

/**
 * \defgroup    KEYSCAN_FIFO_Available_Mask KEYSCAN FIFO Available Mask
 * \{
 * \ingroup     KEYSCAN_Exported_Constants
 */
#define STATUS_FIFO_DATA_NUM_MASK           ((uint32_t)(0x3F << 4))
/** \} */

/** End of KEYSCAN_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * \defgroup    KEYSCAN_Exported_Functions KEYSCAN Exported Functions
 * \{
 * \ingroup     KEYSCAN
 */

/**
 * \brief  Deinitialize the Keyscan peripheral registers to their default reset values(turn off keyscan clock).
 * \param[in]  KeyScan: SSelected KeyScan peripheral.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_keyscan_init(void)
 * {
 *     KeyScan_DeInit(KEYSCAN);
 * }
 * \endcode
 */
void KeyScan_DeInit(KEYSCAN_TypeDef *KeyScan);

/**
 * \brief   Initialize the KeyScan peripheral according to the specified
 *          parameters in the KeyScan_InitStruct
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_InitStruct: Pointer to a KEYSCAN_InitTypeDef structure that
 *             contains the configuration information for the specified KeyScan peripheral
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_keyscan_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_KEYSCAN, APBPeriph_KEYSCAN_CLOCK, ENABLE);

 *     KEYSCAN_InitTypeDef KEYSCAN_InitStruct;
 *     KeyScan_StructInit(&KEYSCAN_InitStruct);

 *     KEYSCAN_InitStruct.rowSize  = 2;
 *     KEYSCAN_InitStruct.colSize  = 2;
 *     KEYSCAN_InitStruct.scanmode     = KeyScan_Manual_Scan_Mode;

 *     KeyScan_Init(KEYSCAN, &KEYSCAN_InitStruct);

 *     KeyScan_INTConfig(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_ClearINTPendingBit(KEYSCAN, KEYSCAN_INT_SCAN_END);
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, DISABLE);  // Unmask keyscan interrupt
 *     KeyScan_Cmd(KEYSCAN, ENABLE);
 * }
 * \endcode
 */
void KeyScan_Init(KEYSCAN_TypeDef *KeyScan, KEYSCAN_InitTypeDef *KeyScan_InitStruct);

/**
 * \brief  Fill each KeyScan_InitStruct member with its default value.
 * \param[in]  KeyScan_InitStruct: Pointer to a KEYSCAN_InitTypeDef structure which will be initialized.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_keyscan_init(void)
 * {
 *     RCC_PeriphClockCmd(APBPeriph_KEYSCAN, APBPeriph_KEYSCAN_CLOCK, ENABLE);

 *     KEYSCAN_InitTypeDef KEYSCAN_InitStruct;
 *     KeyScan_StructInit(&KEYSCAN_InitStruct);

 *     KEYSCAN_InitStruct.rowSize  = 2;
 *     KEYSCAN_InitStruct.colSize  = 2;
 *     KEYSCAN_InitStruct.scanmode     = KeyScan_Manual_Scan_Mode;

 *     KeyScan_Init(KEYSCAN, &KEYSCAN_InitStruct);

 *     KeyScan_INTConfig(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_ClearINTPendingBit(KEYSCAN, KEYSCAN_INT_SCAN_END);
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, DISABLE);  // Unmask keyscan interrupt
 *     KeyScan_Cmd(KEYSCAN, ENABLE);
 * }
 * \endcode
 */
void KeyScan_StructInit(KEYSCAN_InitTypeDef *KeyScan_InitStruct);

/**
 * \brief  Enable or disable the specified KeyScan interrupt.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_IT: Specify the KeyScan interrupt sources to be enabled or disabled.
 *      This parameter can be any combination of the following values, which refer to \ref KEYSCAN_Interrupt_Definition.
 *      \arg KEYSCAN_INT_THRESHOLD: The interrupt is triggered when FIFO data size equals the threshold level.
 *      \arg KEYSCAN_INT_OVER_READ: The interrupt is triggered when reading an empty FIFO to prevent over-reading.
 *      \arg KEYSCAN_INT_SCAN_END: The interrupt is triggered after the Scan key matrix is completed.
 *      \arg KEYSCAN_INT_FIFO_NOT_EMPTY: The interrupt is triggered when there is data in the FIFO,
 *                                       and it can be cleared automatically after the FIFO reads empty.
 *      \arg KEYSCAN_INT_ALL_RELEASE: The interrupt is triggered if no key is pressed
 *                                    before the release time counter equals the set value.
 * \param[in] newState: New state of the specified KeyScan interrupts.
 *      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_INTConfig(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, DISABLE);
 * }
 * \endcode
 */
void KeyScan_INTConfig(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT,
                       FunctionalState newState);

/**
 * \brief  Mask the specified KeyScan interrupt.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_IT: Specify the KeyScan interrupt sources to be enabled or disabled.
 *      This parameter can be any combination of the following values, which refer to \ref KEYSCAN_Interrupt_Definition.
 *      \arg KEYSCAN_INT_THRESHOLD: The interrupt is triggered when FIFO data size equals the threshold level.
 *      \arg KEYSCAN_INT_OVER_READ: The interrupt is triggered when reading an empty FIFO to prevent over-reading.
 *      \arg KEYSCAN_INT_SCAN_END: The interrupt is triggered after the Scan key matrix is completed.
 *      \arg KEYSCAN_INT_FIFO_NOT_EMPTY: The interrupt is triggered when there is data in the FIFO,
 *                                       and it can be cleared automatically after the FIFO reads empty.
 *      \arg KEYSCAN_INT_ALL_RELEASE: The interrupt is triggered if no key is pressed
 *                                    before the release time counter equals the set value.
 * \param[in] newState: New state of the specified KeyScan interrupts mask.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_INTConfig(KEYSCAN, KEYSCAN_INT_SCAN_END, ENABLE);
 *     KeyScan_INTMask(KEYSCAN, KEYSCAN_INT_SCAN_END, DISABLE);
 * }
 * \endcode
 */
void KeyScan_INTMask(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT,
                     FunctionalState newState);

/**
 * \brief  Read data from keyscan FIFO.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[out] outBuf: Buffer to save data read from KeyScan FIFO.
 * \param[in]  count: Data length to be read.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     uint16_t data[3] = {0};
 *     KeyScan_Read(KEYSCAN, data, 3);
 * }
 * \endcode
 */
void KeyScan_Read(KEYSCAN_TypeDef *KeyScan, uint16_t *outBuf, uint16_t count);

/**
 * \brief   Enable or disable the KeyScan peripheral.
 * \param[in] KeyScan: Selected KeyScan peripheral.
 * \param[in] NewState: New state of the KeyScan peripheral.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_Cmd(KEYSCAN, ENABLE);
 * }
 * \endcode
 */
void KeyScan_Cmd(KEYSCAN_TypeDef *KeyScan, FunctionalState NewState);

/**
 * \brief   Set filter data.
 * \param[in] KeyScan: Selected KeyScan peripheral.
 * \param[in] data: Configure the data to be filtered.
 *      This parameter should not be more than 9 bits.
 * \param[in] NewState: New state of the data filter.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_FilterDataConfig(KEYSCAN, 0x01, ENABLE);
 *
 * }
 * \endcode
 */
void KeyScan_FilterDataConfig(KEYSCAN_TypeDef *KeyScan, uint16_t data,
                              FunctionalState NewState);

/**
 * \brief   KeyScan debounce time config.
 * \param[in] KeyScan: Selected KeyScan peripheral.
 * \param[in] time: Keyscan hardware debounce time. debounce time = delay clock * time.
 * \param[in] NewState: New state of the KeyScan debounce function.
 *      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_debounceConfig(KEYSCAN, 10, ENABLE);
 *
 * }
 * \endcode
 */
__STATIC_INLINE void KeyScan_debounceConfig(KEYSCAN_TypeDef *KeyScan, uint8_t time,
                                            FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    KeyScan->TIMERCR &= ~((0xff << 18) | BIT31);
    KeyScan->TIMERCR |= ((NewState << 31) | time << 18);

}

/**
 * \brief   Get KeyScan FIFO data num.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \return  Data length in FIFO.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     uint16_t data_len = KeyScan_GetFifoDataNum(KEYSCAN);
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t KeyScan_GetFifoDataNum(KEYSCAN_TypeDef *KeyScan)
{
    assert_param(IS_KeyScan_PERIPH(KeyScan));

    return (uint16_t)((KeyScan->STATUS & STATUS_FIFO_DATA_NUM_MASK) >> 4);
}

/**
 * \brief  Clear the KeyScan interrupt pending bit.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_IT: Specify the interrupt pending bit to clear.
 *      This parameter can be any combination of the following values, which refer to \ref KEYSCAN_Interrupt_Definition.
 *      \arg KEYSCAN_INT_THRESHOLD: The interrupt is triggered when FIFO data size equals the threshold level.
 *      \arg KEYSCAN_INT_OVER_READ: The interrupt is triggered when reading an empty FIFO to prevent over-reading.
 *      \arg KEYSCAN_INT_SCAN_END: The interrupt is triggered after the Scan key matrix is completed.
 *      \arg KEYSCAN_INT_FIFO_NOT_EMPTY: The interrupt is triggered when there is data in the FIFO,
 *                                       and it can be cleared automatically after the FIFO reads empty.
 *      \arg KEYSCAN_INT_ALL_RELEASE: The interrupt is triggered if no key is pressed
 *                                    before the release time counter equals the set value.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_ClearINTPendingBit(KEYSCAN, KEYSCAN_INT_SCAN_END);
 * }
 * \endcode
 */
__STATIC_INLINE void KeyScan_ClearINTPendingBit(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_IT)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_KEYSCAN_CONFIG_IT(KeyScan_IT));

    KeyScan->INTCLR |= KeyScan_IT;

    return;
}

/**
 * \brief   Clear the specified KeyScan flag.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_FLAG: Specify the flag to clear.
 *      This parameter can be one of the following values, which refer to \ref KEYSCAN_Flag_Definition.
 *      \arg KEYSCAN_FLAG_FIFOLIMIT: Overflow FIFO limit in one scan
 *      \arg KEYSCAN_FLAG_DATAFILTER: FIFO filter status
 *      \arg KEYSCAN_FLAG_OVR: The flag to indicate FIFO is overflow
 * \return  None.
 * \note    KEYSCAN_FLAG_FULL and KEYSCAN_FLAG_EMPTY can't be cleared manually.
 *          They are cleared by hardware automatically.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     KeyScan_ClearFlags(KEYSCAN, KEYSCAN_FLAG_FIFOLIMIT);
 * }
 * \endcode
 */
__STATIC_INLINE void KeyScan_ClearFlags(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_FLAG)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_KEYSCAN_CLEAR_FLAG(KeyScan_FLAG));

    if (KeyScan_FLAG & KEYSCAN_FLAG_FIFOLIMIT)
    {
        KeyScan->INTCLR |= BIT8;
    }
    if (KeyScan_FLAG & KEYSCAN_FLAG_DATAFILTER)
    {
        KeyScan->INTCLR |= BIT7;
    }
    if (KeyScan_FLAG & KEYSCAN_FLAG_OVR)
    {
        KeyScan->INTCLR |= BIT5;
    }

    return;
}

/**
 * \brief   Check whether the specified KeyScan flag is set.
 * \param[in]  KeyScan: Selected KeyScan peripheral.
 * \param[in]  KeyScan_FLAG: Specify the flag to check.
 *      This parameter can be one of the following values, which refer to \ref KEYSCAN_Flag_Definition.
 *      \arg KEYSCAN_FLAG_FIFOLIMIT: Overflow FIFO limit in one scan
 *      \arg KEYSCAN_INT_FLAG_THRESHOLD: FIFO threshold interrupt status
 *      \arg KEYSCAN_INT_FLAG_OVER_READ: FIFO over read interrupt status
 *      \arg KEYSCAN_INT_FLAG_SCAN_END: Scan finish interrupt status
 *      \arg KEYSCAN_INT_FLAG_FIFO_NOT_EMPTY: FIFO not empty interrupt status
 *      \arg KEYSCAN_INT_FLAG_ALL_RELEASE: Release interrupt status
 *      \arg KEYSCAN_FLAG_DATAFILTER: FIFO filter status
 *      \arg KEYSCAN_FLAG_OVR: The flag to indicate FIFO is overflow
 *      \arg KEYSCAN_FLAG_FULL: The flag to indicate FIFO is full
 *      \arg KEYSCAN_FLAG_EMPTY: The flag to indicate FIFO is empty
 * \return  The new state of KeyScan_FLAG (SET or RESET).
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     FlagStatus flag_status = KeyScan_GetFlagState(KEYSCAN, KEYSCAN_FLAG_OVR);
 *
 * }
 * \endcode
 */
__STATIC_INLINE FlagStatus KeyScan_GetFlagState(KEYSCAN_TypeDef *KeyScan, uint32_t KeyScan_FLAG)
{
    FlagStatus bitstatus = RESET;

    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));
    assert_param(IS_KEYSCAN_FLAG(KeyScan_FLAG));

    if ((KeyScan->STATUS & KeyScan_FLAG) != 0)
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/**
 * \brief  Read FIFO data.
 * \param[in] KeyScan: Selected KeyScan peripheral.
 * \return Keyscan FIFO data.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void keyscan_demo(void)
 * {
 *     uint16_t data = KeyScan_ReadFifoData(KEYSCAN);
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t KeyScan_ReadFifoData(KEYSCAN_TypeDef *KeyScan)
{
    /* Check the parameters */
    assert_param(IS_KeyScan_PERIPH(KeyScan));

    return (uint16_t)(KeyScan->FIFODATA);
}

/** End of KEYSCAN_Exported_Functions
  * \}
  */

/** End of KEYSCAN
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_KEYSCAN_H_ */


/******************* (C) COPYRIGHT 2024 Realtek Semiconductor *****END OF FILE****/
