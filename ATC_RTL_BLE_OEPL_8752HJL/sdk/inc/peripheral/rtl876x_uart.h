/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* \file     rtl876x_uart.h
* \brief    Header file for the peripheral UART driver.
* \details  This file provides all UART firmware functions.
* \author   yuan
* \date     2024-01-18
* \version  v2.1.0
*********************************************************************************************************
*/

#ifndef _RTL876X_UART_H_
#define _RTL876X_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup  IO          Peripheral Drivers
 * \defgroup    UART        UART
 *
 * \brief       Manage the UART peripheral functions.
 *
 * \ingroup     IO
 */

/*============================================================================*
 *                         Includes
 *============================================================================*/
#include "rtl876x.h"
#include "rtl876x_alias.h"

/*============================================================================*
 *                         Types
 *============================================================================*/
/**
 * \defgroup    UART_Exported_Types     UART Exported Types
 *
 * \ingroup     UART
 */

/**
 * \brief       UART initialization parameters
 * \ingroup     UART_Exported_Types
 */
typedef struct
{
    //baudrate calibration
    uint16_t UART_OvsrAdj;              /*!< Specify the baud rate setting, ovsr_adj. Refer to the baud rate setting table. */
    uint16_t UART_Div;                  /*!< Specify the baud rate setting, div. Refer to the baud rate setting table. */
    uint16_t UART_Ovsr;                 /*!< Specify the baud rate setting, ovsr. Refer to the baud rate setting table. */
    uint16_t UART_WordLen;              /*!< Specify the UART Word length.
                                             This parameter can be a value of \ref UART_Word_Length. */
    uint16_t UART_StopBits;             /*!< Specify the stop bit.
                                             This parameter can be a value of \ref UART_Stop_Bits. */
    uint16_t UART_Parity;               /*!< Specify the parity.
                                             This parameter can be a value of \ref UART_Parity. */
    uint16_t UART_TxThdLevel;           /*!< Specify the Tx threshold level.
                                             This parameter must range from 1 to 16. */
    uint16_t UART_RxThdLevel;           /*!< Specify the Rx threshold level.
                                             This parameter must range from 1 to 31. */
    uint16_t UART_IdleTime;             /*!< Specify the idle time.
                                             This parameter can be a value of \ref UART_Rx_Idle_Time. */
    uint16_t UART_HardwareFlowControl;  /*!< Specify the hardware flow control.
                                             This parameter can be a value of \ref UART_Hardware_Flow_Control. */
    uint16_t UART_DmaEn;                /*!< Specify the DMA mode.
                                             This parameter can be DISABLE or ENABLE. */
    uint16_t UART_TxDmaEn;              /*!< Specify the Tx DMA mode.
                                             This parameter can be DISABLE or ENABLE.
                                             This bit field controls the level at which a DMA request is made by the transmit logic.
                                             (TX FIFO depth - TX_FIFO_LEVEL) >= UART_TxWaterLevel, that is, the dma_tx_req signal is
                                             generated when the number of free entries in the transmit FIFO is equal to or above this field value. */
    uint16_t UART_RxDmaEn;              /*!< Specify the Rx DMA mode.
                                             This parameter can be DISABLE or ENABLE.
                                             This Function controls the level at which a DMA request is made by the receive logic.
                                             RX_FIFO_LEVEL >= UART_RxWaterLevel ; that is, dma_rx_req is generated when the number of
                                             valid data entries in the receive FIFO is equal to or above this field value. */
    uint8_t UART_TxWaterLevel;          /*!< Specify the DMA Tx water level.
                                             This parameter must range from 1 to 16. */
    uint8_t UART_RxWaterLevel;          /*!< Specify the DMA Rx water level.
                                             This parameter must range from 1 to 31. */
} UART_InitTypeDef;

/** End of UART_Exported_Types
  * \}
  */

/*============================================================================*
 *                         Constants
 *============================================================================*/
/**
 * \defgroup    UART_Exported_Constants     UART Exported Constants
 *
 * \ingroup     UART
 */

/**
 * \defgroup    UART_Config UART Config
 * \{
 * \ingroup     UART_Exported_Constants
 */
#define IS_UART_PERIPH(PERIPH) (((PERIPH) == UART0) || ((PERIPH) == UART1) || ((PERIPH) == UART2))

