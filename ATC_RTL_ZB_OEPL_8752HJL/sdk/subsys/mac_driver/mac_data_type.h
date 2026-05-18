/**************************************************************************//**
 * @file     mac_data_type.h
 * @brief    IEEE802.15.4 MAC/PHY driver data structure type definition.
 *
 * @version  V1.00
 * @date     2023-06-12
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2023 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/


#ifndef _MAC_DATA_TYPE_H_
#define _MAC_DATA_TYPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "string.h"
#include "zb_section.h"
#include "mac_port_macro.h"

#define MAC_ENTRY_PRIO_MIN          (-1)

#define MAC_ENTRY_PRIO_NORMAL       (0)

#define MAC_ENTRY_PRIO_CSL          (512)

#define MAC_DEFAULT_CHANNEL         (14)  // the default channel after MAC is initialized
#define MAX_CHANNEL_NUM             (16)  // max supported channel number (CH11 ~ CH26)

/**
  \brief RX Frame type filter reset value, accept Beacon, Data and Command frame type
*/
#define MAC_RXFRMTYPE_RSTVAL        (0x0b)

/**
  \brief RX Frame type filter reset value, accept Enh-Beacon, Enh-Ack, Enh-Data and Enh-Command frame type
*/
#define MAC_RXENHFRMTYPE_RSTVAL     (0x0f)

/**
  \brief Default aMinLIFSPeriod value
*/
#define MAC_MINLIFS_RSTVAL          (0x21)

#define MAC_NONCE_LENGTH            (13)
#define MAC_KEY_LENGTH              (16)

/**
  \brief  MAC Frame parameters related definition
*/
#define MAC_MAX_TX_FRM_LEN          (125)     // Maximum length(byte) of a TX frame
#define MAC_MIN_TX_FRM_LEN          (1)       // Minimum length(byte) of a TX frame

/**
  \brief  FIFO ID
*/
typedef enum mac_fifo_id_e
{
    TXN_FIFO_ID = 0,    /* Normal TX FIFO */
    TXB_FIFO_ID = 1,    /* Beacon TX FIFO */
    TXG1_FIFO_ID = 2,   /* GTS1 TX FIFO */
    TXG2_FIFO_ID = 3,   /* GTS2 TX FIFO */
    KEY_FIFO_ID = 4,    /* Key FIFO */
    RX_FIFO_ID = 5,     /* RX FIFO */

    INVALID_FIFO_ID = 6,    /* Invalid FIFO ID */
} mac_fifo_id_t;

/**
  \brief  Define TX/RX FIFO and Key buffer base address
*/
#ifdef PORT_PLATFORM_BEE3PLUS
uint32_t mac_FIFO_BaseAddr(mac_fifo_id_t fifo_id);
#define MAC_TXN_BASE_ADDR             ((uint32_t)mac_FIFO_BaseAddr(TXN_FIFO_ID))
#define MAC_TXB_BASE_ADDR             ((uint32_t)mac_FIFO_BaseAddr(TXB_FIFO_ID))
#define MAC_TXG1_BASE_ADDR            ((uint32_t)mac_FIFO_BaseAddr(TXG1_FIFO_ID))
#define MAC_TXG2_BASE_ADDR            ((uint32_t)mac_FIFO_BaseAddr(TXG2_FIFO_ID))
#define MAC_KEY_FIFO_BASE_ADDR        ((uint32_t)mac_FIFO_BaseAddr(KEY_FIFO_ID))
#define MAC_RX_BASE_ADDR              ((uint32_t)mac_FIFO_BaseAddr(RX_FIFO_ID))
#else
extern uint32_t mac_FIFO_BaseAddr_rom(mac_fifo_id_t fifo_id);
#define MAC_TXN_BASE_ADDR             ((uint32_t)mac_FIFO_BaseAddr_rom(TXN_FIFO_ID))
#define MAC_TXB_BASE_ADDR             ((uint32_t)mac_FIFO_BaseAddr_rom(TXB_FIFO_ID))
#define MAC_TXG1_BASE_ADDR            ((uint32_t)mac_FIFO_BaseAddr_rom(TXG1_FIFO_ID))
#define MAC_TXG2_BASE_ADDR            ((uint32_t)mac_FIFO_BaseAddr_rom(TXG2_FIFO_ID))
#define MAC_KEY_FIFO_BASE_ADDR        ((uint32_t)mac_FIFO_BaseAddr_rom(KEY_FIFO_ID))
#define MAC_RX_BASE_ADDR              ((uint32_t)mac_FIFO_BaseAddr_rom(RX_FIFO_ID))
#endif

