/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    app_section.h
  * @brief   Memory section definition for user application.
  * @author
  * @date    2017.6.7
  * @version v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   * *************************************************************************************
  */

/*============================================================================*
 *                      Define to prevent recursive inclusion
 *============================================================================*/
#ifndef _APP_SECTION_H_
#define _APP_SECTION_H_

/** @defgroup APP_SECTION APP Section
  * @brief Memory section definition for user application.
  * @{
  */
/*============================================================================*
  *                                   Macros
  *============================================================================*/
/** @defgroup APP_SECTION_Exported_Macros App Section Exported Macros
    * @brief
    * @{
    */
/*flash const data or flash function */
#define APP_FLASH_HEADER     __attribute__((section(".app.flash.header")))     __attribute__((used)) //!< Flash header.
#define APP_FLASH_HEADER_EXT __attribute__((section(".app.flash.header_ext"))) __attribute__((used)) //!< Flash header ext.
/* flash code */
#define APP_FLASH_TEXT_SECTION     __attribute__((section(".app.flash.text")))  //!< Flash text.
#define APP_FLASH_GAP_TEXT_SECTION     __attribute__((section(".app.flash.gap.text"))) //!< Flash text (gap region).
/* flash ro data */
#define APP_FLASH_RODATA_SECTION   __attribute__((section(".app.flash.rodata"))) //!< Flash rodata.
/* ram code */
#define DATA_RAM_FUNCTION               __attribute__((section(".app.data_ram.text"))) //!< Data ram code.
#define SHARE_CACHE_RAM_SECTION          __attribute__((section(".ram.sharecacheram.text")))  //!< Share cache ram code or data.

/*  global variable or ram function,  data on (default) */
#define RAM_DATAON_DATA_SECTION    __attribute__((section(".ram.dataon.data")))  //!< Data placed on DATA ON ram.
#define RAM_DATAON_BSS_SECTION     __attribute__((section(".ram.dataon.bss")))  //!< BSS placed on DATA ON ram.
#define RAM_DATAON_UNINIT_SECTION  __attribute__((section(".uninit.ram")))  //!< Uninit placed on DATA ON ram.

/* buffer on */
#define RAM_BUFFERON_DATA_SECTION  __attribute__((section(".ram.bufferon.data")))  //!< Data placed on BUFFER ON ram.
#define RAM_BUFFERON_BSS_SECTION   __attribute__((section(".ram.bufferon.bss")))   //!< BSS placed on BUFFER ON ram.


/* overlay section, only support three overlay sections now. */
#define OVERLAY_SECTION_BOOT_ONCE  __attribute__((section(".app.overlay_a")))   //!< Overlay a section.
#define OVERLAY_B_SECTION          __attribute__((section(".app.overlay_b")))   //!< Overlay b section.
#define OVERLAY_C_SECTION          __attribute__((section(".app.overlay_c")))   //!< Overlay c section.

/** End of APP_SECTION_Exported_Macros
    * @}
    */

/** @} */ /* End of group APP_SECTION */

#endif /* _APP_SECTION_H_ */
