/**
*****************************************************************************************
*     Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file  mac_driver_interface.h
  * @brief  MAC driver interface header file
  * @details
  * @author  yushuo_ko
  * @date  2024-12-13
  * @version v0.1
  */

/*============================================================================*
*               Define to prevent recursive inclusion
*============================================================================*/
#ifndef _MAC_DRIVER_INTERFACE_H_
#define _MAC_DRIVER_INTERFACE_H_

/*============================================================================*
 *                        Header Files
 *============================================================================*/
#include "mac_driver.h"
#include "mac_driver_mpan.h"

/** @defgroup MAC_API IEEE 802.15.4 MAC API
  * @brief IEEE 802.15.4 MAC API.
  * @{
  */

/*============================================================================*
 *                         Functions
 *============================================================================*/
/** @defgroup MAC_API_Exported_Functions IEEE 802.15.4 MAC API Exported Functions
  * @brief MAC Exported Functions.
  * @{
  */

/**
  * @brief Initialize MAC attribute.
  *
  * @param[in,out] attribute The mac_attribute_t pointer to be initialized.
  */
void mac_attribute_init(mac_attribute_t *attribute);

/**
  * @brief Register MAC callback functions.
  *
  * @param[in] intr_cb      Interrupt handler callback function.
  * @param[in] lv2dbm_cb    Translate EDScan level to dbm callback function.
  * @param[in] gnt_entry_cb Set gnt entry priority callback function.
  * @param[in] cca_comb_cb  Modem CCA combine callback function.
  */
void mac_callback_register(mac_intr_callback_t intr_cb,
                           mac_edscan_lv2dbm_callback_t lv2dbm_cb,
                           mac_gnt_entry_callback_t gnt_entry_cb,
                           modem_cca_comb_callback_t cca_comb_cb);

/**
  * @brief Initialize mac layer with attributes.
  *
  * @param[in,out] ptr_drv The mac_driver_t pointer to be initialized, mac_driver_t should be defined in data section.
  * @param[in] attribute   The mac_attribute_t pointer pointing to structure with default value.
  */
void mac_init(mac_driver_t *ptr_drv, mac_attribute_t *attribute);

/**
  * @brief Additional initialize step.
  */
void mac_init_ext(void);

/**
  * @brief Enter MAC layer driver critical section (no MAC interrupt).
  */
void mac_cs_enter(void);

/**
  * @brief Exit MAC layer driver critical section (no MAC interrupt).
  */
void mac_cs_exit(void);

/**
  * @brief Check if MAC clock is enabled.
  *
  * @return The status of MAC clock.
  * @retval 1: MAC clock is enabled.
  * @retval 0: MAC clock is disabled.
  */
uint8_t mac_enabled_check(void);

/**
  * @brief Enable MAC clock.
  */
void mac_enable(void);

/**
  * @brief Disable MAC clock.
  */
void mac_disable(void);

/**
  * @brief Get current BT native clock and clock counter value.
  *
  * @param[out] pbt_clk The pointer of return BT clock data structure.
  */
void mac_btclk_get(mac_bt_clk_t *pbt_clk);

/**
  * @brief Convert a BT clock value to a time value with unit of 1us.
  *
  * @param[in] bt_clk The BT clock value to be converted.
  *
  * @return The converted time value with unit of 1us.
  */
uint32_t mac_btclk_to_us(mac_bt_clk_t bt_clk);

/**
  * @brief Get current BT native clock and clock counter value and convert
  *         it as micro-second.
  *
  * @return Current BT clock counter in micro-second format.
  */
uint32_t mac_btus_get(void);

/**
  * @brief Get current MAC time in us, based on BT clock counter.
  *
  * @return Current MAC time.
  */
uint64_t mac_timestamp_get(void);

/**
  * @brief Setup a BT clock interrupt by a time format in micro-second.
  *
  * @param[in] tid     The ID of BT clock timer to be set.
  * @param[in] time_us The given BT clock value in us format to trigger the interrupt.
  */
void mac_btus_intr_set(uint32_t tid, uint32_t time_us);

/**
  * @brief Copy memory from source to destination.
  *
  * @param[out] dest The destination memory address.
  * @param[in] src   The source memory address.
  * @param[in] len   The length of memory to be copied.
  */