/**
  \brief  Security Key buffer base address
*/
#define KEYBUF_OFFSET_NORMAL        (0)
#define KEYBUF_OFFSET_GTS1          (0x10)
#define KEYBUF_OFFSET_GTS2_BCN      (0x20)
#define KEYBUF_OFFSET_RX            (0x30)

#define MAC_NORMAL_KEY              (MAC_KEY_FIFO_BASE_ADDR + KEYBUF_OFFSET_NORMAL)
#define MAC_GTS1_KEY                (MAC_KEY_FIFO_BASE_ADDR + KEYBUF_OFFSET_GTS1)
#define MAC_GTS2_BCN_KEY            (MAC_KEY_FIFO_BASE_ADDR + KEYBUF_OFFSET_GTS2_BCN)
#define MAC_RX_KEY                  (MAC_KEY_FIFO_BASE_ADDR + KEYBUF_OFFSET_RX)

/**
  \brief  TX Normal FIFO related definition
*/
#define MAC_TX_FIFO_SIZE            (127)

// TODO
#define MAC_TXN_HLEN_IDX            (0x00) //header length offset
#define MAC_TXN_FLEN_IDX            (0x01) //frame length offset
#define MAC_TXN_FRM_IDX             (0x02)

#define MAC_TXN_HLEN                (MAC_TXN_BASE_ADDR + MAC_TXN_HLEN_IDX)
#define MAC_TXN_FLEN                (MAC_TXN_BASE_ADDR + MAC_TXN_FLEN_IDX)
#define MAC_TXN_FRM                 (MAC_TXN_BASE_ADDR + MAC_TXN_FRM_IDX)

/**
  \brief  TX Beacon FIFO related definition
*/
#define MAC_TXB_FIFO_SIZE           (127)

#define MAC_TXB_HLEN_IDX            (0x00) //header length
#define MAC_TXB_FLEN_IDX            (0x01) //frame length
#define MAC_TXB_FRM_IDX             (0x02)

#define MAC_TXB_HLEN                (MAC_TXB_BASE_ADDR + MAC_TXB_HLEN_IDX)
#define MAC_TXB_FLEN                (MAC_TXB_BASE_ADDR + MAC_TXB_FLEN_IDX)
#define MAC_TXB_FRM                 (MAC_TXB_BASE_ADDR + MAC_TXB_FRM_IDX)

/**
  \brief  GTS1 TX FIFO related definition
*/
#define MAC_TXG1_FIFO_SIZE          (127)

#define MAC_TXG1_HLEN_IDX           (0x00) //header length
#define MAC_TXG1_FLEN_IDX           (0x01) //frame length
#define MAC_TXG1_FRM_IDX            (0x02)

#define MAC_TXG1_HLEN               (MAC_TXG1_BASE_ADDR + MAC_TXG1_HLEN_IDX)
#define MAC_TXG1_FLEN               (MAC_TXG1_BASE_ADDR + MAC_TXG1_FLEN_IDX)
#define MAC_TXG1_FRM                (MAC_TXG1_BASE_ADDR + MAC_TXG1_FRM_IDX)

/**
  \brief  GTS2 TX FIFO related definition
*/
#define MAC_TXG2_FIFO_SIZE          (127)

#define MAC_TXG2_HLEN_IDX           (0x00)
#define MAC_TXG2_FLEN_IDX           (0x01)
#define MAC_TXG2_FRM_IDX            (0x02)

#define MAC_TXG2_HLEN               (MAC_TXG2_BASE_ADDR + MAC_TXG2_HLEN_IDX)
#define MAC_TXG2_FLEN               (MAC_TXG2_BASE_ADDR + MAC_TXG2_FLEN_IDX)
#define MAC_TXG2_FRM                (MAC_TXG2_BASE_ADDR + MAC_TXG2_FRM_IDX)

