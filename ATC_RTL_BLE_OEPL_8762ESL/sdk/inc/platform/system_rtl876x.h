/**
*****************************************************************************************
*     Copyright(c) 2016, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    System_rtl876x.h
  * @brief   CMSIS Device System Header file
  * @details CMSIS Device System Header File for RTL876x Device Series
  * @author
  * @date    3. March 2016
  * @version v1.0
  * *************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2016 Realtek Semiconductor Corporation</center></h2>
   * ************************************************************************************
   */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef SYSTEM_RTL876X_H
#define SYSTEM_RTL876X_H


/*============================================================================*
 *                      Headers
 *============================================================================*/
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup SYSTEM_RTL876X  System RTL876X
  * @brief CMSIS API sets for RTL876x Device Series
  * @{
  */

/*============================================================================*
 *                      Types
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_Types System RTL876X Exported Types
  * @{
  */
typedef void (*IRQ_Fun)();       /**< ISR Handler Prototype */

typedef enum
{
    LOG_TIMESTAMP_OS        = 0,
    LOG_TIMESTAMP_HW_TIMER  = 1,
    LOG_TIMESTAMP_RSVD      = 2,
    LOG_TIMESTAMP_MAX       = 3
} T_LOG_TIMESTAMP_TYPE;

typedef enum
{
    LOG_BAUD_RATE_9600,
    LOG_BAUD_RATE_19200,
    LOG_BAUD_RATE_115200,
    LOG_BAUD_RATE_230400,
    LOG_BAUD_RATE_460800,
    LOG_BAUD_RATE_921600,
    LOG_BAUD_RATE_2000000,
    LOG_BAUD_RATE_3000000,
    LOG_BAUD_RATE_4000000,
    LOG_BAUD_RATE_6000000
} LOG_BAUD_RATE_TypeDef;

typedef enum
{
    InitialSP_VECTORn = 0,
    Reset_VECTORn,
    NMI_VECTORn,
    HardFault_VECTORn,
    RSVD_MemMang_VECTORn,
    RSVD_BusFault_VECTORn,
    RSVD_UsageFault_VECTORn,
    RSVD0_VECTORn,
    RSVD1_VECTORn,
    RSVD2_VECTORn,
    RSVD3_VECTORn,
    SVC_VECTORn,
    DebugMonitor_VECTORn,
    RSVD4_VECTORn,
    PendSV_VECTORn,
    SysTick_VECTORn,
    System_VECTORn = 16,
    WDG_VECTORn,
    BTMAC_VECTORn,
    Timer3_VECTORn,
    Timer2_VECTORn,
    Platform_VECTORn,
    I2S0_TX_VECTORn,
    I2S0_RX_VECTORn,
    Timer4_5_VECTORn,
    GPIO4_VECTORn,
    GPIO5_VECTORn,
    Uart1_VECTORn,
    Uart0_VECTORn,
    RTC_VECTORn,
    SPI0_VECTORn,
    SPI1_VECTORn,
    I2C0_VECTORn,
    I2C1_VECTORn,
    ADC_VECTORn,
    Peripheral_VECTORn,
    GDMA0_Channel0_VECTORn,
    GDMA0_Channel1_VECTORn,
    GDMA0_Channel2_VECTORn,
    GDMA0_Channel3_VECTORn,
    Enhanced_Timer0_VECTORn,
    Enhanced_Timer1_VECTORn,
    GPIO_Group3_VECTORn,
    GPIO_Group2_VECTORn,
    IR_VECTORn,
    GPIO_Group1_VECTORn,
    GPIO_Group0_VECTORn,
    RSVD5_VECTORn,
    Timer4_VECTORn = 48,
    Timer5_VECTORn,
    SPIC0_VECTORn = 50,
    Qdecode_VECTORn,
    Keyscan_VECTORn,
    SPI2W_VECTORn,
    LPCOMP_VECTORn,
    PTA_Mailbox_VECTORn,
    CAP_Touch_VECTORn,
    TRNG_VECTORn = 57,
} VECTORn_Type;

typedef enum _SYSTEM_CALL_OPCODE
{
    SYSTEM_CALL_BASE            = 0x0,
    SYSTEM_CALL_WDG_RESET,
    SYSTEM_CALL_AUTO_K_RF,
    SYSTEM_CALL_PMU_FORCE_PWM_MODE,
    SYSTEM_CALL_ENTER_POWERDOWN_DIRECTLY,
} T_SYSTEM_CALL_OPCODE;

typedef bool (*APP_MAIN_FUNC)();
typedef void (*USER_CALL_BACK)();

/** @} */ /* End of group SYSTEM_RTL876X_Exported_types */

/*============================================================================*
 *                      Variables
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_Variables System RTL876X Exported Variables
    * @brief
    * @{
    */
extern APP_MAIN_FUNC upperstack_entry;
extern APP_MAIN_FUNC app_pre_main;
extern APP_MAIN_FUNC app_main;
extern USER_CALL_BACK app_pre_main_cb;
/** @} */ /* End of group SYSTEM_RTL876X_Exported_Variables */

/*============================================================================*
 *                      Functions
 *============================================================================*/
/** @defgroup SYSTEM_RTL876X_Exported_Functions System RTL876X Exported Functions
    * @brief
    * @{
    */

/**
 * @brief  Initialize RAM vector table to a given RAM address.
 * @param  ram_vector_addr: RAM Vector Address.
 * @retval TRUE Success
 * @retval FALSE Fail
 * @note   When using vector table relocation, the base address of the new vector
 *         table must be aligned to the size of the vector table extended to the
 *         next larger power of 2. In RTL8762C, the base address is aligned at 0x100.
 */
extern bool RamVectorTableInit(uint32_t ram_vector_addr);

/**
 * @brief  Update ISR Handler in RAM Vector Table.
 * @param  v_num: Vector number(index)
 * @param  isr_handler: User defined ISR Handler.
 * @retval TRUE Success
 * @retval FALSE Fail
 */
extern bool RamVectorTableUpdate(VECTORn_Type v_num, IRQ_Fun isr_handler);

/*
 * @brief  Call the system service.
 * @param  opcode: operation code.
 * @param  parm: parameter.
 * @return none
 */
extern void SystemCall(uint32_t opcode, uint32_t parm);

/*
 * @brief  update ram layout for app.
 * @param  app_global_size: app global ram size.
 * @param  data_heap_size: data on heap total size.
 * @param  share_cache_ram_size: the size of share cache to ram.
 * @return none
 */
extern void update_ram_layout(uint32_t app_global_size, uint32_t data_heap_size,
                              uint32_t share_cache_ram_size);


/**
 * @brief  get cpu clock
 * @param  none
 * @return uint32, for example 40000000 is 40M, 20000000 is 20M.
  */
extern uint32_t get_cpu_clock(void);


/**
 * @brief  get ic type
 * @param  none
 * @return uint8_t, ic type:
 * #define RTL8762ESF              0x1D
 * #define RTL8762EGF              0x18
 * #define RTL8762ERF              0x28
 * #define RTL8762EJF              0x29
 * #define RTL8762EMF              0x39
 * #define RS625MF                 0x49
 * #define RTL8762EKF              0x5A
 * #define RTL8762EKO              0x6C
 */
extern uint8_t get_ic_type(void);

/**
 * @brief  Deinit the IO function of one pin.
 * @param  Pin_Num: pin number.
 *     This parameter is from ADC_0 to P4_1, please refer to rtl876x.h "Pin_Number" part.
 * @retval None
 */
extern void Pinmux_Deinit_rom(uint8_t Pin_Num);


/** @} */ /* End of group SYSTEM_RTL876X_Exported_Functions */

/** @} */ /* End of group SYSTEM_RTL876X */

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_RTL876X_H */