void *mac_memcpy(void *dest, const void *src, uint32_t len);

/**
  * @brief To request PHY grant for MAC TX/RX.
  */
void mac_radio_on(void);

/**
  * @brief To release PHY grant, no TX/RX is allowed after releasing PHY grant.
  */
void mac_radio_off(void);

/**
  * @brief To get current Radio(PHY arbitration) state.
  *
  * @return Current radio state.
  */
mac_radio_state_t mac_radio_state_get(void);

/**
  * @brief This function is used to change RF channel between 11 ~ 26.
  *
  * @param[in] ch Channel number. Value range is 11~26.
  *
  * @return The result of changing channel.
  * @retval MAC_STS_SUCCESS           Set channel successfully.
  * @retval MAC_STS_INVALID_PARAMETER The channel number is out of range.
  */
uint8_t mac_channel_set(uint8_t ch);

/**
  * @brief Read current RF channel
  *
  * @return RF channel, range 11-26.
  */
uint8_t mac_channel_get(void);

/**
  * @brief This function is used to change RF freq between 2402 ~ 2480.
  *
  * @param[in] freq Frequency number. Value range is 2402~2480.
  *
  * @return The result of changing channel.
  * @retval MAC_STS_SUCCESS           Set freq successfully.
  * @retval MAC_STS_INVALID_PARAMETER The freq value is out of range.
  */
uint8_t mac_freq_set(uint16_t  freq);

/**
  * @brief Read current RF freq.
  *
  * @return RF freq, range 2402-2480.
  */
uint16_t mac_freq_get(void);

/**
  * @brief This function is used to set the RF TX power.
  *
  * @param[in] tx_dbm The TX power in 0.5dBm unit.
  *
  * @return The TX gain index for this TX power setting.
  */
uint8_t mac_tx_power_set(int8_t tx_dbm);

/**
  * @brief Get RSSI value from current raw data.
  *
  * @param[in] rssi_raw RSSI raw data from phy layer.
  * @param[in] channel  Channel number. Value range is 11~26.
  *
  * @return RSSI value (dbm).
  */
int8_t mac_rssi_get(uint16_t rssi_raw, uint8_t channel);

/**
  * @brief Set IEEE 802.15.4 destination PAN id.
  *
  * @param[in] pid 16 bits length PAN identifier.
  */
void mac_panid_set(uint16_t pid);

/**
  * @brief Get IEEE 802.15.4 destination PAN id.
  *
  * @return 16 bits length PAN identifier.
  */
uint16_t mac_panid_get(void);

/**
  * @brief Set IEEE 802.15.4 destination network(short) address.
  *
  * @param[in] sadr 16 bits length network(short) address.
  */
void mac_short_addr_set(uint16_t sadr);

/**
  * @brief Get IEEE 802.15.4 destination network(short) address.
  *
  * @return 16 bits length network(short) address.
  */
uint16_t mac_short_addr_get(void);

/**
  * @brief Set IEEE 802.15.4 destination extend(long) address.
  *
  * @param[in] ladr 64 bits extend(long) address pointer.
  */
void mac_long_addr_set(uint8_t  *ladr);

/**
  * @brief Get IEEE 802.15.4 destination extend(long) address.
  *
  * @return 64 bits extend(long) address pointer.
  */
uint8_t *mac_long_addr_get(void);

/**
  * @brief Set the maximum retry count of the Normal FIFO transmission.
  *
  * @param[in] max_retry The maximum retry count.
  *
  * @return Status.
  * @retval MAC_STS_SUCCESS Maximum retry set OK.
  */
uint8_t mac_txn_retry_set(uint8_t max_retry);

/**
  * @brief Get last TxN retry times.
  *
  * @return Last TxN retry times 0-3.
  */
uint8_t mac_txn_retry_get(void);

/**
  * @brief Set RF TX power index.
  *
  * @param[in] tx_gain The TX gain to set.
  */
void mac_tx_gain_set(uint8_t tx_gain);

/**
  * @brief Get current TX gain index setting value.
  *
  * @return Current TX gain control register value.
  */