/**
  \brief  RX FIFO related definition
*/
#define MAC_RX_FIFO_SIZE            (144)

#define MAC_RX_FLEN_IDX             (0x00)
#define MAC_RX_FRM_IDX              (0x01)

#define MAC_RX_FLEN                 (MAC_RX_BASE_ADDR + MAC_RX_FLEN_IDX)
#define MAC_RX_FRM                  (MAC_RX_BASE_ADDR + MAC_RX_FRM_IDX)

/**
  \brief  BT Slot Time
*/
#define BT_SLOT_TIME_US             (625)
#define MAX_BT_CLOCK_COUNTER        (0x400000UL * BT_SLOT_TIME_US)  // 22-bits BT slot counter

/**
  \brief  PHY Timing Paramters
*/
#define PHY_SYMBOL_TIME_US          (16)    /* 1 symbol time in us */
#define PHY_CCA_RXON_DELAY_US       (166)   /* CCA RX ON PHY settle down delay in us,
                                               controlled by MAC.phy_trx_dly_b.rxon_delay  */
#define PHY_RXON2CCADT_DELAY_US     (10)    /* RX ON to CCADT delay in us, controlled by
                                               MAC.rx_set1_b.rxon2ccadt_dly */
#define PHY_CCACHKPT_SYMB           (9)     /* CCA check duration takes in Symbol time,
                                               controlled by MAC.txprm_b.ccachkp */
#define PHY_TXON_DELAY_US           (122)   /* MAC init TX to RF TX ready delay time is us,
                                               controlled by MAC.phy_trx_dly_b.txon_delay */
#define PHY_RXON_DELAY_US           (114)   /* MAC init RX to RF RX ready delay time is us,
                                               controlled by MAC.phy_trx_dly_b.rxon_delay */
#define PHY_RX_TO_RX_DELAY_US       (50)    /* RX to RX turnaround time is us, controlled by
                                               MAC.phy_to_dly_b.rx2rx_delay */
#define PHY_TX_TO_RX_DELAY_US       (131)   /* TX to RX turnaround time is us, controlled by
                                               MAC.phy_to_dly_b.tx2rx_delay */
#define PHY_RX_TO_TX_DELAY_US       (120)   /* RX On to TX On delay time is us, controlled by
                                               MAC.phy_to_dly_b.rx2tx_delay */
#define PHY_RX_TO_ED_SCAN_DELAY_US  (114)   /* RX On to ED scan start time is us, controlled by
                                               MAC.rx_set1_b.ed_scan_rxon_delay */
#define PHY_RX_TO_CCADT_DELAY_US    (10)    /* RX On to CCADT state is us, controlled by
                                               MAC.rx_set1_b.rxon_2_ccadt_delay */
#define PHY_RX_TO_OFF_DELAY_US      (80)    /* Frame RX end to RX off delay time is us, controlled by
                                               MAC.rx_set1_b.rxon_off_delay */
#define PHY_HDR_SYMBOL_TIME_US      (PHY_SYMBOL_TIME_US*12)
#define PREAMBLE_TO_RX_TSTAMP_US    (PHY_SYMBOL_TIME_US*11) /* the time offset from the preamble of RX frame to
                                                               the RX Time stamp is latched */
#define PHY_TX_LATENCY_US           (3)     /* MAC TX to RF TX latency */
#define PHY_RX_LATENCY_US           (23)    /* RF RX to MAC RX latency */
#define PHY_GNT_PREPARE_TIME_BTCLK  (3)     /* request a high prio grant request as least 3 btclk */

#define ED_SCAN_INVALID_RET         (128)  /* indicates an invalid ED level return value */
#define ED_SCAN_ERR_MASK            (MAC_ed_scan_ctrl0_ed_scan_encounter_tx_trig_sts_Msk |\
                                     MAC_ed_scan_ctrl0_scan_fw_cancel_sts_Msk |\
                                     MAC_ed_scan_ctrl0_scan_gnt_deassert_sts_Msk |\
                                     MAC_ed_scan_ctrl0_scan_ed_vld_tmo_sts_Msk)
#define ED_SCAN_ERR_RESOURCE        (0x80)

