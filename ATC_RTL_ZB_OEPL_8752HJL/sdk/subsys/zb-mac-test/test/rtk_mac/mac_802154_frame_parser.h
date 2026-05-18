/*
 * Copyright (c) 2024 Realtek Semiconductor Corp.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MAC_802154_FRAME_PARSER_H
#define MAC_802154_FRAME_PARSER_H

#include <stdbool.h>
#include <stdint.h>

#define ACK_HEADER_WITH_PENDING      0x12                                         ///< The first byte of an ACK frame containing a pending bit.
#define ACK_HEADER_WITHOUT_PENDING   0x02                                         ///< The first byte of an ACK frame without a pending bit.

#define ACK_REQUEST_OFFSET           1                                            ///< Byte containing the ACK request bit (+1 for the frame length byte).
#define ACK_REQUEST_BIT              (1 << 5)                                     ///< ACK request bit.

#define DEST_ADDR_TYPE_OFFSET        2                                            ///< Byte containing the destination address type (+1 for the frame length byte).
#define DEST_ADDR_TYPE_MASK          0x0c                                         ///< Mask of bits containing the destination address type.
#define DEST_ADDR_TYPE_EXTENDED      0x0c                                         ///< Bits containing the extended destination address type.
#define DEST_ADDR_TYPE_NONE          0x00                                         ///< Bits containing a not-present destination address type.
#define DEST_ADDR_TYPE_SHORT         0x08                                         ///< Bits containing the short destination address type.
#define DEST_ADDR_OFFSET             6                                            ///< Offset of the destination address in the Data frame (+1 for the frame length byte).

#define DSN_OFFSET                   3                                            ///< Byte containing the DSN value (+1 for the frame length byte).
#define DSN_SUPPRESS_OFFSET          2                                            ///< Byte containing the DSN suppression field.
#define DSN_SUPPRESS_BIT             0x01                                         ///< Bits containing the DSN suppression field.

#define FRAME_COUNTER_SUPPRESS_BIT   0x20                                         ///< Bit containing the Frame Counter Suppression field.

#define FRAME_PENDING_OFFSET         1                                            ///< Byte containing a pending bit (+1 for the frame length byte).
#define FRAME_PENDING_BIT            (1 << 4)                                     ///< Pending bit.

#define FRAME_TYPE_OFFSET            1                                            ///< Byte containing the frame type bits (+1 for the frame length byte).
#define FRAME_TYPE_MASK              0x07                                         ///< Mask of bits containing the frame type.
#define FRAME_TYPE_ACK               0x02                                         ///< Bits containing the ACK frame type.
#define FRAME_TYPE_BEACON            0x00                                         ///< Bits containing the Beacon frame type.
#define FRAME_TYPE_COMMAND           0x03                                         ///< Bits containing the Command frame type.
#define FRAME_TYPE_DATA              0x01                                         ///< Bits containing the Data frame type.
#define FRAME_TYPE_EXTENDED          0x07                                         ///< Bits containing the Extended frame type.
#define FRAME_TYPE_FRAGMENT          0x06                                         ///< Bits containing the Fragment or the Frak frame type.
#define FRAME_TYPE_MULTIPURPOSE      0x05                                         ///< Bits containing the Multipurpose frame type.

#define FRAME_VERSION_OFFSET         2                                            ///< Byte containing the frame version bits (+1 for the frame length byte).
#define FRAME_VERSION_MASK           0x30                                         ///< Mask of bits containing the frame version.
#define FRAME_VERSION_0              0x00                                         ///< Bits containing the frame version 0b00.
#define FRAME_VERSION_1              0x10                                         ///< Bits containing the frame version 0b01.
#define FRAME_VERSION_2              0x20                                         ///< Bits containing the frame version 0b10.
#define FRAME_VERSION_3              0x30                                         ///< Bits containing the frame version 0b11.

#define IE_HEADER_LENGTH_MASK        0x3f                                         ///< Mask of bits containing the length of an IE header content.
#define IE_PRESENT_OFFSET            2                                            ///< Byte containing the IE Present bit.
#define IE_PRESENT_BIT               0x02                                         ///< Bits containing the IE Present field.

#define KEY_ID_MODE_MASK             0x18                                         ///< Mask of bits containing Key Identifier Mode in the Security Control field.
#define KEY_ID_MODE_0                0                                            ///< Bits containing the 0x00 Key Identifier Mode.
#define KEY_ID_MODE_1                0x08                                         ///< Bits containing the 0x01 Key Identifier Mode.
#define KEY_ID_MODE_2                0x10                                         ///< Bits containing the 0x10 Key Identifier Mode.
#define KEY_ID_MODE_3                0x18                                         ///< Bits containing the 0x11 Key Identifier Mode.

#define MAC_CMD_ASSOC_REQ            0x01                                         ///< Command frame identifier for MAC Association request.
#define MAC_CMD_ASSOC_RESP           0x02                                         ///< Command frame identifier for MAC Association response.
#define MAC_CMD_DISASSOC_NOTIFY      0x03                                         ///< Command frame identifier for MAC Disaccociation notification.
#define MAC_CMD_DATA_REQ             0x04                                         ///< Command frame identifier for MAC Data Requests.
#define MAC_CMD_PANID_CONFLICT       0x05                                         ///< Command frame identifier for MAC PAN ID conflict notification.
#define MAC_CMD_ORPHAN_NOTIFY        0x06                                         ///< Command frame identifier for MAC Orphan notification.
#define MAC_CMD_BEACON_REQ           0x07                                         ///< Command frame identifier for MAC Beacon.
#define MAC_CMD_COORD_REALIGN        0x08                                         ///< Command frame identifier for MAC Coordinator realignment.
#define MAC_CMD_GTS_REQUEST          0x09                                         ///< Command frame identifier for MAC GTS request.

#define PAN_ID_COMPR_OFFSET          1                                            ///< Byte containing the PAN ID compression bit (+1 for the frame length byte).
#define PAN_ID_COMPR_MASK            0x40                                         ///< PAN ID compression bit.

#define PAN_ID_OFFSET                4                                            ///< Offset of PAN ID in the Data frame (+1 for the frame length byte).

#define PHR_OFFSET                   0                                            ///< Offset of the PHY header in a frame.

#define SECURITY_ENABLED_OFFSET      1                                            ///< Byte containing the Security Enabled bit.
#define SECURITY_ENABLED_BIT         0x08                                         ///< Bits containing the Security Enabled field.
#define SECURITY_LEVEL_MASK          0x07                                         ///< Mask of bits containing the Security level field.
#define SECURITY_LEVEL_MIC_32        0x01                                         ///< Bits containing the 32-bit Message Integrity Code (0b001).
#define SECURITY_LEVEL_MIC_64        0x02                                         ///< Bits containing the 64-bit Message Integrity Code (0b010).
#define SECURITY_LEVEL_MIC_128       0x03                                         ///< Bits containing the 128-bit Message Integrity Code (0b011).
#define SECURITY_LEVEL_ENC_MIC_32    0x05                                         ///< Bits containing the 32-bit Encrypted Message Integrity Code (0b101).
#define SECURITY_LEVEL_ENC_MIC_64    0x06                                         ///< Bits containing the 64-bit Encrypted Message Integrity Code (0b110).
#define SECURITY_LEVEL_ENC_MIC_128   0x07                                         ///< Bits containing the 128-bit Encrypted Message Integrity Code (0b111).

#define SRC_ADDR_TYPE_EXTENDED       0xc0                                         ///< Bits containing the extended source address type.
#define SRC_ADDR_TYPE_NONE           0x00                                         ///< Bits containing a not-present source address type.
#define SRC_ADDR_TYPE_MASK           0xc0                                         ///< Mask of bits containing the source address type.
#define SRC_ADDR_TYPE_OFFSET         2                                            ///< Byte containing the source address type (+1 for the frame length byte).
#define SRC_ADDR_TYPE_SHORT          0x80                                         ///< Bits containing the short source address type.

#define SRC_ADDR_OFFSET_SHORT_DST    8                                            ///< Offset of the source address in the Data frame if the destination address is short.
#define SRC_ADDR_OFFSET_EXTENDED_DST 14                                           ///< Offset of the source address in the Data frame if the destination address is extended.

#define DSN_SIZE                     1                                            ///< Size of the Sequence Number field.
#define FCF_SIZE                     2                                            ///< Size of the FCF field.
#define FCS_SIZE                     2                                            ///< Size of the FCS field.
#define FRAME_COUNTER_SIZE           4                                            ///< Size of the Frame Counter field.
#define IE_HEADER_SIZE               4                                            ///< Size of the obligatory IE Header field elements, including the header termination.
#define IMM_ACK_LENGTH               5                                            ///< Length of the ACK frame.
#define KEY_ID_MODE_1_SIZE           1                                            ///< Size of the 0x01 Key Identifier Mode field.
#define KEY_ID_MODE_2_SIZE           5                                            ///< Size of the 0x10 Key Identifier Mode field.
#define KEY_ID_MODE_3_SIZE           9                                            ///< Size of the 0x11 Key Identifier Mode field.
#define MAX_PACKET_SIZE              127                                          ///< Maximum size of the radio packet.
#define MIC_32_SIZE                  4                                            ///< Size of MIC with the MIC-32 and ENC-MIC-32 security attributes.
#define MIC_64_SIZE                  8                                            ///< Size of MIC with the MIC-64 and ENC-MIC-64 security attributes.
#define MIC_128_SIZE                 16                                           ///< Size of MIC with the MIC-128 and ENC-MIC-128 security attributes.
#define PAN_ID_SIZE                  2                                            ///< Size of the PAN ID.
#define PHR_SIZE                     1                                            ///< Size of the PHR field.
#define SECURITY_CONTROL_SIZE        1                                            ///< Size of the Security Control field.

#define EXTENDED_ADDRESS_SIZE        8                                            ///< Size of the Extended Mac Address.
#define SHORT_ADDRESS_SIZE           2                                            ///< Size of the Short Mac Address.

#define TURNAROUND_TIME              192UL                                        ///< RX-to-TX or TX-to-RX turnaround time (aTurnaroundTime), in microseconds (us).
#define CCA_TIME                     128UL                                        ///< Time required to perform CCA detection (aCcaTime), in microseconds (us).
#define UNIT_BACKOFF_PERIOD          (TURNAROUND_TIME + CCA_TIME)                 ///< Number of symbols in the basic time period used by CSMA-CA algorithm (aUnitBackoffPeriod), in (us).

#define PHY_US_PER_SYMBOL            16                                           ///< Duration of a single symbol in microseconds (us).
#define PHY_SYMBOLS_PER_OCTET        2                                            ///< Number of symbols in a single byte (octet).
#define PHY_SHR_SYMBOLS              10                                           ///< Number of symbols in the Synchronization Header (SHR).

#define ED_RESULT_MAX                0xff                                         ///< Maximal ED result.

#define BROADCAST_ADDRESS            ((uint8_t[SHORT_ADDRESS_SIZE]) {0xff, 0xff}) ///< Broadcast short address.

#define MIN_SIFS_PERIOD_US           192                                          ///< Minimum Short IFS period default value in us.
#define MIN_LIFS_PERIOD_US           640                                          ///< Minimum Long IFS period default value in us.
#define MAX_SIFS_FRAME_SIZE          18                                           ///< Maximum frame length which can be followed by the Short Interframe Space.

#define MAC_802154_FRAME_PARSER_INVALID_OFFSET 0xff

/**
 * @brief Structure that contains pointers to parts of MHR and details of MHR structure.
 */