#define UART_TX_FIFO_SIZE           16
#define UART_RX_FIFO_SIZE           32
/** \} */

/**
 * \defgroup    UART_BAUD_RATE_Define UART BAUD RATE Define
 * \{
 * \ingroup     UART_Exported_Constants
 */
typedef enum
{
    UART_BAUD_RATE_1200,
    UART_BAUD_RATE_9600,
    UART_BAUD_RATE_14400,
    UART_BAUD_RATE_19200,
    UART_BAUD_RATE_28800,
    UART_BAUD_RATE_38400,
    UART_BAUD_RATE_57600,
    UART_BAUD_RATE_76800,
    UART_BAUD_RATE_115200,
    UART_BAUD_RATE_128000,
    UART_BAUD_RATE_153600,
    UART_BAUD_RATE_230400,
    UART_BAUD_RATE_460800,
    UART_BAUD_RATE_500000,
    UART_BAUD_RATE_921600,
    UART_BAUD_RATE_1000000,
    UART_BAUD_RATE_1382400,
    UART_BAUD_RATE_1444400,
    UART_BAUD_RATE_1500000,
    UART_BAUD_RATE_1843200,
    UART_BAUD_RATE_2000000,
    UART_BAUD_RATE_3000000,
    UART_BAUD_RATE_4000000,
    UART_BAUD_RATE_6000000
} UartBaudRate_TypeDef;
/** \} */

/**
 * \defgroup   UART_Interrupts_Definition  UART Interrupts Definition
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_INT_RD_AVA                         ((uint16_t)(1 << 0)) // Receive data available
#define UART_INT_TX_FIFO_EMPTY                  ((uint16_t)(1 << 1))
#define UART_INT_RX_LINE_STS                    ((uint16_t)(1 << 2))
#define UART_INT_TX_DONE                        ((uint16_t)(1 << 4))
#define UART_INT_TX_THD                         ((uint16_t)(1 << 5))
#define UART_INT_RX_BREAK                       ((uint16_t)(1 << 6))
#define UART_INT_RX_IDLE                        ((uint16_t)(1 << 7))

#define IS_UART_INT(INT) ((((INT) & 0xFFFFFF80) == 0x00) && ((INT) != 0x00))

#define IS_UART_GET_INT(INT) ((INT) & (UART_INT_RD_AVA | UART_INT_TX_FIFO_EMPTY | UART_INT_RX_LINE_STS |\
                                       UART_INT_MODEM_STS | UART_INT_TX_DONE | UART_INT_TX_THD |\
                                       UART_INT_RX_IDLE))
/** \} */

/**
 * \defgroup   UART_Interrupt_Identifier  UART Interrupt Identifier
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_INT_PENDING                        ((uint16_t)(1 << 0))
#define UART_INT_ID_LINE_STATUS                 ((uint16_t)(0x03 << 1))
#define UART_INT_ID_RX_LEVEL_REACH              ((uint16_t)(0x02 << 1))
#define UART_INT_ID_RX_DATA_TIMEOUT             ((uint16_t)(0x06 << 1))
#define UART_INT_ID_TX_EMPTY                    ((uint16_t)(0x01 << 1))
#define UART_INT_ID_MODEM_STATUS                ((uint16_t)(0x00 << 1))

#define IS_UART_INT_ID(ID) (((ID) == UART_INT_ID_LINE_STATUS) || ((ID) == UART_INT_ID_RX_LEVEL_REACH) || \
                            ((ID) == UART_INT_ID_RX_DATA_TIMEOUT) || ((ID) == UART_INT_ID_TX_EMPTY) || \
                            ((ID) == UART_INT_ID_MODEM_STATUS))
/** \} */

/**
 * \defgroup   UART_Parity UART Parity
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_PARITY_NO_PARTY                    ((uint16_t)(0x00 << 3))
#define UART_PARITY_ODD                         ((uint16_t)(0x01 << 3))
#define UART_PARITY_EVEN                        ((uint16_t)(0x03 << 3))

#define IS_UART_PARITY(PARITY) (((PARITY) == UART_PARITY_NO_PARTY) || ((PARITY) == UART_PARITY_ODD)\
                                ((PARITY) == UART_PARITY_EVEN))
/** \} */