#define BT_CLK_OVFLW_WATCH_TIMER    (3)     /* BT clock timer source for BT Clock counter overflow watching */

#define MAC_INVALID_TIMER_ID        (0xFF)  /* to indicate an invalid Timer ID */
#define MAC_TIMER_HW_SRC            (2)     /* BT clock timer source for MAC software timer */

#define SHORT_SRC_ADDR_MATCH_ENTRY_NUM  (32)  /* define the number of short address match entry of source
                                                 address match filter */
#define EXT_SRC_ADDR_MATCH_ENTRY_NUM    (16)  /* define the number of short address match entry of
                                                 source address match filter */

#define EPOCH_32BIT_US              (1ULL << 32)
#define EPOCH_FROM_TIME(time)       ((time) & ((uint64_t)UINT32_MAX << 32))

#define MILLI_SECOND_PER_8US        (125)
#define GNT_FAILED_RETRY_MAXCNT     (4)

#define PTA_TICK_TIME_US_MAX        (0x1000)  /* max time of a PTA duty control tick, in us */
#define PTA_DUTY_TIME_TICKS_MAX     (0x100)   /* max time of a PTA high/low duty time period, in unit of ticks */

#define MAC_MASK_ALL_INTERRUPT      (0x000FFFFFUL)    /* to mask all MAC interrupt */

typedef uint8_t bool_t;

/**
  \brief TX FIFO data structure
*/
typedef struct mac_tx_fifo_s
{
    uint8_t hdr_len;    /* header length, this length indicating the plan-text data length for a frame security operation */
    uint8_t frm_len;    /* frame length, indicating the total length of PHY payload, CRC is not included */
    uint8_t payload[MAC_MAX_TX_FRM_LEN];  /* PHY payload */
} mac_txfifo_t, *pmac_txfifo_t;

/**
  \brief RX FIFO data structure
*/
typedef struct mac_rxfifo_s
{
    uint8_t frm_len;    /* frame length, indicating the total length of PHY payload, CRC is not included */
    uint8_t payload[MAC_RX_FIFO_SIZE - 1];  /* PHY payload */
} mac_rxfifo_t, *pmac_rxfifo_t;

/**
  \brief TX/RX Time Stamp (BT Native clock[21:1] + Clock Counter) data structure
*/
typedef struct mac_bt_clk_s
{
    uint32_t bt_clk_counter: 10;    /*!< [9..0] BT clock counter in 1 us unit */
    uint32_t bt_native_clk: 22;     /*!< [31..10] BT native clock[21:1] in unit of BT slot */
} mac_bt_clk_t, *pmac_bt_clk_t;

typedef struct __attribute__((packed)) mac_rxfifo_tail_s
{
    uint8_t lqi;    /*!< the Link Quality index of this received frame report from modem */
    uint16_t rssi;  /*!< the radio signal strength index of this received frame report from modem */
    union
    {
        mac_bt_clk_t bt_time;
        uint32_t mac_time;
    };
    uint8_t sf_count[3];  /* Superframe counter */
} mac_rxfifo_tail_t, *pmac_rxfifo_tail_t;

/**
  \brief Security Control of Auxiliary security header data structure
*/
typedef struct aux_security_ctrl_s
{
    uint8_t sec_lv: 3;          /*!< [2..0] Security Level */
    uint8_t key_id_mod: 2;      /*!< [4..3] Key Identifier Mode */
    uint8_t frm_ctr_suprs: 1;   /*!< [5..5] Frame Counter Suppression */
    uint8_t : 2;
} aux_security_ctrl_t, *paux_security_ctrl_t;

/**
  \brief Auxiliary security header data structure
*/
typedef struct __attribute__((packed)) aux_security_hdr_s
{
    union
    {
        uint8_t security_ctrl;    /*!< Security Control */
        aux_security_ctrl_t security_ctrl_b;
    };
    uint32_t frm_ctr;           /*!< Frame Counter */
    uint8_t key_id[9];           /*!< Key Identifier */
} aux_security_hdr_t, *paux_security_hdr_t;

typedef void (*mac_callback_t)(void *parg);