uint8_t mac_tx_gain_get(void);

/**
  * @brief This function sets the maximum value of BACKOFF parameter of the TX CSMACA algorithm.
  *        The parameter affects only TX normal FIFO.
  *
  * @param[in] MaxCSMABackoffs The max BACKOFF numbers. The value range is 0 ~ 7.
  */
void mac_csma_max_backoffs_set(uint8_t MaxCSMABackoffs);

/**
  * @brief Get the maximum value of BACKOFF parameter of the TX CSMACA algorithm.
  *
  * @return The max BACKOFF numbers. The value range is 0 ~ 7.
  */
uint8_t mac_csma_max_backoffs_get(void);

/**
  * @brief Set the minimum value of Backoff Exponent parameter of the TX CSMACA algorithm.
  *
  * @param[in] macMinBe The min BE numbers. The value range is 0 ~ 3.
  */
void mac_csma_minbe_set(uint8_t macMinBe);

/**
  * @brief Get the minimum value of Backoff Exponent parameter of the TX CSMACA algorithm.
  *
  * @return The min BE numbers. The value range is 0 ~ 3.
  */
uint8_t mac_csma_minbe_get(void);

/**
  * @brief Set the maximum value of Backoff Exponent parameter of the TX CSMACA algorithm.
  *
  * @param[in] macMaxBe The max BE numbers. The value range is 0 ~ 3.
  */
void mac_csma_maxbe_set(uint8_t macMaxBe);

/**
  * @brief Get the maximum value of Backoff Exponent parameter of the TX CSMACA algorithm.
  *
  * @return The max BE numbers. The value range is 0 ~ 3.
  */
uint8_t mac_csma_maxbe_get(void);

/**
  * @brief Get latest TX error status.
  *
  * @return The latest TX error status:
  * @retval bit0: Normal FIFO TX status.
  * @retval bit1: Enh-Ack FIFO TX status.
  * @retval bit2: Reserved.
  * @retval bit3: Enh-Ack TX slot time not enough.
  * @retval bit4: Reserved.
  * @retval bit5: CCA status.
  */
uint32_t mac_txn_status_get(void);

/**
  * @brief Get latest TX at given time error status.
  *
  * @return The latest TX error status:
  * @retval bit0: conflict with another TX process.
  * @retval bit1: conflict with a RX process.
  * @retval bit2: specified TX trigger time is past or it has no enough time to settle PHY.
  * @retval bit3: PHY is not granted at the specified TX trigger time.
  * @retval bit4: PHY is not ready (in RX state) at the specified TX trigger time.
  */
uint32_t mac_txat_status_get(void);

/**
  * @brief Check if the immediate ACK frame pending is set.
  */
uint8_t mac_imm_ack_fp_check(void);

/**
  * @brief Set TxN csma state.
  *
  * @param[in] enable True: TxN csma enabled, False: TxN csma disabled.
  */
void mac_txn_csma_set(bool enable);

/**
  * @brief Get TxN csma state.
  *
  * @return The status of TxN csma.
  * @retval True TxN csma enabled.
  * @retval False TxN csma disabled.
  */
bool mac_txn_csma_get(void);

/**
  * @brief Set the RX frame filter.
  *
  * @param[in] ver     The version of RX frame filter.
  * @param[in] rxftype The type of RX frame filter.
  */
void mac_rx_frm_filter_set(uint8_t ver, uint8_t rxftype);

/**
  * @brief Get the RX frame filter.
  *
  * @param[in] ver The version of RX frame filter.
  *
  * @return The type of RX frame filter.
  */
uint8_t mac_rx_frm_filter_get(uint8_t ver);

/**
  * @brief Set promiscuous mode status.
  *
  * @param[in] en Disable 0/Enable 1.
  */
void mac_promiscuous_set(uint8_t en);

/**
  * @brief Get promiscuous mode status.
  *
  * @return Promiscuous mode status.
  * @retval 0 Disable.
  * @retval 1 Enable.
*/
uint8_t mac_promiscuous_get(void);

/**
  * @brief Set scan mode status.
  *
  * @param[in] en Disable 0/Enable 1.
  */
void mac_scan_mode_set(uint8_t en);

