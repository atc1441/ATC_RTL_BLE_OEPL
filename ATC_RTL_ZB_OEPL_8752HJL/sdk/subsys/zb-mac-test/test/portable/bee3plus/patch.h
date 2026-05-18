#ifndef PATCH_H_
#define PATCH_H_
#if 0
uint32_t mac_GetTxTimeStampUS(void);
uint8_t mac_GetCcaMode(void);
uint8_t mac_GetAcceptFrm(void);
void mac_SetAcceptFrm(uint8_t rxftype);
void mac_GetAcceptFrm15(void);
void mac_SetAcceptFrm15(uint8_t rxftype);
uint32_t mac_GetSrcExtMatch(uint8_t index, bool high_bytes);
uint8_t mac_GetImmAckPendingBit(void);
uint8_t mac_GetErrPkt(void);
uint8_t mac_GetNoCrcInTxNFIFO(void);
uint8_t mac_SetNoCrcInTxNFIFO(uint8_t en);
uint8_t mac_GetRxDualFifoEn(void);
void mac_SetRxDualFifoEn(uint8_t en);
uint8_t  mac_TrigTxN_patch(bool_t AckReq, bool_t SecReq, bool_t Is2015);
void mac_SetErrPkt(uint8_t en);
uint8_t mac_GetRxFifoRdSel(void);
uint8_t mac_GetTxEnhAckTrigEn(void);
void mac_RxEnableAccess(void);
uint8_t mac_RxLength(void);
void mac_RxPayload(uint8_t *payload, uint32_t length);
void mac_RxDisableAccess(void);
uint8_t mac_GetRxFifoFull(void);
uint32_t mac_GetTxAtStatus(void);
uint8_t mac_GetRxFrmSeqComp(void);
uint32_t mac_FIFO_BaseAddr(mac_fifo_id_t fifo_id);
uint8_t mac_SetFreq(uint16_t  freq);
uint16_t mac_GetFreq(void);
uint16_t mac_GetShortAddress(void);
uint8_t *mac_GetLongAddress(void);
uint8_t mac_IsEnabled(void);
void mac_SetSrcAddrMatchMode_patch(uint8_t mode);
uint8_t mac_GetSrcAddrMatchMode_patch(void);
void mac_SetPANCoord(uint8_t is_pancoord);
uint8_t mac_GetPANCoord(void);
void mac_SetFrmPend(uint8_t enable);
uint8_t mac_GetFrmPend(void);
void mac_SetDataReqCmdFrmPend(uint8_t enable);
uint8_t mac_GetDataReqCmdFrmPend(void);
uint8_t mac_TrigTxNAtTime_patch(bool_t AckReq, bool_t SecReq, bool_t Is2015, uint32_t time_us);

void mac_SetTxNCsma(bool enable);
bool mac_GetTxNCsma(void);
void mac_SetMaxCSMABackoffs(uint8_t MaxCSMABackoffs);
uint8_t mac_GetMaxCSMABackoffs(void);
void mac_SetCSMAMinBe(uint8_t macMinBe);
uint8_t mac_GetCSMAMinBe(void);
void mac_SetCSMAMaxBe(uint8_t macMaxBe);
uint8_t mac_GetCSMAMaxBe(void);
void mac_SetCcaEDThreshold(uint8_t value);
void mac_SetCcaMode_patch(mac_cca_mode_t cca_mode);

#define MAC_TXN_BASE_ADDR             ((uint32_t)mac_FIFO_BaseAddr(TXN_FIFO_ID))
#define MAC_TXB_BASE_ADDR             ((uint32_t)mac_FIFO_BaseAddr(TXB_FIFO_ID))
#define MAC_TXG1_BASE_ADDR            ((uint32_t)mac_FIFO_BaseAddr(TXG1_FIFO_ID))
#define MAC_TXG2_BASE_ADDR            ((uint32_t)mac_FIFO_BaseAddr(TXG2_FIFO_ID))
#define MAC_KEY_FIFO_BASE_ADDR        ((uint32_t)mac_FIFO_BaseAddr(KEY_FIFO_ID))
#define MAC_RX_BASE_ADDR              ((uint32_t)mac_FIFO_BaseAddr(RX_FIFO_ID))
#endif
#endif // PATCH_H_