/**
  \brief MAC Software Timer handler structure
*/
typedef struct mac_timer_handle_s
{
    mac_callback_t timer_callback;  /* callback function on timeout event */
    void *callback_arg; /* argument of callback function */
    uint32_t timeout; /* the timestamp (in us format of BT clock) to trigger timeout */
    void *pnext;  /* next timer handle */
    struct
    {
        uint32_t timer_id: 8;    /*!< [7..0] Timer index of this timer */
        uint32_t is_free: 1;    /*!< [8..8] is this timer handle free */
        uint32_t is_start: 1;   /*!< [9..9] is this timer running */
        uint32_t is_timeout: 1; /*!< [10..10] is this timer timeout triggered */
    } timer_ctrl;
} mac_timer_handle_t, *pmac_timer_handle_t;

/**
  \brief flags of RX frame early interrupt processing
*/
typedef struct mac_rx_early_sts_s
{
    uint8_t enh_ack_tx_penging: 1;    /*!< [0..0] enh-ack tx is pending (on GTS FIFO) */
    uint8_t : 7;     /*!< [7..1] reserved */
} mac_rx_early_sts_t, *pmac_rx_early_sts_t;

/**
  \brief  Defines ED Scan State
*/
typedef enum mac_ed_scan_state_e
{
    MAC_EDS_STOPPED = 0x00,
    MAC_EDS_PENDING = 0x01,
    MAC_EDS_RUNNING = 0x02
} mac_ed_scan_state_t;

/**
  \brief  Defines MAC/PHY State
*/
typedef enum mac_radio_state_e
{
    MAC_RADIO_STATE_DISABLED = 0x00,  // disabled (initial)
    MAC_RADIO_STATE_INACTIVE = 0x01,  // not active
    MAC_RADIO_STATE_W4PHYGNT = 0x02,  // waiting for PHY grant
    MAC_RADIO_STATE_RX = 0x03,        // Receiving
    MAC_RADIO_STATE_TX = 0x04,        // Transmitting
    MAC_RADIO_STATE_SCAN = 0x05,      // ED Scanning
} mac_radio_state_t;

/**
  \brief  Defines BT Clock Timer ID
*/
typedef enum mac_bt_timer_id_e
{
    MAC_BT_TIMER0 = 0,
    MAC_BT_TIMER1 = 1,
    MAC_BT_TIMER2 = 2,
    MAC_BT_TIMER3 = 3,
    MAC_BT_TIMER4 = 4,
    MAC_BT_TIMER5 = 5,

    MAC_BT_TIMER_NUM = 6
} mac_bt_timer_id_t;

typedef struct tx_pending_sts_s
{
    uint8_t txn: 1;           /*!< [0..0] TX Normal FIFO trigger pending */
    uint8_t tgs1: 1;          /*!< [1..1] TX GTS1 FIFO trigger pending */
    uint8_t tgs2: 1;          /*!< [2..2] TX GTS2 FIFO trigger pending */
    uint8_t txb: 1;           /*!< [3..3] TX Beacon FIFO trigger pending */
    uint8_t : 4;
} tx_pending_sts_t;

typedef void (*mac_ed_scan_callback_t)(int8_t peak_ed_level, int8_t avrg_ed_level, uint8_t status);
/**
  \brief Energy Scan States structure
*/
typedef struct mac_ed_scan_states_s
{
    uint8_t state;    /* current state of ED scan */
    uint8_t channel;  /* channel for ED scan */
    uint8_t peak_ed_level;  /* the peak ED level of each ED scan round during
                               the whole ED scan process */
    uint8_t scan_round_of_8symb;  /* number of 8-symbols per scan round */
    uint32_t duration_us;   /* duration (unit of us) for ED scan */
    uint32_t ed_level_sum;  /* sum of ED level for each ED scan round */
    uint32_t ed_scan_rounds;  /* ED scan round counter */
    mac_ed_scan_callback_t callback;  /* the callback function for ED scan done */
} mac_ed_scan_states_t, *pmac_ed_scan_states_t;

/**
  \brief  Defines RF transceiver state
*/
enum mac_transceiver_state_e
{
    TRANSCEIVER_TRX_AUTO = 0,
    TRANSCEIVER_RX_ON = 1,
    TRANSCEIVER_TX_ON = 2,
    TRANSCEIVER_TRX_OFF = 3,