/**
 * \defgroup   UART_Stop_Bits  UART Stop Bits
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_STOP_BITS_1                        ((uint16_t)(0 << 2))
#define UART_STOP_BITS_2                        ((uint16_t)(1 << 2))

#define IS_UART_STOPBITS(STOP) (((STOP) == UART_STOP_BITS_1) || ((STOP) == UART_STOP_BITS_2))
/** \} */

/**
 * \defgroup    UART_Word_Length UART Word Length
 * \{
 * \ingroup     UART_Exported_Constants
 */
#define UART_WORD_LENGTH_7BIT                   ((uint16_t)(0 << 0))
#define UART_WORD_LENGTH_8BIT                   ((uint16_t)(1 << 0))

#define IS_UART_WORD_LENGTH(LEN) ((((LEN)) == UART_WORD_LENGTH_7BIT) || (((LEN)) == UART_WORD_LENGTH_8BIT))
/** \} */

/**
 * \defgroup   UART_Hardware_Flow_Control  UART Hardware Flow Control
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_HW_FLOW_CTRL_DISABLE               ((uint16_t)0x00)
#define UART_HW_FLOW_CTRL_ENABLE                ((uint16_t)((1 << 5) | (1 << 1)))

#define IS_UART_AUTO_FLOW_CTRL(CTRL) (((CTRL) == UART_AUTO_FLOW_CTRL_EN) || ((CTRL) == UART_AUTO_FLOW_CTRL_DIS))
/** \} */

/**
 * \defgroup   UART_Flag UART Flag
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_FLAG_RX_DATA_AVA                   ((uint16_t)(1 << 0))
#define UART_FLAG_RX_OVERRUN                    ((uint16_t)(1 << 1))
#define UART_FLAG_RX_PARITY_ERR                 ((uint16_t)(1 << 2))
#define UART_FLAG_RX_FRAME_ERR                  ((uint16_t)(1 << 3))
#define UART_FLAG_RX_BREAK_ERR                  ((uint16_t)(1 << 4))
#define UART_FLAG_TX_FIFO_EMPTY                 ((uint16_t)(1 << 5))  // Transmitter Holding Register or Transmitter FIFO empty
#define UART_FLAG_TX_EMPTY                      ((uint16_t)(1 << 6))  // Transmitter Holding Register (or tx FIFO) and Transmitter shift Register empty
#define UART_FLAG_RX_FIFO_ERR                   ((uint16_t)(1 << 7))
#define UART_FLAG_RX_BREAK                      ((uint16_t)(1 << 8))
#define UART_FLAG_RX_IDLE                       ((uint16_t)(1 << 9))  // Only to show difference because the address of UART RX Idle flag is isolate
#define UART_FLAG_TX_DONE                       ((uint16_t)(1 << 10)) // Interrupt status of tx waveform done & TX_FIFO_EMPTY = 1
#define UART_FLAG_TX_THD                        ((uint16_t)(1 << 11)) // Interrupt status of TX_FIFO_LEVEL <= txfifo_trigger_level

#define IS_UART_GET_FLAG(FLAG) (((FLAG) == UART_FLAG_RX_DATA_AVA) || ((FLAG) == UART_FLAG_RX_OVERRUN) || \
                                ((FLAG) == UART_FLAG_RX_PARITY_ERR) || ((FLAG) == UART_FLAG_RX_FRAME_ERR) || \
                                ((FLAG) == UART_FLAG_RX_BREAK_ERR) || ((FLAG) == UART_FLAG_TX_FIFO_EMPTY) || \
                                ((FLAG) == UART_FLAG_TX_EMPTY) || ((FLAG) == UART_FLAG_RX_FIFO_ERR) || \
                                ((FLAG) == UART_FLAG_RX_BREAK) || ((FLAG) == UART_FLAG_RX_IDLE) || \
                                ((FLAG) == UART_FLAG_TX_DONE) || ((FLAG) == UART_FLAG_TX_THD))
/** \} */

/**
 * \defgroup   UART_DMA UART DMA
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_DMA_DISABLE                        ((uint16_t)(0 << 3))
#define UART_DMA_ENABLE                         ((uint16_t)(1 << 3))

#define IS_UART_DMA_CFG(CFG) (((CFG) == UART_DMA_ENABLE) || ((CFG) == UART_DMA_DISABLE))
/** \} */

/**
 * \defgroup   UART_Rx_Idle_Time   UART Rx Idle Time
 * \{
 * \ingroup     UART_Exported_Constants
 */