typedef struct
{
    const uint8_t
    *p_dst_panid;            ///< Pointer to the destination PAN ID field, or NULL if missing.
    const uint8_t
    *p_dst_addr;             ///< Pointer to the destination address field, or NULL if missing.
    const uint8_t *p_src_panid;            ///< Pointer to the source PAN ID field, or NULL if missing.
    const uint8_t *p_src_addr;             ///< Pointer to the source address field, or NULL if missing.
    const uint8_t
    *p_sec_ctrl;             ///< Pointer to the security control field, or NULL if missing.
    uint8_t         dst_addr_size;         ///< Size of the destination address field.
    uint8_t         src_addr_size;         ///< Size of the source address field.
    uint8_t         addressing_end_offset; ///< Offset of the first byte following addressing fields.
} mac_802154_frame_parser_mhr_data_t;

/**
 * @brief Determines if the destination address is extended.
 *
 * @param[in]   p_frame   Pointer to a frame to be checked.
 *
 * @retval  true   Destination address is extended.
 * @retval  false  Destination address is not extended.
 */
bool mac_802154_frame_parser_dst_addr_is_extended(const uint8_t *p_frame);

/**
 * @brief Gets the destination address from the provided frame.
 *
 * @param[in]   p_frame             Pointer to a frame.
 * @param[out]  p_dst_addr_extended Pointer to a value, which is true if the destination address
 *                                  is extended. Otherwise, it is false.
 *
 * @returns  Pointer to the first byte of the destination address in @p p_frame.
 *           NULL if the destination address cannot be retrieved.
 */