/**
  * @brief Get scan mode status
  *
  * @return Scan mode status.
  * @retval 0 Disable.
  * @retval 1 Enable.
  */
uint8_t mac_scan_mode_get(void);

/**
  * @brief Get Rx frame sequence number.
  *
  * @return Rx frame sequence number.
  */
uint8_t mac_rx_frm_seq_get(void);

/**
  * @brief Check Rx frame with ack request.
  *
  * @return The status of Rx frame with ack request.
  * @retval 0 Disable.
  * @retval 1 Enable.
  */
uint8_t mac_rx_frm_ack_req_get(void);

/**
  * @brief Get Rx frame type.
  *
  * @return Rx frame type.
  * @retval 0x0 Beacon.
  * @retval 0x1 Data.
  * @retval 0x2 Ack.
  * @retval 0x3 MAC command.
  * @retval 0x4 Reserved.
  * @retval 0x5 Multiple.
  * @retval 0x6 Fragment.
  * @retval 0x7 Extended.
  */
uint8_t mac_rx_frm_type_get(void);

/**
  * @brief Get Rx frame version.
  *
  * @return Rx frame version.
  * @retval 0x0 IEEE 802.15.4 2003.
  * @retval 0x1 IEEE 802.15.4 2006.
  * @retval 0x2 IEEE 802.15.4 2015.
  * @retval 0x3 Reserved.
  */
uint8_t mac_rx_frm_version_get(void);

/**
  * @brief Check Rx frame with panid compress.
  *
  * @return The status of Rx frame with panid compress.
  * @retval 0 Disable.
  * @retval 1 Enable.
  */
uint8_t mac_rx_frm_panid_compress_get(void);

/**
  * @brief Check Rx frame with security.
  *
  * @return The status of Rx frame with security.
  * @retval 0 Disable.
  * @retval 1 Enable.
  */
uint8_t mac_rx_frm_sec_en_get(void);

/**
  * @brief Get Rx frame security level.
  *
  * @return Rx frame security level.
  * @retval 0x0 None.
  * @retval 0x1 MIC-32.
  * @retval 0x2 MIC-64.
  * @retval 0x3 MIC-128.
  * @retval 0x4 ENC.
  * @retval 0x5 ENC-MIC-32.
  * @retval 0x6 ENC-MIC-64.
  * @retval 0x7 ENC-MIC-128.
  */
uint8_t mac_rx_frm_sec_level_get(void);

/**
  * @brief Get Rx frame security key id mode.
  *
  * @return Rx frame security key id mode.
  * @retval 0x0 Key is determined implicitly from the originator and recipient(s) of the frame, as indicated in the frame header.
  * @retval 0x1 Key is determined from the Key Index field.
  * @retval 0x2 Key is determined explicitly from the 4-octet Key Source field and the Key Index field.
  * @retval 0x3 Key is determined explicitly from the 8-octet Key Source field and the Key Index field.
  */
uint8_t mac_rx_frm_sec_keyid_mode_get(void);

/**
  * @brief Get Rx frame security key id.
  *
  * @return Security key id.
  */
uint8_t mac_rx_frm_sec_keyid_get(void);

/**
  * @brief Get Rx frame source address mode.
  *
  * @return Source address mode.
  * @retval 0x0 Not present.
  * @retval 0x1 Reserved.
  * @retval 0x2 short.
  * @retval 0x3 extended.
  */
uint8_t mac_rx_frm_src_addr_mode_get(void);

/**
  * @brief Get Rx frame short address.
  *
  * @return Short address.
  */
uint16_t mac_rx_frm_short_addr_get(void);

/**
  * @brief Get Rx frame extended address.
  *
  * @return Extended address.
  *
  */
uint64_t mac_rx_frm_long_addr_get(void);

/**
  * @brief Check Rx frame with security.
  *
  * @return The status of Rx frame with security.
  * @retval 0 Disable.
  * @retval 1 Enable.
  */
uint8_t mac_rx_frm_seq_compress_get(void);