#define UART_RX_IDLE_1BYTE                      ((uint16_t)(0x00))
#define UART_RX_IDLE_2BYTE                      ((uint16_t)(0x01))
#define UART_RX_IDLE_4BYTE                      ((uint16_t)(0x02))
#define UART_RX_IDLE_8BYTE                      ((uint16_t)(0x03))
#define UART_RX_IDLE_16BYTE                     ((uint16_t)(0x04))
#define UART_RX_IDLE_32BYTE                     ((uint16_t)(0x05))
#define UART_RX_IDLE_64BYTE                     ((uint16_t)(0x06))
#define UART_RX_IDLE_128BYTE                    ((uint16_t)(0x07))
#define UART_RX_IDLE_256BYTE                    ((uint16_t)(0x08))
#define UART_RX_IDLE_512BYTE                    ((uint16_t)(0x09))
#define UART_RX_IDLE_1024BYTE                   ((uint16_t)(0x0A))
#define UART_RX_IDLE_2048BYTE                   ((uint16_t)(0x0B))
#define UART_RX_IDLE_4096BYTE                   ((uint16_t)(0x0C))
#define UART_RX_IDLE_8192BYTE                   ((uint16_t)(0x0D))
#define UART_RX_IDLE_16384BYTE                  ((uint16_t)(0x0E))
#define UART_RX_IDLE_32768BYTE                  ((uint16_t)(0x0F))

#define IS_UART_IDLE_TIME(TIME) ((TIME) <= 0x0F)
/** \} */

/**
 * \defgroup   UART_Interrupts_Mask_Definition  UART Interrupts Mask Definition
 * \{
 * \ingroup     UART_Exported_Constants
 */
#define UART_INT_MASK_RD_AVA                    ((uint16_t)(1 << 0))
#define UART_INT_MASK_TX_FIFO_EMPTY             ((uint16_t)(1 << 1))
#define UART_INT_MASK_RX_LINE_STS               ((uint16_t)(1 << 2))
#define UART_INT_MASK_RX_BREAK                  ((uint16_t)(1 << 4))
#define UART_INT_MASK_RX_IDLE                   ((uint16_t)(1 << 5))
#define UART_INT_MASK_TX_DONE                   ((uint16_t)(1 << 6)) // Transmitter Holding Register (or tx FIFO) and Transmitter shift Register both empty
#define UART_INT_MASK_TX_THD                    ((uint16_t)(1 << 7))

#define IS_UART_INT_MASK(INT) ((INT) & (UART_INT_MASK_RD_AVA | UART_INT_MASK_TX_FIFO_EMPTY | UART_INT_MASK_RX_LINE_STS | \
                                        UART_INT_MASK_RX_BREAK | UART_INT_MASK_RX_IDLE | UART_INT_MASK_TX_DONE | \
                                        UART_INT_MASK_TX_THD))
/** \} */

/**
 * \cond
 * \defgroup   UART_Tx_Rx_FIFO_CLEAR_BIT   UART TRx Fifo Clear Bits
 * \{
 */
#define FCR_CLEAR_RX_FIFO_Set           ((uint32_t)(1 << 1))
#define FCR_CLEAR_RX_FIFO_Reset         ((uint32_t)~(1 << 1))
#define FCR_CLEAR_TX_FIFO_Set           ((uint32_t)(1 << 2))
#define FCR_CLEAR_TX_FIFO_Reset         ((uint32_t)~(1 << 2))
/**
 * \}
 * \endcond
 */

/** End of UART_Exported_Constants
  * \}
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/**
 * \defgroup    UART_Exported_Functions UART Exported Functions
 * \{
 * \ingroup     UART
 */

/**
 * \brief   Deinitialize the selected UART peripheral registers to their default reset values (turns off UART clock).
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_uart_init(void)
 * {
 *    UART_DeInit(UART0);
 * }
 * \endcode
 */
void UART_DeInit(UART_TypeDef *UARTx);

