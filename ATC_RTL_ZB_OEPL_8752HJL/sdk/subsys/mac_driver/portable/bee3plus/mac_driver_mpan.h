#ifndef _MAC_DRIVER_MPAN_H
#define _MAC_DRIVER_MPAN_H

#include "mac_data_type.h"

#define MAX_PAN_NUM 1

typedef void (*mac_irq_callback_t)(uint8_t pan_id, uint32_t arg);

typedef struct pan_mac_irq_handler_s
{
    mac_irq_callback_t txn_handler; // Normal FIFO TX done
    mac_irq_callback_t txnterr_handler; // Given time TXN error interrupt handler
    mac_irq_callback_t txg1_handler; // GTS1 TX done
    mac_irq_callback_t rxely_handler; // RX early interrupt
    mac_irq_callback_t rxdone_handler; // RX done interrupt
    mac_irq_callback_t rxsec_handler; // RX Security frame interrupt
    mac_irq_callback_t mactmr_handler; // MAC timer interrupt
    mac_irq_callback_t edscan_handler; // ED scan done interrupt
} pan_mac_irq_handler_t, *ppan_mac_irq_handler_t;

typedef struct pan_mac_comm_s
{
    union
    {
        uint32_t mac_ctrl; /*!< MAC control */
        struct
        {
            uint32_t initialed : 1; /*!< is MAC initialed  */
            uint32_t enable : 1; /*!< is MAC enabled */
            uint32_t radio_on : 1; /*!< is radio On */
            uint32_t tx_busy : 1; /*!< is TX busy */
            uint32_t tmr0_owner : 2; /*!< indicated which PAN own the Timer0 */
            uint32_t tmr1_owner : 2; /*!< indicated which PAN own the Timer1 */
            /* timer2 is reserved for MAC software timer, timer3 is reserved for
              free run BT clock overflow detection */
            uint32_t tmr4_owner : 2; /*!< indicated which PAN own the Timer4 */
            uint32_t tmr5_owner : 2; /*!< indicated which PAN own the Timer5 */
            uint32_t ctrl_owner : 2; /*!< indicated which PAN own the MAC control access */
            uint32_t tx_owner : 2; /*!< indicated which PAN own the packet TX resource */
            uint32_t : 16;
        } mac_ctrl_b;
    };
    void *mac_lock; /*!< the lock for MAC controll access, to prevent conflict
                         between multi-PAN */
    void *tx_lock; /*!< the lock for MAC TX to prevent packet TX conflict
                             between multi-PAN */
    /* interrupt callback functions */
    /* timer 0 timeout callback */
    mac_irq_callback_t timer0_callback;
    uint32_t tmr0_cb_arg;
    /* timer 1 timeout callback */
    mac_irq_callback_t timer1_callback;
    uint32_t tmr1_cb_arg;
    /* timer 4 timeout callback */
    mac_irq_callback_t timer4_callback;
    uint32_t tmr4_cb_arg;
    /* timer 5 timeout callback */
    mac_irq_callback_t timer5_callback;
    uint32_t tmr5_cb_arg;
} pan_mac_comm_t, *ppan_mac_comm_t;

uint8_t mpan_GetCurrentPANIdx(void);

void mpan_CommonInit(pan_mac_comm_t *ptr);

void mpan_tx_lock(uint8_t pan_idx);

void mpan_tx_unlock(void);

void mpan_mac_lock(uint8_t pan_idx);

void mpan_mac_unlock(void);

void mpan_RegisterISR(uint8_t pan_idx, mac_irq_callback_t txn, mac_irq_callback_t rxely,
                      mac_irq_callback_t rxdone, mac_irq_callback_t edscan);

void mpan_RegisterTimer(uint8_t pan_idx, mac_bt_timer_id_t tmr_id, mac_irq_callback_t cb,
                        uint32_t arg);

void mpan_EnableCtl(uint8_t pan_idx, uint8_t enable);

void mpan_SetAsPANCoord(uint8_t pan_idx, uint8_t is_pancoord);

void mpan_SetPANId(uint16_t pid, uint8_t pan_idx);

uint16_t mpan_GetPANId(uint8_t pan_idx);

void mpan_SetShortAddress(uint16_t sadr, uint8_t pan_idx);

void mpan_SetLongAddress(uint8_t  *ladr, uint8_t pan_idx);

uint8_t *mpan_GetLongAddress(uint8_t pan_idx);

void mpan_SetRxFrmTypeFilter06(uint8_t rxfrmtype, uint8_t pan_idx);

void mpan_SetRxFrmTypeFilter15(uint8_t rxfrmtype, uint8_t pan_idx);

void mpan_SetEnhFrmPending(uint8_t frmpend, uint8_t pan_idx);

void mpan_SetDataReqFrmPending(uint8_t frmpend, uint8_t pan_idx);

void mpan_SetAddrMatchMode(uint8_t mode, uint8_t pan_idx);

uint8_t mpan_AddSrcShortAddrMatch(uint16_t short_addr, uint16_t panid, uint8_t pan_idx);

uint8_t mpan_DelSrcShortAddrMatch(uint16_t short_addr, uint16_t panid, uint8_t pan_idx);

uint8_t mpan_AddSrcExtAddrMatch(uint8_t *pext_addr, uint8_t pan_idx);

uint8_t mpan_DelSrcExtAddrMatch(uint8_t *pext_addr, uint8_t pan_idx);

uint8_t mpan_DelAllSrcAddrMatch(uint8_t pan_idx);

#endif