/**
  * @brief Select the CCA mode of CSMA-CA for frame TX.
  *
  * @param[in] cca_mode The selected CCA mode:
  *                     @arg MAC_CCA_NONE(0):       Disable CCA.
  *                     @arg MAC_CCA_ED(1):         Energy Detection mode.
  *                     @arg MAC_CCA_CS(2):         Carrier Sense mode.
  *                     @arg MAC_CCA_CS_ED(3):      CS or ED combination mode.
  *                     @arg MAC_CCA_CS_ED_AND(4):  CS and ED combination mode.
  */
void mac_cca_mode_set(mac_cca_mode_t cca_mode);

/**
  * @brief Get the CCA mode of CSMA-CA for frame TX.
  *
  * @param[in] cca_mode The selected CCA mode.
  *
  * @return The selected CCA mode.
  * @retval MAC_CCA_NONE(0)       Disable CCA.
  * @retval MAC_CCA_ED(1)         Energy Detection mode.
  * @retval MAC_CCA_CS(2)         Carrier Sense mode.
  * @retval MAC_CCA_CS_ED(3)      CS or ED combination mode.
  * @retval MAC_CCA_CS_ED_AND(4)  CS and ED combination mode.
  */
uint8_t mac_cca_mode_get(void);

/**
  * @brief Set CCA ED mode threshold.
  *
  * @param[in] value CCA ED mode threshold.
  */
void mac_cca_ed_threshold_set(uint8_t value);

/**
  * @brief Get CCA ED mode threshold.
  *
  * @return CCA ED mode threshold.
  */
int8_t mac_cca_ed_threshold_get(void);

/**
  * @brief To do an energy detection scan for a given duration on current channel and
  *        report the maximum energy level detected during this scan.
  *
  * @param[in] scan_duration  The duration of this ED scan, in units of us.
  * @param[out] ed_peak_lev   To return the peak energy level of this ED scan.
  * @param[out] ed_avrg_lev   To return the average energy level of this ED scan.
  *
  * @return The function result.
  * @retval MAC_STS_SUCCESS           ED scan successfully.
  * @retval MAC_STS_INVALID_PARAMETER ED level return buffer is NULL.
  * @retval MAC_STS_CANCELED          ED scan is canceled by FW or FW trigger TXN.
  * @retval MAC_STS_PHY_GRANT_FAILED  ED scan is failed due to PHY grant failure.
  * @retval MAC_STS_TIMEOUT           ED scan failed due to waiting for PHY report timeout.
  */
uint8_t mac_ed_scan_poll(uint32_t scan_duration, int8_t *ed_peak_lev,
                         int8_t *ed_avrg_lev);

/**
  * @brief To start a scheduled ED scan (non-blocking ED scan) procedure.
  *
  * @param[in] scan_duration The duration of this ED scan, in units of us.
  * @param[in] callback The callback function for ED scan done event.
  *
  * @return The function result.
  * @retval MAC_STS_SUCCESS               Scheduled ED scan started successfully.
  * @retval MAC_STS_INVALID_PARAMETER     Call this function with invalid arguments.
  * @retval MAC_STS_TEMPORARY_UNAVAILABLE Previous scheduled ED scan process is not finished yet.
  */
uint8_t mac_ed_scan_schedule(uint32_t scan_duration,
                             mac_ed_scan_callback_t callback);

/**
  * @brief To cancel a started scheduled ED scan process.
  */
void mac_ed_scan_cancel(void);

/**
  * @brief Initialize MAC driver software timer handler.
  *
  * @param[in] ptmr_entities The software timer handler entities list.
  * @param[in] tmr_num       Number of software timer handler in the entities list.
  */
void mac_sw_timer_init(mac_timer_handle_t *ptmr_entities,
                       uint8_t tmr_num);

/**
  * @brief Allocate a MAC software handler from the free timer handler pool.
  *
  * @return The allocated timer handler.
  *
  */
pmac_timer_handle_t mac_sw_timer_alloc(void);

/**
  * @brief Free a MAC timer handler.
  *
  * @param[in] pmac_tmr The MAC timer handler to be freed.
  */
void mac_sw_timer_free(pmac_timer_handle_t pmac_tmr);

