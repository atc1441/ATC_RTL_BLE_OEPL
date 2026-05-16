#ifndef _BOARD_H_
#define _BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtl876x_pinmux.h"

/*******************************************************
*                 Debug UART
*******************************************************/
#define UART_TX_PIN     P3_0    /* GPIO24 — TXD_PAD  */
#define UART_RX_PIN     P3_1    /* GPIO25 — RXD_PAD  */
#define UART_BAUDRATE   115200

/*******************************************************
*                 EPD — Software SPI
*
*  SPI is bit-banged via GPIO.
*  EPD_BS must be held LOW at all times (SPI mode select).
*  EPD_PWR enables the display power rail (HIGH = on).
*******************************************************/
#define EPD_BS_PIN      P3_2    /* GPIO26 — Bootstrap: tie LOW (SPI mode) */
#define EPD_BUSY_PIN    P3_3    /* GPIO27 — Busy flag: input, HIGH = busy  */
#define EPD_RST_PIN     P0_0    /* GPIO0  — Reset: active LOW              */
#define EPD_DC_PIN      P0_1    /* GPIO1  — Data/Command: HIGH=data LOW=cmd*/
#define EPD_CS_PIN      P0_2    /* GPIO2  — Chip Select: active LOW        */
#define EPD_CLK_PIN     P0_4    /* GPIO4  — SPI clock                      */
#define EPD_PWR_PIN     P0_5    /* GPIO5  — Power enable: HIGH = on        */
#define EPD_MOSI_PIN    P0_6    /* GPIO6  — SPI MOSI                       */

/*******************************************************
*                 NFC — Software I2C
*
*  SDA/SCL are open-drain; provide external pull-ups.
*  NFC_PWR enables the NFC IC power rail (HIGH = on).
*  NFC_FIELD is an input that signals an RF field present.
*
*  NOTE: NFC_FIELD_PIN = P5_2 (pad index 38) is in the
*  AON (Always-On) GPIO domain.  It cannot be configured
*  with the standard GPIO_Init / GPIO_GetPin API.
*  AON GPIO configuration will be added when the NFC
*  driver is implemented.
*******************************************************/
#define NFC_PWR_PIN     P2_2    /* GPIO18 — Power enable: HIGH = on        */
#define NFC_SDA_PIN     P2_4    /* GPIO20 — I2C SDA (open-drain)           */
#define NFC_SCL_PIN     P2_5    /* GPIO21 — I2C SCL (open-drain)           */
#define NFC_FIELD_PIN   P5_2    /* pad 38  — RF field detect (AON GPIO)    */

/*******************************************************
*                 External SPI Flash — Bit-bang
*
*  Image-storage flash (EPD images).  Bit-banged via GPIO.
*  Separate from the firmware flash (internal to the SoC).
*
*  Pin mapping  (P4_x = GPIO28-31 on RTL8762E):
*    P4_0 → CLK   (GPIO28)
*    P4_1 → MISO  (GPIO29)
*    P4_2 → MOSI  (GPIO30)
*    P4_3 → CS    (GPIO31, active LOW)
*******************************************************/
#define EXT_FLASH_CS_PIN    P4_3
#define EXT_FLASH_CLK_PIN   P4_0
#define EXT_FLASH_MOSI_PIN  P4_2
#define EXT_FLASH_MISO_PIN  P4_1

/*******************************************************
*                 DLPS Module Config
*******************************************************/
#define DLPS_EN                         1



/* if use user define dlps enter/dlps exit callback function */
#define USE_USER_DEFINE_DLPS_ENTER_CB   1
#define USE_USER_DEFINE_DLPS_EXIT_CB    1



/* if use any peripherals below, #define it 1 */
#define USE_ADC_DLPS                    1
#define USE_CODEC_DLPS                  0
#define USE_GPIO_DLPS                   1
#define USE_I2C0_DLPS                   0
#define USE_I2C1_DLPS                   0
#define USE_I2S0_DLPS                   0
#define USE_IR_DLPS                     0
#define USE_KEYSCAN_DLPS                0
#define USE_QDECODER_DLPS               0
#define USE_SPI0_DLPS                   0
#define USE_SPI1_DLPS                   0
#define USE_SPI2W_DLPS                  0
#define USE_TIM_DLPS                    0
#define USE_ENHTIM_DLPS                 0
#define USE_UART0_DLPS                  1   /* save/restore UART0 regs across DLPS */
#define USE_UART1_DLPS                  0
#define USE_CTC_DLPS                    0



/* do not modify USE_IO_DRIVER_DLPS macro */
#define USE_IO_DRIVER_DLPS             ( USE_ADC_DLPS     | USE_CODEC_DLPS | USE_GPIO_DLPS  | USE_I2C0_DLPS   \
                                         | USE_I2C1_DLPS    | USE_I2S0_DLPS  | USE_IR_DLPS    | USE_KEYSCAN_DLPS\
                                         | USE_QDECODER_DLPS| USE_SPI0_DLPS  | USE_SPI1_DLPS  | USE_SPI2W_DLPS  \
                                         | USE_TIM_DLPS     | USE_ENHTIM_DLPS| USE_UART0_DLPS | USE_UART1_DLPS  \
                                         | USE_CTC_DLPS\
                                         | USE_USER_DEFINE_DLPS_ENTER_CB\
                                         | USE_USER_DEFINE_DLPS_EXIT_CB)


#ifdef __cplusplus
}
#endif

#endif  /* _BOARD_H_ */
