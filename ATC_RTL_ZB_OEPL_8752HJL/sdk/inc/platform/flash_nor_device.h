/**
 *****************************************************************************************
 *     Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
 *****************************************************************************************
 * @file    flash_nor_device.h
 * @brief   Nor flash external API header file
 * @author  Yao-Yu
 * @date    2020-08-31
 * @version v0.1
 * ***************************************************************************************
 */

/*============================================================================*
 *               Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _FLASH_NOR_DEVICE_H
#define _FLASH_NOR_DEVICE_H

/*============================================================================*
 *                               Header Files
*============================================================================*/
#include <stdint.h>
#include <stdbool.h>

/** @defgroup  FLASH_DEVICE    Flash Device
    * @brief Flash image layout and APIs
    * @{
    */

/*============================================================================*
  *                                   Variables
 *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Variables Flash Device Exported Variables
    * @brief
    * @{
    */
typedef void (*FlashCB)(void);
extern void *flash_sem;
extern uint8_t *flash_nor_ioctl_buf;
extern uint32_t flash_nor_ioctl_buf_len;

/** End of FLASH_DEVICE_Exported_Variables
  * @}
  */

/****************************************************************************************
 * Nor Flash Macro
 ****************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Macros Flash Device Exported Macros
    * @brief
    * @{
    */

#define FLASH_NOR_PAGE_SIZE                         (256)
#define FLASH_NOR_SECTOR_SIZE                       (0x1000)
#define FLASH_NOR_BLOCK_SIZE                        (0x10000)
#define FLASH_NOR_BP_FIELDS_OFFSET                  (0x02)
#define FLASH_NOR_STATUS_WIP_BIT                    (0x01)
#define FLASH_NOR_STATUS_WEL_BIT                    (0x02)
#define FLASH_NOR_IOCTL_MAX_BUF_LEN                 (0xFF)
#define FLASH_NOR_TABLE_MAGIC_PATTERN               (0x5A5A12A5)
#define NOR_FLASH_OFFSET_TO_NO_CACHE                    (0x01000000)
#define IS_FLASH_SIZE_LARGER_THAN_16MB(size)        (size > (1 << 24))
#define IS_SAME_FLASH_NOR_PAGE(addr0, addr1)        (((addr0) & 0xFFFFFF00) == ((addr1) & 0xFFFFFF00))
#define CAL_ADDR                                    get_header_addr_by_img_id(OTA) + offsetof(T_IMG_HEADER_FORMAT, magic_pattern)
#define IS_SPIC0_ADDR(addr)                         (((addr >= FMC_MAIN) && (addr < FMC_MAIN + SPIC0_SIZE)) || ((addr >= (FMC_MAIN | NOR_FLASH_OFFSET_TO_NO_CACHE)) && (addr < (FMC_MAIN | NOR_FLASH_OFFSET_TO_NO_CACHE) + SPIC0_SIZE)))

/** End of FLASH_DEVICE_Exported_Macros
  * @}
  */

/****************************************************************************************
 * Nor Flash Enumeration
 ****************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Types Flash Device Exported Types
  * @brief
  * @{
  */

typedef enum
{
    FLASH_NOR_ERASE_SECTOR  = 1,
    FLASH_NOR_ERASE_BLOCK   = 2,
    FLASH_NOR_ERASE_CHIP    = 4,
} FLASH_NOR_ERASE_MODE;

typedef enum
{
    FLASH_NOR_DMA_AUTO_F2R = 0,
    FLASH_NOR_DMA_AUTO_R2F,
    FLASH_NOR_DMA_USER_F2R,
    FLASH_NOR_DMA_USER_R2F
} FLASH_NOR_DMA_TYPE;

typedef enum
{
    FLASH_NOR_IDX_SPIC0,            /* Nor flash controlled by SPIC0 */
    FLASH_NOR_IDX_SPIC1,
    FLASH_NOR_IDX_MAX
} FLASH_NOR_IDX_TYPE;