    TRANSCEIVER_STATE_MAX = 4
};

/**
  \brief  Defines MAC Security level of version 2006
*/
enum mac_security_level_2006_e
{
    AES_NONE            = 0,
    AES_MIC_32          = 1,
    AES_MIC_64          = 2,
    AES_MIC_128         = 3,
    AES_ENC             = 4,
    AES_ENC_MIC_32      = 5,
    AES_ENC_MIC_64      = 6,
    AES_ENC_MIC_128     = 7,

    MAX_SEC_LEVEL       = 8
};

/**
  \brief  Defines MAC Security level of version 2003
*/
enum mac_security_level_2003_e
{
    AES_CTR             = 1,
    AES_CCM_128         = 2,
    AES_CCM_64          = 3,
    AES_CCM_32          = 4,
    AES_CBC_MAC_128     = 5,
    AES_CBC_MAC_64      = 6,
    AES_CBC_MAC_32      = 7
};

/**
  \brief  Defines MAC function execution result
*/
typedef enum mac_status_e
{
    MAC_STS_SUCCESS = 0x00,
    MAC_STS_FAILURE = 0x01,
    MAC_STS_CHANNEL_BUSY = 0x02,
    MAC_STS_RETRY_EXCEED = 0x03,
    MAC_STS_DATA_LEN_ERROR = 0x04,
    MAC_STS_UNSUPPORT_SECURITY_MODE = 0x05,
    MAC_STS_INVALID_PARAMETER = 0x6,
    MAC_STS_SECURITY_FAILED = 0x7,
    MAC_STS_TEMPORARY_UNAVAILABLE = 0x8,
    MAC_STS_TRANSMISSION_TIME_NOT_ENOUGH = 0x9,
    MAC_STS_FREQUENCY_NOT_SUPPORT = 0xa,
    MAC_STS_PHY_GRANT_FAILED = 0xb,
    MAC_STS_TIMEOUT = 0x0c,
    MAC_STS_CANCELED = 0x0d,
    MAC_STS_PENDING = 0x0e,
    MAC_STS_HW_LIMIT = 0x0f,
    MAC_STS_NOT_SUPPORT = 0x10
} mac_status_t;

/**
  \brief  Defines MAC Power saving mode
*/
typedef enum mac_power_mode_e
{
    MAC_STANDBY_MODE = 0x00,
    MAC_DEEP_SLEEP_MODE = 0x01,
    MAC_POWER_DOWN_MODE = 0x03
} mac_power_mode_t;

/**
  \brief  Defines MAC CCA mode
*/
typedef enum mac_cca_mode_e
{
    MAC_CCA_NONE = 0,
    MAC_CCA_ED = 1,
    MAC_CCA_CS = 2,
    MAC_CCA_CS_ED = 3,

    MAC_CCA_CS_ED_AND = 4,
} mac_cca_mode_t;

/**
  \brief  Defines ZB MAC control RF mode selection
*/
typedef enum mac_rf_mode_e
{
    RF_IQM = 0,
    RF_TPM = 1,

    RF_MODE_NUM
} mac_rf_mode_t;

/**
  \brief  Defines RX Frame Filter
*/
typedef enum mac_rx_frame_filter_e
{
    MAC_RX_DROP_ALL = 0x00,
    MAC_RX_ACCEPT_BEACON = 0x01,
    MAC_RX_ACCEPT_DATA = 0x02,
    MAC_RX_ACCEPT_ACK = 0x04,
    MAC_RX_ACCEPT_COMMAND = 0x08
} mac_rx_frame_filter_t;

/**
  \brief  Defines RX Frame Source Address mode
*/
typedef enum mac_rx_frame_sadr_mode_e
{
    MAC_RX_SADR_MODE_RSV0 = 0,
    MAC_RX_SADR_MODE_RSV1 = 1,
    MAC_RX_SADR_MODE_SHORT = 2,
    MAC_RX_SADR_MODE_LONG = 3
} mac_rx_frame_sadr_mode_t;