const uint8_t *mac_802154_frame_parser_dst_addr_get(const uint8_t *p_frame,
                                                    bool           *p_dst_addr_extended);

/**
 * @brief Gets the offset of the destination address field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the destination address field, including one byte
 *           of the frame length.
 * @returns  Zero if the destination address cannot be retrieved.
 *
 */
uint8_t mac_802154_frame_parser_dst_addr_offset_get(const uint8_t *p_frame);

/**
 * @brief Gets the destination PAN ID from the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Pointer to the first byte of the destination PAN ID in @p p_frame.
 * @returns  NULL if the destination PAN ID cannot be retrieved.
 *
 */
const uint8_t *mac_802154_frame_parser_dst_panid_get(const uint8_t *p_frame);

/**
 * @brief Gets the offset of the destination PAN ID field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the destination PAN ID field, including one byte
 *           of the frame length.
 * @returns  Zero in case the destination PAN ID cannot be retrieved.
 *
 */
uint8_t mac_802154_frame_parser_dst_panid_offset_get(const uint8_t *p_frame);

/**
 * @brief Gets the offset of the end of the destination address fields.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset of the first byte following the destination addressing fields in the MHR.
 */
uint8_t mac_802154_frame_parser_dst_addr_end_offset_get(const uint8_t *p_frame);

