#ifndef ZB_TST_H_
#define ZB_TST_H_

#if defined (__cplusplus)
extern "C" {
#endif

#define ZB_DBG_UART                         UART2
#define ZB_DBG_UART_TX                      UART2_TX
#define ZB_DBG_UART_RX                      UART2_RX
#define ZB_DBG_UART_IRQn                    UART2_IRQn
#define ZB_DBG_UART_VECTORn                 UART2_VECTORn
#define ZB_CLI_UART                         UART3
#define ZB_CLI_UART_TX                      UART3_TX
#define ZB_CLI_UART_RX                      UART3_RX
#define ZB_CLI_UART_IRQn                    UART3_IRQn
#define ZB_CLI_UART_VECTORn                 UART3_VECTORn

#define ZB_TIM                              TIM2

// Define UART PIN Mux
#ifdef BOARD_DONGLE // For dongle board
#define ZB_DBG_UART_TX_PIN                  P2_3
#define ZB_DBG_UART_RX_PIN                  P2_2
#else // For EVB board
#define ZB_DBG_UART_TX_PIN                  P3_0
#define ZB_DBG_UART_RX_PIN                  P3_1
#endif
#define ZB_CLI_UART_TX_PIN                  P2_4
#define ZB_CLI_UART_RX_PIN                  P2_5

#define ZB_RESET_PIN                        P0_5
#define ZB_INTERRUPT_PIN                    P0_4

#define ZB_TASK_PRIORITY                    4           //!< Task priorities
#define ZB_TASK_STACK_SIZE                  (1024 * 4)   //!< Task stack size

#if defined (__cplusplus)
}
#endif

#endif /* ! ZB_TST_H_ */
