#ifndef VECTOR_TABLE_H
#define VECTOR_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

/** @defgroup VECTOR_TABLE  Vector Table
  * @brief
  * @{
  */

/** @defgroup VECTOR_TABLE_Exported_Types Vector Table Exported Types
  * @brief
  * @{
  */
typedef void (*IRQ_Fun)(void);       /**< ISR Handler Prototype */

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
    TMETER_VECTORn,
    BTMAC_VECTORn,
    Timer3_VECTORn,
    Timer2_VECTORn,
    BTMAC_WRAP_AROUND_VECTORn,
    I2S0_TX_VECTORn,
    I2S0_RX_VECTORn,
    Timer4_5_VECTORn,
    ZIGBEE_VECTORn,
    PF_RTC_VECTORn,
    UART0_VECTORn,
    UART1_VECTORn,
    I8080_VECTORn,
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
    UART2_VECTORn,
    Timer4_VECTORn,
    Timer5_VECTORn,
    SPIC0_VECTORn,
    Qdecode_VECTORn,
    Keyscan_VECTORn,
    SPI2W_VECTORn,
    LPCOMP_VECTORn,
    PTA_Mailbox_VECTORn,
    SPIC1_VECTORn,
    SHA256_VECTORn,
    Platform_VECTORn,
    TRNG_VECTORn,
    FLASH_SEC_VECTORn,
    RTC_VECTORn,
    WDT_VECTORn,
} VECTORn_Type;

extern uint32_t vector_table_level_two;

/** End of VECTOR_TABLE_Exported_Types
  * @}
  */

/*============================================================================*
*                              Functions
*============================================================================*/
/** @defgroup VECTOR_TABLE_Exported_Functions Vector Table Exported Functions
  * @brief
  * @{
  */

/**
 * @brief  Initialize RAM vector table to a given RAM address.
 *
 * @param[in]  ram_vector_addr: RAM Vector Address.
 *
 * @return Init result
 * @retval true Success
 * @retval false Fail
 *
 * @note   When using vector table relocation, the base address of the new vector
 *         table must be aligned to the size of the vector table extended to the
 *         next larger power of 2. In RTL8762C, the base address is aligned at 0x100.
 */
extern bool RamVectorTableInit(uint32_t ram_vector_addr);

/**
 * @brief  Update ISR Handler in RAM Vector Table.
 *
 * @param[in]  v_num: Vector number(index)
 * @param[in]  isr_handler: User defined ISR Handler.
 *
 * @return Update result.
 * @retval true Success
 * @retval false Fail
 */
extern bool RamVectorTableUpdate(VECTORn_Type v_num, IRQ_Fun isr_handler);

/**
 * @brief  Default Handler in ROM.
 *
 */
extern void ROM_Default_Handler(void);

/**
 * @brief  Default UART0_Handler in ROM.
 *
 * @note src\app\ppt_esb_test\data_uart.c need this
 *
 */
extern void UART0_Handler(void);

/**
 * @brief Default BTMAC_Handler in ROM.
 */
extern void BTMAC_Handler(void);

/** End of VECTOR_TABLE_Exported_Functions
  * @}
  */

/** End of VECTOR_TABLE
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif // VECTOR_TABLE_H