/**
 * @brief Determines if the source address is extended.
 *
 * @param[in]   p_frame   Pointer to a frame to check.
 *
 * @retval  true   The source address is extended.
 * @retval  false  The source address is not extended.
 *
 */
bool mac_802154_frame_parser_src_addr_is_extended(const uint8_t *p_frame);

/**
 * @brief Determines if the source address is short.
 *
 * @param[in]   p_frame   Pointer to a frame to check.
 *
 * @retval  true   The source address is short.
 * @retval  false  The source address is not short.
 *
 */
bool mac_802154_frame_parser_src_addr_is_short(const uint8_t *p_frame);

/**
 * @brief Gets the source address from the provided frame.
 *
 * @param[in]   p_frame             Pointer to a frame.
 * @param[out]  p_src_addr_extended Pointer to a value, which is true if source address is extended.
 *                                  Otherwise, it is false.
 *
 * @returns  Pointer to the first byte of the source address in @p p_frame.
 * @returns  NULL if the source address cannot be retrieved.
 *
 */
const uint8_t *mac_802154_frame_parser_src_addr_get(const uint8_t *p_frame,
                                                    bool           *p_src_addr_extended);

/**
 * @brief Gets the offset of the source address field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the source address field, including one byte of the frame length.
 * @returns  Zero if the source address cannot be retrieved.
 *
 */
uint8_t mac_802154_frame_parser_src_addr_offset_get(const uint8_t *p_frame);

/**
 * @brief Gets the source PAN ID from the provided frame.
 *
 * @param[in]   p_frame   Pointer to a frame.
 *
 * @returns  Pointer to the first byte of the source PAN ID in @p p_frame.
 * @returns  NULL if the source PAN ID cannot be retrieved or if it is compressed.
 *
 */