/**
 * \brief   Initialize the selected UART peripheral according to the specified parameters in UART_InitStruct.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in] UART_InitStruct: Pointer to a UART_InitTypeDef structure that
*             contains the configuration information for the selected UART peripheral.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_uart_init(void)
 * {
 *     UART_DeInit(UART0);
 *
 *     RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
 *
 *     UART_InitTypeDef UART_InitStruct;
 *     UART_StructInit(&UART_InitStruct);
 *     UART_InitStruct.UART_Div         = 20;
 *     UART_InitStruct.UART_Ovsr        = 12;
 *     UART_InitStruct.UART_OvsrAdj     = 0x252;
 *     UART_InitStruct.UART_RxThdLevel  = 16;
 *    // Add other initialization parameters that need to be configured here.
 *     UART_Init(UART0, &UART_InitStruct);
 * }
 * \endcode
 */
void UART_Init(UART_TypeDef *UARTx, UART_InitTypeDef *UART_InitStruct);

/**
 * \brief   Fill each UART_InitStruct member with its default value.
 * \param[in] UART_InitStruct: pointer to a UART_InitTypeDef structure which will be initialized.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_uart_init(void)
 * {
 *     UART_DeInit(UART0);
 *
 *     RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
 *
 *     UART_InitTypeDef UART_InitStruct;
 *     UART_StructInit(&UART_InitStruct);
 *     UART_InitStruct.UART_Div         = 20;
 *     UART_InitStruct.UART_Ovsr        = 12;
 *     UART_InitStruct.UART_OvsrAdj     = 0x252;
 *     UART_InitStruct.UART_RxThdLevel  = 16;
 *     // Add other initialization parameters that need to be configured here.
 *     UART_Init(UART0, &UART_InitStruct);
 * }
 * \endcode
 */
void UART_StructInit(UART_InitTypeDef *UART_InitStruct);

/**
 * \brief   Mask or unmask the specified UART interrupt source.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in] UART_INT_MASK: Specify the UART interrupt source to be masked or unmasked.
 *                           This parameter can be any combination of the following values, refer to \ref UART_Interrupts_Mask_Definition.
 *      \arg UART_INT_MASK_RD_AVA: Mask INTCR (erbi).
 *      \arg UART_INT_MASK_TX_FIFO_EMPTY: Mask INTCR (etbei).
 *      \arg UART_INT_MASK_RX_LINE_STS: Mask INTCR (elsi).
 *      \arg UART_INT_MASK_RX_BREAK: Mask rx break interrupt.
 *      \arg UART_INT_MASK_RX_IDLE: Mask rx idle interrupt.
 *      \arg UART_INT_MASK_TX_DONE: Mask tx done interrupt.
 *      \arg UART_INT_MASK_TX_THD: Mask tx fifo threshold interrupt.
 * \param[in] NewState: New state of the specified UART interrupt source.
 *                      This parameter can be: ENABLE or DISABLE.
 * \return None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_uart_init(void)
 * {
 *     UART_DeInit(UART0);
 *
 *     RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
 *
 *     UART_InitTypeDef UART_InitStruct;
 *     UART_StructInit(&UART_InitStruct);
 *     UART_InitStruct.UART_Div         = 20;
 *     UART_InitStruct.UART_Ovsr        = 12;
 *     UART_InitStruct.UART_OvsrAdj     = 0x252;
 *     UART_InitStruct.UART_RxThdLevel  = 16;
 *     // Add other initialization parameters that need to be configured here.
 *     UART_Init(UART0, &UART_InitStruct);
 *
 *     UART_MaskINTConfig(UART0, UART_INT_MASK_RD_AVA, ENABLE);
 *     UART_INTConfig(UART0, UART_INT_RD_AVA, ENABLE);
 *     UART_MaskINTConfig(UART0, UART_INT_MASK_RD_AVA, DISABLE);
 * }
 * \endcode
 */
void UART_MaskINTConfig(UART_TypeDef *UARTx, uint32_t UART_INT_MASK, FunctionalState NewState);