typedef struct
{
    // flash info
    uint32_t  PageSize;
    uint32_t  Main_Addr;
    uint32_t  Spic_Cal_Addr;

    // flash layout table
    uint32_t  ota_bank0_addr;
    uint32_t  ota_bank0_size;
    uint32_t  ota_bank1_addr;
    uint32_t  ota_bank1_size;
    uint32_t  ftl_addr;
    uint32_t  ftl_size;

    uint32_t  ota_tmp_addr;
    uint32_t  ota_tmp_size;

    uint32_t  bkp_data1_addr;
    uint32_t  bkp_data1_size;
    uint32_t  bkp_data2_addr;
    uint32_t  bkp_data2_size;

    // only patch_addr is fixed
    // other fields are changable
    // app/ota/ftl/dsp dont need to be sequencial
    // just beware of the size of them have no overlap condition

    uint32_t  HardFault_Record_CFG;
    // default value: 0x7
    // [0]: 1, enable saving HardFault_Record
    // [1]: reserved//1, 1 bit slow mode
    // [2]: 1, dump HardFaultRecord in boot time
    // [3]: 1, Clean up HardFaultRecord after dumping HardFault_Record
    // [4]: 1, Clean up HardFaultRecord before saving HardFault_Record
    uint32_t  HardFault_Record_BegAddr;
    uint32_t  HardFault_Record_EndAddr;

} T_FLASH_INFO_TBL;

typedef enum
{
    FLASH_NOR_RET_UNKNOWN,
    FLASH_NOR_RET_NOT_EXIST_BASIC_CMD,
    FLASH_NOR_RET_NOT_EXIST_ADV_CMD,
    FLASH_NOR_RET_NOT_EXIST_QUERY_INFO,
    FLASH_NOR_RET_CMD_NOT_SUPPORT,
    FLASH_NOR_RET_DEV_NOT_SUPPORT,
    FLASH_NOR_RET_VENDOR_NOT_SUPPORT,
    FLASH_NOR_RET_LOCK_FAILED,
    FLASH_NOR_RET_UNLOCK_FAILED,
    FLASH_NOR_RET_BIT_MODE_SET_FAILED,
    FLASH_NOR_RET_BIT_MODE_NOT_SUPPORT,
    FLASH_NOR_RET_ILLEGAL_OPERATION,
    FLASH_NOR_RET_PARAM_INVALID,
    FLASH_NOR_RET_WAIT_BUSY_FAILED,
    FLASH_NOR_RET_IDX_OUT_OF_RANGE,
    FLASH_NOR_RET_ADDR_OUT_OF_RANGE,
    FLASH_NOR_RET_ADDR_LARGER_THAN_FLASH_SIZE,
    FLASH_NOR_RET_CAL_IN_PHYSICAL_CYC_NOT_FOUND,
    FLASH_NOR_RET_CAL_RD_DUMMY_LENGTH_NOT_FOUND,
    FLASH_NOR_RET_CAL_FAILED,
    FLASH_NOR_RET_MALLOC_FAILED,
    FLASH_NOR_RET_HOOK_FUNC,
    FLASH_NOR_RET_SUSPEND_UNNECCESSARY,
    FLASH_NOR_RET_SET_4_BYTE_ADDRESS_MODE_FAILED,
    FLASH_NOR_RET_SUCCESS
} FLASH_NOR_RET_TYPE;

typedef enum
{
    FLASH_NOR_1_BIT_MODE,
    FLASH_NOR_2_BIT_MODE,
    FLASH_NOR_4_BIT_MODE,
    FLASH_NOR_8_BIT_MODE,
} FLASH_NOR_BIT_MODE;

typedef enum
{
    NOR_FLASH_SCCD,
    NOR_FLASH_OCCD,
    NOR_FLASH_OTA_BANK_0,
    NOR_FLASH_OTA_BANK_1,
    NOR_FLASH_OTA_TMP,
    NOR_FLASH_BKP_DATA1,
    NOR_FLASH_BKP_DATA2,
    NOR_FLASH_FTL,
    NOR_FLASH_HARDFAULT_RECORD,
    NOR_FLASH_TOTAL,
} FLASH_LAYOUT_NAME;