/**
  * @brief Start a MAC timer with a given timestamp (BT clock) of timeout event to be triggered.
  *
  * @param[in] pmac_tmr  The MAC timer handler to be started.
  * @param[in] timeout   The timestamp (based on BT clock) to trigger timeout.
  * @param[in] pcallback The callback function for timeout event which will be called in timer
  *                      interrupt handler.
  * @param[in] arg       The argument of the timeout callback function.
  */
void mac_sw_timer_start(pmac_timer_handle_t pstart_tmr, uint32_t timeout,
                        void *pcallback, void *arg);

/**
  * @brief To stop a running MAC timer.
  *
  * @param[in] pstop_tmr The MAC timer to be stopped.
  */
void mac_sw_timer_stop(pmac_timer_handle_t pstop_tmr);

/**
  * @brief Load frame into Tx normal fifo.
  *
  * @param[in] HdrL   The header length of the frame.
  * @param[in] FrmL   The frame length of the frame.
  * @param[in] TxFIFO The tx fifo structure which user wants to transmit.
  *
  * <table style="width:50%; text-align:center; border-collapse:collapse; margin:left;">
  * <tr>
  *   <th style="border: 1px solid black; text-align:center;">1 byte</th>
  *   <th style="border: 1px solid black; text-align:center;">1 bytes</th>
  *   <th style="border: 1px solid black; text-align:center;">125 byte</th>
  * </tr>
  * <tr>
  *   <td style="border: 1px solid black; text-align:center;">HDR LEN</td>
  *   <td style="border: 1px solid black; text-align:center;">FRM LEN</td>
  *   <td style="border: 1px solid black; text-align:center;">USER DATA</td>
  * </tr>
  * </table>
  *
  * @return The process status.
  * @retval MAC_STS_SUCCESS        The TX transmission is successful.
  * @retval MAC_STS_DATA_LEN_ERROR The frame length is larger than 125 bytes.
  */
uint8_t mac_txn_payload_set(uint8_t HdrL, uint8_t FrmL, uint8_t *TxFIFO);

/**
  * @brief Set if require ack, security process, then trigger Tx normal FIFO.
  *
  * @param[in] ackreq Require Tx waiting an ack frame.
  * @param[in] secreq Trigger Tx outgoing security process.
  *
  * @return Status.
  * @retval MAC_STS_SUCCESS TxN is triggered.
  */
uint8_t mac_txn_trig(uint8_t ackreq, uint8_t secreq);

/**
  * @brief Set if TxN no CRC.
  *
  * @param[in] en 0: CRC is added to the frame, 1: CRC is not added to the frame.
  */
void mac_txn_nocrc_set(uint8_t en);

/**
  * @brief Get if TxN no CRC.
  *
  * @return 0 The status of TxN on CRC.
  * @retval 0 CRC is added to the frame.
  * @retval 1 CRC is not added to the frame.
  */
uint8_t mac_txn_nocrc_get(void);

/**
  * @brief Get the TxN timestamp.
  *
  * @return The TxN timestamp.
  */
uint32_t mac_txn_timestamp_get(void);

/**
  * @brief Set if require ack, then enable normal FIFO TX with a specified
  *        trigger time.
  *
  * @param[in] ackreq Require Tx waiting an ack frame.
  * @param[in] docca  Require to do CCA before TX.
  * @param[in] txtime The time to trigger Tx.
  *
  * @return Status.
  * @retval MAC_STS_SUCCESS                      TxN is triggered.
  * @retval MAC_STS_TRANSMISSION_TIME_NOT_ENOUGH The specified TXN trigger time is past.
  */
uint8_t mac_txn_trig_at_time(uint8_t ackreq, uint8_t secreq, uint8_t docca,
                             mac_bt_clk_t txtime);

/**
  * @brief Set the pending enhanced ack frame length.
  *
  * @param[in] len Pending enhanced ack frame length.
  */
void mac_tx_enh_ack_set_pending(uint8_t len);

/**
  * @brief Get the pending enhanced ack frame length.
  *
  * @return Pending enhanced ack frame length.
  */
uint8_t mac_tx_enh_ack_get_pending(void);