/**
  \brief  Defines MAC TX/RX Timestamp timer
*/
typedef enum mac_timestamp_tid_e
{
    TIMER_ID_MAC = 0,    /* the 16-bits MAC free-run downcounter in 8us unit */
    TIMER_ID_BT = 1,     /* the BT clock: 27-bits native clock in 625us unit +
                         10-bits clock counter in 1us unit */
    TIMER_ID_INV = 2     // invalid timestamp timer
} mac_timestamp_tid_t;

/**
  \brief  Defines the mode of auto set pending bit of Imm-Ack
*/
typedef enum mac_auto_ack_pending_bit_mode_e
{
    AUTO_ACK_PENDING_MODE_ZIGBEE = 0,
    AUTO_ACK_PENDING_MODE_THREAD = 1,
} mac_auto_ack_pending_bit_mode_t;

/**
  \brief  Source Address Match entry type
*/
typedef enum mac_src_addr_match_entry_type_e
{
    SRC_MATCH_SHORT = 0,    /* Short address match filter entry type */
    SRC_MATCH_LONG = 1,     /* Long address match filter entry type */
} mac_src_addr_match_entry_type_t;

/**
  \brief  The ACK type for the TXM
*/
typedef enum mac_ack_type_e
{
    MAC_IMM_ACK = 0,    /* Imm-Ack (frame version = 0/1) */
    MAC_ENH_ACK = 1,    /* Enk-Ack (frame version = 2) */
} mac_ack_type_t;

/**
  \brief  Defines MAC Initialize attribute
*/
typedef struct
{
    uint32_t int_mask;
    struct
    {
        uint32_t rx_pingpong_en : 1;        /*!< RX ping-pong FIFO enable control:
                                                 0: disable, 1: enable                */
        uint32_t rf_early_term : 1;         /*!< RX PHY early term enable control:
                                                 0: disable, 1: enable                */
        uint32_t frm06_rx_early : 1;        /*!< RX early interrrupt for frame version
                                                 06 enable control: 0: disable,
                                                 1: enable                            */
        uint32_t rx_timestamp_select : 1;   /*!< RX timestamp timer selection:
                                                 0: MAC timer, 1: BT Native clock     */
        uint32_t cca_mode : 3;              /*!< CCA mode selection:
                                                 00b: disable; 01b: ED mode; 10b:
                                                 CS mode; 11b: ED + CS combination
                                                 mode */
        uint32_t anch_jump_val : 4;         /*!< Anchor point jump for PHY
                                                 arbitration retry                   */
        uint32_t default_channel : 5;       /*!< default channel                     */

        uint32_t reserved0 : 4;
        uint32_t ccachkp : 4;               /*!< The number of symbol for CCA check point */
        uint32_t phy_tx_latency : 5;        /*!< MAC TX to PHY TX latency            */
        uint32_t disdec : 1;                /*!< is disable RX decryption            */
        uint32_t no_backoff_retx : 1;       /*!< is enable no-backoff for TX re-trigger due to
                                                 PHY re-grant  */
        uint32_t reserved1 : 1;
    } mac_cfg;

    struct
    {
        uint32_t tx_rf_mode : 2;            /*!< RF mode for TX     */
        uint32_t rx_rf_mode : 2;            /*!< RF mode for RX     */
        uint32_t lna_en : 1;                /*!< is RX LNA enabled  */
        uint32_t phy_gnt_slot_offset : 5;   /*!< slot number offset from current to Grant PHY */
        uint32_t rx_on_delay_attr_valid : 1;   /*!< is PHY RX On delay attribute setting valid  */
        uint32_t tx_on_delay_attr_valid : 1;   /*!< is PHY RX On delay attribute setting valid  */
        uint32_t rf_fast_chnl_switch_en : 1;   /*!< is RF fast channel switching enabled  */
        uint32_t reserved : 19;
    } phy_cfg;

    uint8_t phy_arbitration_en;             /*!< PHY arbitration enable control:
                                                0: disable; 1: enable                */
    uint8_t cca_ed_threshold;
    uint8_t rx2tx_delay;                    /*!< RX to TX delay for wait RF ready    */
    uint8_t cca_rxon_delay;                 /*!< normal RX to CCA RX delay           */
    uint8_t rx2tx_immack_delay;
    uint8_t txagc;
    uint8_t txon_delay;                     /*!< MAC initial TX to PHY TX ready delay */
    /* MAC driver configuration */
    struct
    {
        uint8_t sw_backup_mac_reg : 1;     /*!< is backup MAC reg by software     */
        uint8_t sw_backup_sa_filter : 1;   /*!< is backup MAC source address match
                                                 filetr by software */
        uint8_t hci_rst_backup_mac : 1;    /*!< is backup whole MAC register by
                                                 software for BT HCI reset */
        uint8_t reserved : 5;
    } driver_cfg;

    /* gain table per channel, in unit of 0.5dBm */
    const int8_t *ptx_gain_tbl;

    uint8_t rx2rx_delay;                    /*!< RX to RX delay for wait RF ready    */
    uint8_t rxon_delay;                     /*!< MAC initial RX to PHY RX ready delay */
    uint8_t tx2rx_delay;                    /*!< TX to RX delay for wait RF ready    */
    uint8_t ed_scan_rxon_delay;             /*!< RX to ED scan RX on delay */
    uint8_t rxon2ccadt_dly;                 /*!< RX to CCADT state delay */
    uint8_t rxon_off_dly;                   /*!< Frame RX end to RX off delay */
} mac_attribute_t;