/**
 * \brief   Enable or disable the specified UART interrupts.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in] UART_IT: Specify the UART interrupt source to be enabled or disabled.
 *                     This parameter can be any combination of the following values, refer to \ref UART_Interrupts_Definition.
 *      \arg UART_INT_RD_AVA: Rx data available interrupt source.
 *      \arg UART_INT_TX_FIFO_EMPTY: Tx FIFO empty interrupt source.
 *      \arg UART_INT_RX_LINE_STS: Rx line status interrupt source.
 *      \arg UART_INT_MODEM_STS: Modem status interrupt source.
 *      \arg UART_INT_TX_DONE: Tx done interrupt source.
 *      \arg UART_INT_TX_THD: Tx threshold (FIFO data length <= threshold) interrupt source.
 *      \arg UART_INT_RX_BREAK: Rx break signal interrupt source.
 *      \arg UART_INT_RX_IDLE: Rx bus idle interrupt source.
 * \param[in] NewState: New state of the specified UART interrupt source.
 *            This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void driver_uart_init(void)
 * {
 *     UART_DeInit(UART0);
 *
 *     RCC_PeriphClockCmd(APBPeriph_UART0, APBPeriph_UART0_CLOCK, ENABLE);
 *
 *     UART_InitTypeDef UART_InitStruct;
 *     UART_StructInit(&UART_InitStruct);
 *     UART_InitStruct.UART_Div = 20;
 *     UART_InitStruct.UART_Ovsr = 12;
 *     UART_InitStruct.UART_OvsrAdj = 0x252;
 *     UART_InitStruct.UART_RxThdLevel = 16;
 *     // Add other initialization parameters that need to be configured here.
 *     UART_Init(UART0, &UART_InitStruct);
 *
 *     UART_INTConfig(UART0, UART_INT_RD_AVA, ENABLE);
 * }
 * \endcode
 */
void UART_INTConfig(UART_TypeDef *UARTx, uint32_t UART_IT, FunctionalState NewState);

/**
 * \brief   Send data.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in] inBuf: Buffer of data to be sent.
 * \param[in] count: Length of data to be sent.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint8_t data[] = "UART demo";
 *     UART_SendData(UART0, data, sizeof(data));
 * }
 * \endcode
 */
void UART_SendData(UART_TypeDef *UARTx, const uint8_t *inBuf, uint16_t count);

/**
 * \brief   Receive data from RX FIFO.
 * \param[in]  UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[out] outBuf: Buffer to store data read from RX FIFO.
 * \param[in]  count: Length of data to read.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint8_t data[32] = {10};
 *     UART_ReceiveData(UART0, data, 10);
 * }
 * \endcode
 */
void UART_ReceiveData(UART_TypeDef *UARTx, uint8_t *outBuf, uint16_t count);

/**
 * \brief   Set UART communication baudrate.
 * \param[in]   UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in]   div: Parameter of the selected UART baudrate.
 * \param[in]   ovsr: Parameter of the selected UART baudrate.
 * \param[in]   ovsr_adj: Parameter of the selected UART baudrate.
 * \return  None.
 *
 * \note UART_BaudRate_Table div/ovsr/ovsr_adj: These three parameters set the baud rate calibration parameters of UART.
 *
 *  |baudrate   |div     |ovsr    |ovsr_adj   |
 *  |:---------:|:------:|:------:|:---------:|
 *  |1200Hz     |2589    |7       |0x7F7      |
 *  |2400Hz     |1200    |8       |0x3EF      |
 *  |4800Hz     |600     |8       |0x3EF      |
 *  |9600Hz     |271     |10      |0x24A      |
 *  |14400Hz    |271     |5       |0x222      |
 *  |19200Hz    |165     |7       |0x5AD      |
 *  |28800Hz    |110     |7       |0x5AD      |
 *  |38400Hz    |85      |7       |0x222      |
 *  |57600Hz    |55      |7       |0x5AD      |
 *  |76800Hz    |35      |9       |0x7EF      |
 *  |115200Hz   |20      |12      |0x252      |
 *  |128000Hz   |25      |7       |0x555      |
 *  |153600Hz   |15      |12      |0x252      |
 *  |230400Hz   |10      |12      |0x252      |
 *  |460800Hz   |5       |12      |0x252      |
 *  |500000Hz   |8       |5       |0          |
 *  |921600Hz   |4       |5       |0x3F7      |
 *  |1000000Hz  |4       |5       |0          |
 *  |1382400Hz  |2       |9       |0x2AA      |
 *  |1444400Hz  |2       |8       |0x5F7      |
 *  |1500000Hz  |2       |8       |0x492      |
 *  |1843200Hz  |2       |5       |0x3F7      |
 *  |2000000Hz  |2       |5       |0          |
 *  |2100000Hz  |1       |14      |0x400      |
 *  |2764800Hz  |1       |9       |0x2AA      |
 *  |3000000Hz  |1       |8       |0x492      |
 *  |3250000Hz  |1       |7       |0x112      |
 *  |3692300Hz  |1       |5       |0x5F7      |
 *  |3750000Hz  |1       |5       |0x36D      |
 *  |4000000Hz  |1       |5       |0          |
 *  |6000000Hz  |1       |1       |0x36D      |
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint16_t div = 20;
 *     uint16_t ovsr = 12;
 *     uint16_t ovsr_adj = 0x252;
 *     UART_SetBaudRate(UART0, div, ovsr, ovsr_adj);
 * }
 * \endcode
 */