typedef enum _FLASH_NOR_IOCTL_TYPE
{
    FLASH_NOR_GET_BASE                          = 0x0000,
    FLASH_NOR_GET_ADDR_BASE,
    FLASH_NOR_GET_RDID,
    FLASH_NOR_GET_SIZE,
    FLASH_NOR_GET_BP,
    FLASH_NOR_GET_BP_TOP_BOTTOM,
    FLASH_NOR_GET_WAIT_BUSY_CTR,
    FLASH_NOR_GET_BIT_MODE,

    FLASH_NOR_SET_BASE                          = 0x1000,
    FLASH_NOR_SET_BP,
    FLASH_NOR_SET_BP_TOP_BOTTOM,
    FLASH_NOR_SET_BP_UNLOCK_BY_ADDR,
    FLASH_NOR_SET_WAIT_BUSY_CTR,
    FLASH_NOR_SET_SPIC_BAUD,
    FLASH_NOR_SET_LOG_BITMAP,

    FLASH_NOR_EXEC_BASE                         = 0x2000,
    FLASH_NOR_EXEC_FLASH_INIT,
    FLASH_NOR_EXEC_DP,
    FLASH_NOR_EXEC_FLASH_SW_RESET,
    FLASH_NOR_EXEC_QUERY_INFO_LOADING,
    FLASH_NOR_EXEC_HIGH_SPEED_MODE,
    FLASH_NOR_EXEC_FLASH_CAL,
    FLASH_NOR_EXEC_FLASH_READ,
    FLASH_NOR_EXEC_FLASH_WRITE,
    FLASH_NOR_EXEC_FLASH_ERASE,

    FLASH_NOR_BASE_MASK                         = 0xF000,
} FLASH_NOR_IOCTL_TYPE;

/* *INDENT-OFF* */
/** End of FLASH_DEVICE_Exported_Types
  * @}
  */

/****************************************************************************************
 * Nor Flash Functions
 ****************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Functions Flash Device Exported Functions
    * @brief
    * @{
    */
/**
 * @brief set flash bit mode
 * @param idx  specific nor flash
 * @param bit_mode  wanted bit mode
 * @return FLASH_NOR_RET_SUCCESS if success
*/
extern FLASH_NOR_RET_TYPE(*flash_nor_try_high_speed_mode)(FLASH_NOR_IDX_TYPE idx,
                                                          FLASH_NOR_BIT_MODE bit_mode);

/**
 * @brief get flash base address
 * @param idx  specific nor flash
 * @return flash base address
*/
uint32_t flash_nor_get_addr_base(FLASH_NOR_IDX_TYPE idx);

/**
 * @brief check flash bp level before ota copy, if flash is locked, must unlock bp in NVRAM
 * @param none
 * @return none
*/
extern void (*check_flash_bp_before_ota_copy)(void);

/**
 * @brief read flash data via auto mode
 * @param addr    address to read
 * @return value in this address
*/
uint32_t flash_nor_auto_read(uint32_t addr);

/**
 * @brief init flash DMA read flow
 * @param dma_type  read type, usually memory to memory
 * @param flash_cb  call back function
 * @param src_addr  read source
 * @param dst_addr  read destination
 * @param data_len  read length
 * @return void
*/
void flash_nor_auto_dma_read(FLASH_NOR_DMA_TYPE dma_type, FlashCB flash_cb, uint32_t src_addr,
                             uint32_t dst_addr, uint32_t data_len);

/**
 * @brief           dump spic & nor flash info
 * @return          none
 */
void flash_nor_dump_flash_info(void);

/**
 * @brief           get nor flash layout member start address
 * @param name      specify address of nor flash layout member
 * @return          specific address of nor flash layout member
 */
uint32_t flash_nor_get_bank_addr(FLASH_LAYOUT_NAME name);

/**
 * @brief           get nor flash layout member size
 * @param name      specify size of nor flash layout member
 * @return          specific size of nor flash layout member
 */
uint32_t flash_nor_get_bank_size(FLASH_LAYOUT_NAME name);

/**
 * @brief           set specific nor flash commands and query info
 * @param idx       specific nor flash
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
FLASH_NOR_RET_TYPE flash_nor_set_all_info(FLASH_NOR_IDX_TYPE idx);

/**
 * @brief           task-safe of @ref flash_nor_read
 * @param addr      the ram address mapping of nor flash going to be read
 * @param data      data buffer to be read into
 * @param byte_len  read data length
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
extern FLASH_NOR_RET_TYPE(*flash_nor_read_locked)(uint32_t addr, uint8_t *data, uint32_t byte_len);

/**
 * @brief           task-safe of @ref flash_nor_write
 * @param addr      the ram address mapping of nor flash going to be written
 * @param data      data buffer to be write into
 * @param byte_len  write data length
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
extern FLASH_NOR_RET_TYPE(*flash_nor_write_locked)(uint32_t addr, uint8_t *data, uint32_t byte_len);

/**
 * @brief           task-safe of @ref flash_nor_erase
 * @param addr      the ram address mapping of nor flash going to be erased
 * @param mode      erase mode defined as @ref FLASH_NOR_ERASE_MODE
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
extern FLASH_NOR_RET_TYPE(*flash_nor_erase_locked)(uint32_t addr, FLASH_NOR_ERASE_MODE mode);

/**
 * @brief           nor flash IO control entry
 * @param cmd       @ref FLASH_NOR_IOCTL_TYPE cmd
 * @param idx       specific nor flash
 * @param p1        parameter 1 based on different @ref FLASH_NOR_IOCTL_TYPE cmd
 * @param p2        parameter 2 based on different @ref FLASH_NOR_IOCTL_TYPE cmd
 * @param p3        parameter 3 based on different @ref FLASH_NOR_IOCTL_TYPE cmd
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
extern FLASH_NOR_RET_TYPE flash_nor_ioctl(uint16_t cmd, uint16_t idx, uint32_t p1, uint32_t p2,
                                          uint32_t p3);

/**
 * @brief              set top/bottom of block protect with lock
 * @param idx          specific nor flash
 * @param from_bottom  true if from bottom
 * @return             @ref FLASH_NOR_RET_TYPE result
 */