/**
  * @brief Load frame into Tx Enh-ACK fifo.
  *
  * @param[in] HdrL   The header length of the frame.
  * @param[in] FrmL   The frame length of the frame.
  * @param[in] TxFIFO The tx fifo structure which user wants to transmit.
  *
  * <table style="width:50%; text-align:center; border-collapse:collapse; margin:left;">
  * <tr>
  *   <th style="border: 1px solid black; text-align:center;">1 byte</th>
  *   <th style="border: 1px solid black; text-align:center;">1 bytes</th>
  *   <th style="border: 1px solid black; text-align:center;">125 byte</th>
  * </tr>
  * <tr>
  *   <td style="border: 1px solid black; text-align:center;">HDR LEN</td>
  *   <td style="border: 1px solid black; text-align:center;">FRM LEN</td>
  *   <td style="border: 1px solid black; text-align:center;">USER DATA</td>
  * </tr>
  * </table>
  *
  * @return The process status.
  * @retval MAC_STS_SUCCESS        The TX transmission is successful.
  * @retval MAC_STS_DATA_LEN_ERROR The frame length is larger than 125 bytes.
  */
uint8_t mac_tx_enh_ack_payload_set(uint8_t HdrL, uint8_t FrmL, uint8_t *TxFIFO);

/**
  * @brief Set if require ack, security process, then trigger Tx normal FIFO.
  *
  * @param[in] early  Trigger Tx enhack in rx early interrupt.
  * @param[in] secreq Trigger Tx enhack outgoing security process.
  *
  * @return Status
  * @retval MAC_STS_SUCCESS Tx enhack is triggered.
  */
uint8_t mac_tx_enh_ack_trig(uint8_t early, uint8_t secreq);

/**
  * @brief Get the Tx enhack state.
  *
  * @return The Tx enhack state.
  */
uint8_t mac_tx_enh_ack_state_get(void);

/**
  * @brief Trigger upper layer encrypt process.
  */
void mac_upper_enc_trig(void);

/**
  * @brief Trigger upper layer decrypt process.
  */
void mac_upper_dec_trig(void);

/**
  * @brief Read received packet from RX FIFO.
  *
  * @param[out] rxfifo User data buffer pointer. The packet length will be put in the start address of buffer
  *
  * <table style="width:50%; text-align:center; border-collapse:collapse; margin:left;">
  * <tr>
  *   <th style="border: 1px solid black; text-align:center;">1 byte</th>
  *   <th style="border: 1px solid black; text-align:center;">125 bytes</th>
  *   <th style="border: 1px solid black; text-align:center;">1 byte</th>
  *   <th style="border: 1px solid black; text-align:center;">2 bytes</th>
  *   <th style="border: 1px solid black; text-align:center;">4/2 bytes</th>
  *   <th style="border: 1px solid black; text-align:center;">3 bytes</th>
  * </tr>
  * <tr>
  *   <td style="border: 1px solid black; text-align:center;">FRM LEN</td>
  *   <td style="border: 1px solid black; text-align:center;">USER DATA</td>
  *   <td style="border: 1px solid black; text-align:center;">LQI</td>
  *   <td style="border: 1px solid black; text-align:center;">RSSI</td>
  *   <td style="border: 1px solid black; text-align:center;">TimeStamp</td>
  *   <td style="border: 1px solid black; text-align:center;">SF Count</td>
  * </tr>
  * </table>
  *
  * @return Rx status.
  * @retval MAC_STS_FAILURE There is no received packet.
  * @retval MAC_STS_SUCCESS Retrieved one packet.
  */
uint8_t mac_rx(uint8_t *rxfifo);

/**
  * @brief Set Rx CRC error packet status.
  *
  * @param[in] en Disable 0/Enable 1.
  */
void mac_rx_err_pkt_set(uint8_t en);

/**
  * @brief Get Rx CRC error packet status.
  *
  * @return Rx CRC error packet status.
  * @retval 0 Disable.
  * @retval 1 Enable.
  */
uint8_t mac_rx_err_pkt_get(void);

/**
  * @brief Config the mode of auto set frame pending bit for Ack frame.
  *
  * @param[in] mode @arg AUTO_ACK_PENDING_MODE_ZIGBEE: Set the pending bit as IEEE802.15.4 Spec.
  *                 @arg AUTO_ACK_PENDING_MODE_THREAD: Set the pending bit as Thread Spec.
  */
