#ifndef MEM_CONFIG_H
#define MEM_CONFIG_H

#include "flash_map.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*
 *                        Code configuration
 *============================================================================*/
/** @brief set app bank to support OTA: 1 is ota bank1, 0 is ota bank0 */
#define BUILD_BANK                                0

/** @brief ram code configuration: 1 is ram code, 0 is flash code */
#define FEATURE_RAM_CODE                          0

/** @brief Flash-Security feature: 1 is enable, 0 is disable */
#define FEATURE_FLASH_SEC                         0

/*============================================================================*
 *                        data ram layout configuration
 *============================================================================*/
/* Data RAM layout:                    104K
example:
   1) reserved for rom:                18K (fixed)
   2) Platform Patch:                   8K (fixed)
   3) Stack Patch:                      8K (fixed)
   4) upperstack:                       2K (adjustable, depend on used upperstack version)
   5) app global + ram code:           16K (adjustable, config APP_GLOBAL_SIZE)
   6) Heap ON:                         52K (adjustable, config HEAP_DATA_ON_SIZE)
*/
/** @brief data ram size for upperstack global variables and code */
#define UPPERSTACK_GLOBAL_SIZE         (2 * 1024)
/** @brief data ram size for app global variables and code, could be changed */
#define APP_GLOBAL_SIZE                (16 * 1024)
/** @brief data ram size for heap, could be changed, but (UPPERSTACK_GLOBAL_SIZE + APP_GLOBAL_SIZE + HEAP_DATA_ON_SIZE) must be 68k  */
#define HEAP_DATA_ON_SIZE              (70 * 1024 - APP_GLOBAL_SIZE - UPPERSTACK_GLOBAL_SIZE)
/** @brief shared cache ram size (adjustable, config SHARE_CACHE_RAM_SIZE: 0/4KB/8KB) */
#define SHARE_CACHE_RAM_SIZE           (0 * 1024)
/*****************************************************/
/** @brief app global ram start addr, can't be modified */
#define APP_GLOBAL_ADDR                (0x200000 + (18 + 8 + 8) * 1024 + UPPERSTACK_GLOBAL_SIZE)

/***************The following macros can't be modified***************/
#define DATA_RAM_START_ADDR            0x00200000
#define DATA_RAM_TOTAL_SIZE            (104 * 1024)

#define SHARE_CACHE_RAM_ADDR           (DATA_RAM_START_ADDR + DATA_RAM_TOTAL_SIZE)

#ifdef __cplusplus
}
#endif


/** @} */ /* End of group MEM_CONFIG */
#endif