extern FLASH_NOR_RET_TYPE(*flash_nor_set_tb_bit_locked)(FLASH_NOR_IDX_TYPE idx, bool from_bottom);

/**
 * @brief           set block protect level with lock
 * @param idx       specific nor flash
 * @param bp_lv     nor flash BP level
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
extern FLASH_NOR_RET_TYPE(*flash_nor_set_bp_lv_locked)(FLASH_NOR_IDX_TYPE idx, uint8_t bp_lv);

/**
 * @brief           get block protect level with lock
 * @param idx       specific nor flash
 * @param bp_lv     nor flash BP level
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
extern FLASH_NOR_RET_TYPE(*flash_nor_get_bp_lv_locked)(FLASH_NOR_IDX_TYPE idx, uint8_t *bp_lv);

/**
 * @brief           set sequential transfer function
 * @param idx       specific nor flash
 * @param enable    true if enable
 * @return          @ref FLASH_NOR_RET_TYPE result
 */
FLASH_NOR_RET_TYPE flash_nor_set_seq_trans_enable(FLASH_NOR_IDX_TYPE idx, bool enable);

/**
 * @brief           get flash ID
 * @param idx       specific nor flash
 * @return          return flash ID
 */
uint32_t flash_nor_get_id(FLASH_NOR_IDX_TYPE idx);

/**
 * @brief           read 4 bytes flash with auto mode (addr should be 4 byte align)
 * @param addr      specific nor flash
 * @param data      read data
 * @return          true if success
 */
bool flash_nor_auto_read_locked(uint32_t addr, uint32_t *data);

/**
 * @brief get flash default block protect level depend on different flash id and different flash layout
 * @param none
 * @return default set bp level
*/
extern uint8_t (*flash_nor_get_default_bp_lv)(void);

/**
 * @brief set flash bp level to 0
 * @param pre_bp_lv  previous bp level
 * @return @ref FLASH_NOR_RET_TYPE result
*/
extern FLASH_NOR_RET_TYPE(*flash_nor_unlock_bp_all)(uint8_t *pre_bp_lv);

/**
 * @brief set flash bp level to unlock specified address
 * @param unlock_addr  specified address
 * @param old_bp_lv    previous bp level
 * @return @ref FLASH_NOR_RET_TYPE result
*/
extern FLASH_NOR_RET_TYPE(*flash_nor_unlock_bp_by_addr_locked)(uint32_t unlock_addr,
                                                               uint8_t *old_bp_lv);

/**
 * @brief set flash sequential transfer
 * @param idx       specific nor flash
 * @param enable    enable or disable
 * @return @ref FLASH_NOR_RET_TYPE result
*/
FLASH_NOR_RET_TYPE flash_nor_set_seq_trans(FLASH_NOR_IDX_TYPE idx, bool enable);


/**
 * @brief get flash current bit mode
 * @param idx       specific nor flash
 * @param mode      get bit mode
 * @return @ref FLASH_NOR_RET_TYPE result
*/
extern FLASH_NOR_RET_TYPE(*flash_nor_get_bit_mode)(FLASH_NOR_IDX_TYPE idx,
                                                   FLASH_NOR_BIT_MODE *mode);

/**
 * @brief get flash size
 * @param idx       specific nor flash
 * @return flash size
*/
extern uint32_t flash_nor_get_flash_size(FLASH_NOR_IDX_TYPE idx);

/** @} */ /* End of group FLASH_DEVICE_Exported_Functions */
/** @} */ /* End of group FLASH_DEVICE */

#endif