const uint8_t *mac_802154_frame_parser_src_panid_get(const uint8_t *p_frame);

/**
 * @brief Gets the offset of the source PAN ID field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the source PAN ID field, including one byte of the frame length.
 * @returns  Zero if the source PAN ID cannot be retrieved or is compressed.
 *
 */
uint8_t mac_802154_frame_parser_src_panid_offset_get(const uint8_t *p_frame);

/**
 * @brief Gets the pointer and the details of MHR parts of a given frame.
 *
 * @param[in]  p_frame   Pointer to a frame to parse.
 * @param[out] p_fields  Pointer to a structure that contains pointers and details
 *                       of the parsed frame.
 *
 * @retval true   Frame parsed correctly.
 * @retval false  Parse error. @p p_fields values are invalid.
 *
 */
bool mac_802154_frame_parser_mhr_parse(const uint8_t                       *p_frame,
                                       mac_802154_frame_parser_mhr_data_t *p_fields);

/**
 * @brief Gets the security control field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Pointer to the first byte of the security control field in @p p_frame.
 * @returns  NULL if the security control cannot be retrieved (that is, security is not enabled).
 *
 */
const uint8_t *mac_802154_frame_parser_sec_ctrl_get(const uint8_t *p_frame);

/**
 * @brief Gets the offset of the first byte after the addressing fields in MHR.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the first byte after the addressing fields in MHR.
 */
uint8_t mac_802154_frame_parser_addressing_end_offset_get(const uint8_t *p_frame);

/**
 * @brief Gets the offset of the security control field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the security control field, including one byte of the frame length.
 * @returns  Zero if the security control cannot be retrieved (that is, security is not enabled).
 *
 */
uint8_t mac_802154_frame_parser_sec_ctrl_offset_get(const uint8_t *p_frame);

/**
 * @brief Gets the key identifier field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Pointer to the first byte of the key identifier field in @p p_frame.
 * @returns  NULL if the key identifier cannot be retrieved (that is, security is not enabled).
 *
 */
const uint8_t *mac_802154_frame_parser_key_id_get(const uint8_t *p_frame);

/**
 * @brief Gets the offset of the key identifier field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the key identifier field, including one byte of the frame length.
 * @returns  Zero if the key identifier cannot be retrieved (that is, security is not enabled).
 *
 */
uint8_t mac_802154_frame_parser_key_id_offset_get(const uint8_t *p_frame);

/**
 * @brief Determines if the sequence number suppression bit is set.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @retval  true   Sequence number suppression bit is set.
 * @retval  false  Sequence number suppression bit is not set.
 *
 */
bool mac_802154_frame_parser_dsn_suppress_bit_is_set(const uint8_t *p_frame);

/**
 * @brief Determines if the IE present bit is set.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @retval  true   IE present bit is set.
 * @retval  false  IE present bit is not set.
 *
 */
bool mac_802154_frame_parser_ie_present_bit_is_set(const uint8_t *p_frame);

/**
 * @brief Determines if the Ack Request (AR) bit is set.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @retval  true   AR bit is set.
 * @retval  false  AR bit is not set.
 *
 */
bool mac_802154_frame_parser_ar_bit_is_set(const uint8_t *p_frame);

/**
 * @brief Gets the IE header field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Pointer to the first byte of the IE header field in @p p_frame.
 * @returns  NULL if the IE header cannot be retrieved (that is, the IE header is not present).
 *
 */
const uint8_t *mac_802154_frame_parser_ie_header_get(const uint8_t *p_frame);

/**
 * @brief Gets the offset of the IE header field in the provided frame.
 *
 * @param[in]   p_frame  Pointer to a frame.
 *
 * @returns  Offset in bytes of the IE header field, including one byte of the frame length.
 * @returns  Zero if the IE header cannot be retrieved (that is, the IE header is not present).
 *
 */
uint8_t mac_802154_frame_parser_ie_header_offset_get(const uint8_t *p_frame);

#endif // MAC_802154_FRAME_PARSER_H