void mac_addr_match_mode_set(uint8_t mode);

/**
  * @brief Get the mode of auto set frame pending bit for Ack frame.
  *
  * @return The mode of auto set frame pending bit for ACK frame.
  * @retval AUTO_ACK_PENDING_MODE_ZIGBEE Set the pending bit as IEEE802.15.4 Spec.
  * @retval AUTO_ACK_PENDING_MODE_THREAD Set the pending bit as Thread Spec.
  */
uint8_t mac_addr_match_mode_get(void);

/**
  * @brief To add a short address and panid pair to an entry of Source Address Match Filter.
  *
  * @param[in] short_addr The short address to be added into the Source Address Match Filter.
  * @param[in] panid      The panid to be added into the Source Address Match Filter.
  *
  * @return The function result.
  * @retval MAC_STS_SUCCESS  Add a new short address entry successfully.
  * @retval MAC_STS_HW_LIMIT No free address match entry.
  *
  */
uint8_t mac_addr_match_short_add(uint16_t short_addr, uint16_t panid);

/**
  * @brief To disable a short address match entry by a given short address and panid pair.
  *
  * @param[in] short_addr The short address of the short address match entry to be disabled.
  * @param[in] panid      The panidof of the short address match entry to be disabled.
  *
  * @return The function result.
  * @retval MAC_STS_SUCCESS Disable a short address entry successfully.
  * @retval MAC_STS_FAILURE The short address match entry is not found.
  */
uint8_t mac_addr_match_short_del(uint16_t short_addr, uint16_t panid);

/**
  * @brief To disable all short address match entries.
  */
void mac_addr_match_short_flush(void);

/**
  * @brief To add an extended address match entry of Source Address Match Filter.
  *
  * @param[in] pext_addr The extended address to be added into the Source Address Match Filter.
  *
  * @return The function result.
  * @retval MAC_STS_SUCCESS  Add a new short address entry successfully.
  * @retval MAC_STS_HW_LIMIT No free address match entry.
  */
uint8_t mac_addr_match_long_add(uint8_t *pext_addr);

/**
  * @brief To disable an extended address match entry by a given extended address.
  *
  * @param[in] pext_addr The extended address of the extended address match entry to be disabled.
  *
  * @return The function result.
  * @retval MAC_STS_SUCCESS Disable a short address entry successfully.
  * @retval MAC_STS_FAILURE The short address match entry is not found.
  */
uint8_t mac_addr_match_long_del(uint8_t *pext_addr);

/**
  * @brief To disable all extended address match entries.
  */
void mac_addr_match_long_flush(void);

/**
  * @brief Get the source address match entry of Source Address Match Filter.
  *
  * @param[in] index The index of the source address match entry.
  *
  * @param[in] high_bytes @arg True: get the high bytes of the source address match entry.
  *                       @arg False: get the low bytes of the source address match entry.
  *
  * @return The source address match entry.
  */
uint32_t mac_addr_match_entry_get(uint8_t index, bool high_bytes);

/**
  * @brief Load nonce into MAC.
  *
  * @param[in] nonce The nonce structure to be loaded.
  */
void mac_nonce_set(uint8_t *nonce);

/**
  * @brief Load key into TxN key fifo.
  *
  * @param[in] key The TxN key to be loaded.
  */
void mac_txn_key_set(uint8_t *key);

/**
  * @brief Config the security level of Normal FIFO cipher.
  *
  * @param[in] level The security level(as the definition in IEEE 802.15.4 MAC Spec).
  */
void mac_txn_cipher_set(uint8_t level);

/**
  * @brief Load key into TxG1 key fifo.
  *
  * @param[in] key The TxG1 key to be loaded.
  */
void mac_tx_enh_ack_key_set(uint8_t *key);

/**
  * @brief Config the security level of Enh-Ack FIFO cipher.
  *
  * @param[in] level The security level(as the definition in IEEE 802.15.4 MAC Spec).
  */
void mac_tx_enh_ack_cipher_set(uint8_t level);


/** @} */ /* End of group MAC_API_Exported_Functions */
/** @} */ /* End of group MAC_API */

#endif