void UART_SetBaudRate(UART_TypeDef *UARTx, uint16_t div, uint16_t ovsr, uint16_t ovsr_adj);

/**
 * \brief   Set UART communication parameters.
 * \param[in]   UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in]   wordLen: Data width of the selected UART peripheral.
 *              This parameter can be one of the following values:
 *      \arg UART_WORD_LENGTH_7BIT: 7-bit data length.
 *      \arg UART_WORD_LENGTH_8BIT: 8-bit data length.
 * \param[in]   parity: Parity of the selected UART peripheral.
 *              This parameter can be one of the following values:
 *      \arg UART_PARITY_NO_PARITY: No parity.
 *      \arg UART_PARITY_ODD: Odd parity.
 *      \arg UART_PARITY_EVEN: Even parity.
 * \param[in]   stopBits: Stop bit of the selected UART peripheral.
 *              This parameter can be one of the following values:
 *      \arg UART_STOP_BITS_1: 1-bit stop bit.
 *      \arg UART_STOP_BITS_2: 2-bit stop bit.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint16_t word_len = UART_WORD_LENGTH_8BIT;
 *     uint16_t parity = UART_PARITY_NO_PARTY;
 *     uint16_t stop_bits = UART_STOP_BITS_1;
 *     UART_SetParams(UART0, wordLen, parity, stopBits);
 * }
 * \endcode
 */
void UART_SetParams(UART_TypeDef *UARTx, uint16_t wordLen, uint16_t parity, uint16_t stopBits);

/**
 * \brief   Configure UART loopback function.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in] NewState: New state of the UART loopback function.
 *                      This parameter can be: ENABLE or DISABLE.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     UART_LoopBackCmd(UART0, ENABLE);
 * }
 * \endcode
 */
void UART_LoopBackCmd(UART_TypeDef *UARTx, FunctionalState NewState);

/**
 * \brief  Check whether the specified UART flag is set.
 * \param[in]  UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in]  UART_FLAG: Specify the UART flag to check.
 *      This parameter can be one of the following values, refer to \ref UART_Flag.
 *      \arg UART_FLAG_RX_DATA_AVA: Rx data is available.
 *      \arg UART_FLAG_RX_OVERRUN: Rx FIFO overrun.
 *      \arg UART_FLAG_RX_PARITY_ERR: Rx parity error.
 *      \arg UART_FLAG_RX_FRAME_ERR: Rx frame error.
 *      \arg UART_FLAG_RX_BREAK_ERR: Rx break error.
 *      \arg UART_FLAG_TX_FIFO_EMPTY: Tx Holding Register or Tx FIFO empty.
 *      \arg UART_FLAG_TX_EMPTY: Tx FIFO and Tx shift register are both empty.
 *      \arg UART_FLAG_RX_FIFO_ERR: Rx FIFO error.
 *      \arg UART_FLAG_RX_BREAK: Rx break.
 *      \arg UART_FLAG_RX_IDLE: Rx idle.
 *      \arg UART_FLAG_TX_DONE: Tx waveform done & TX_FIFO_EMPTY = 1.
 *      \arg UART_FLAG_TX_THD: TX_FIFO_LEVEL <= txfifo_trigger_level.
 * \return New status of the UART flag.
 * \retval SET: The specified UART flag bit is set.
 * \retval RESET: The specified flag is not set.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_senddata_continuous(UART_TypeDef *UARTx, const uint8_t *pSend_Buf, uint16_t vCount)
 * {
 *     uint8_t count;
 *
 *     while (vCount / UART_TX_FIFO_SIZE > 0)
 *     {
 *         while (UART_GetFlagStatus(UARTx, UART_FLAG_TX_FIFO_EMPTY) == RESET);
 *         for (count = UART_TX_FIFO_SIZE; count > 0; count--)
 *         {
 *             UARTx->RB_THR = *pSend_Buf++;
 *         }
 *         vCount -= UART_TX_FIFO_SIZE;
 *     }
 *     while (UART_GetFlagStatus(UARTx, UART_FLAG_TX_FIFO_EMPTY) == RESET);
 *     while (vCount--)
 *     {
 *         UARTx->RB_THR = *pSend_Buf++;
 *     }
 * }
 * \endcode
 */