enum mac_phy_arbitration_priority_e
{
    MAC_PHY_ARBI_LOW_PRIORITY = 0,
    MAC_PHY_ARBI_MID_PRIORITY = 1,
    MAC_PHY_ARBI_HIGH_PRIORITY = 2,
};

#if defined(DUAL_CHNL_EN) && (DUAL_CHNL_EN)
typedef struct zb_phy_2symb_correlation_cfg_s
{
    uint32_t sfd_timeout_2symb_off : 4; /*!< [3..0] timer for waiting sfd detection timeout
                                             when zb_2symbol_sync = 0，0 ~ 8 for 8 ~ 16 symbol */
    uint32_t sfd_timeout_2symb_on : 4;  /*!< [7..4] timer for waiting sfd detection timeout
                                             after 2-preamble-symbol detected when
                                             zb_2symbol_sync = 1，0 ~ 8 for 8 ~ 16 symbol. */
    uint32_t two_sym_sync_timeout : 3; /*!< [10..8] timer for waiting next 2 symbol sync or
                                            SFD, 0 ~ 7 for 1 ~ 8 symbol */
    uint32_t zb_rx_term_1sym_en : 1; /*!< [11..11] 1-preamble symbol correlation enable */
    uint32_t zb_rx_term_1sym_det_time : 4;  /*!< [15..12] extra time for 1-symbol detection
                                                 0~15 for 2us~17us, unit: 1us */
    uint32_t zb_rx_term_2sym_en : 1; /*!< [16..16] 2-preamble symbol correlation enable */
    uint32_t zb_rx_2sym_sync_th_1st : 6; /*!< [22..17] the 1st 2-preamble symbol correlation
                                          threshold */
    uint32_t zb_rx_2sym_sync_th : 7; /*!< [29..23] the 2-preamble symbol correlation threshold */
    uint32_t zb_preamb_hit_mode_sel : 1; /*!< [30..30] preamble hit mode selection */
} zb_phy_2symb_correlation_cfg_t, *pzb_phy_2symb_correlation_cfg_t;

#endif  /* #if defined(DUAL_CHNL_EN) && (DUAL_CHNL_EN) */

#if defined(MULTI_PAN_EN) && (MULTI_PAN_EN != 0)

enum mac_mpan_index_e
{
    MPAN_PAN0 = 0,
    MPAN_PAN1 = 1,
    MPAN_PAN2 = 2,
    MPAN_PAN3 = 3,

    MAX_PAN_NUM = 4
};

enum mac_mpan_chnl_sel_e
{
    PRIMARY_CHNL = 0,
    SECONDARY_CHNL = 1,
};

#define THREAD_PAN      MPAN_PAN0
#define ZB_PAN          MPAN_PAN1

#endif  /* #if defined(MULTI_PAN_EN) && (MULTI_PAN_EN != 0) */

#ifdef __cplusplus
}
#endif

#endif    // end of #ifndef _MAC_DATA_TYPE_H_