FlagStatus UART_GetFlagStatus(UART_TypeDef *UARTx, uint32_t UART_FLAG);

/**
 * \brief   Send one byte of data.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \param[in] data: Byte data to send.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint8_t data = 0x55;
 *     UART_SendByte(UART0, data);
 * }
 * \endcode
 */
__STATIC_INLINE void UART_SendByte(UART_TypeDef *UARTx, uint8_t data)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    UARTx->RB_THR = data;

    return;
}

/**
 * \brief   Read a byte of data from UART RX FIFO.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  The byte data that has been read.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint8_t data = UART_ReceiveByte(UART0);
 *
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t UART_ReceiveByte(UART_TypeDef *UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    return (uint8_t)(UARTx->RB_THR);
}

/**
 * \brief   Get the interrupt identifier of the selected UART peripheral.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  The interrupt identifier value.
 *          This return value can be one of the following \ref UART_Interrupt_Identifier.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void UART0_Handler(void)
 * {
 *     //Get interrupt ID.
 *     uint32_t int_status = UART_GetIID(UART0);
 * }
 * \endcode
 */
__STATIC_INLINE uint16_t UART_GetIID(UART_TypeDef *UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    return (uint16_t)(UARTx->INTID_FCR & 0x0000000E);
}

/**
 * \brief   Clear Tx FIFO of the selected UART peripheral.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     UART_ClearTxFIFO(UART0);
 * }
 * \endcode
 */
__STATIC_INLINE void UART_ClearTxFIFO(UART_TypeDef *UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    UARTx->INTID_FCR = (((UARTx->STSR & BIT24) >> 21) | ((UARTx->STSR & 0x7C000000) >> 18) |
                        ((UARTx->STSR & BIT25) >> 25) | FCR_CLEAR_TX_FIFO_Set);
}

/**
 * \brief   Clear Rx FIFO of the selected UART peripheral.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     UART_ClearRxFIFO(UART0);
 * }
 * \endcode
 */
__STATIC_INLINE void UART_ClearRxFIFO(UART_TypeDef *UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    UARTx->INTID_FCR = (((UARTx->STSR & BIT24) >> 21) | ((UARTx->STSR & 0x7C000000) >> 18) |
                        ((UARTx->STSR & BIT25) >> 25) | FCR_CLEAR_RX_FIFO_Set);
}

/**
 * \brief   Get the data length in Tx FIFO of the selected UART peripheral.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  Data length in UART TX FIFO.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint8_t data_len = UART_GetTxFIFODataLen(UART0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t UART_GetTxFIFODataLen(UART_TypeDef *UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    return (uint8_t)(UARTx->FIFO_LEVEL & 0x1F);
}

/**
 * \brief   Get the data length in Rx FIFO of the selected UART peripheral.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  Data length in UART RX FIFO.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     uint8_t data_len = UART_GetRxFIFODataLen(UART0);
 * }
 * \endcode
 */
__STATIC_INLINE uint8_t UART_GetRxFIFODataLen(UART_TypeDef *UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    return (uint8_t)((UARTx->FIFO_LEVEL >> 8) & 0x3F);
}

/**
 * \brief   Clear Rx break of the selected UART peripheral.
 * \param[in] UARTx: UART peripheral selected, x can be 0 ~ 2.
 * \return  None.
 *
 * <b>Example usage</b>
 * \code{.c}
 *
 * void uart_demo(void)
 * {
 *     UART_ClearRxBreak(UART0);
 * }
 * \endcode
 */
__STATIC_INLINE void UART_ClearRxBreak(UART_TypeDef *UARTx)
{
    /* Check the parameters */
    assert_param(IS_UART_PERIPH(UARTx));

    UARTx->LSR |= BIT7;
    return;
}

/** End of UART_Exported_Functions
  * \}
  */

/** End of UART
  * \}
  */

#ifdef __cplusplus
}
#endif

#endif /* _RTL876X_UART_H_ */


/******************* (C) COPYRIGHT 2019 Realtek Semiconductor *****END OF FILE****/

