#include"CAN.h"
#define MAX_RX_DATA_SIZE 8
FDCAN_RxHeaderTypeDef            RxHeader_CAN2;
extern FDCAN_RxHeaderTypeDef RxHeader;
extern FDCAN_TxHeaderTypeDef            TxHeader, TxHeader1;
uint8_t TxData[8]= {11,12,32,12};
uint16_t *Resp;
uint8_t RxCAN_1_DATA[MAX_RX_DATA_SIZE];
uint8_t RxCAN_2_DATA[MAX_RX_DATA_SIZE];
extern int sleepcount;
Map canDataMap;
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_HandleTypeDef hfdcan2;

void getCANData(Map *canDataCopy) {
    memcpy(canDataCopy, &canDataMap,sizeof(Map));
    clearBuffer(&canDataMap,sizeof(Map));
}

void TX_DataCAN() {
	uint32_t lvl = HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1);
	if (lvl >= 1) {
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &TxHeader, TxData)!= HAL_OK)  // TRANSMITTING DATA
				{
			Error_Handler();
		}
	}
	uint32_t lvl1 = HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2);
	if (lvl1 >= 1) {
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &TxHeader1, TxData)!= HAL_OK)  // TRANSMITTING DATA
				{
			Error_Handler();
		}
	}
}
void processCAN1(uint32_t id,uint8_t *data,uint8_t length){
	addToMap(&canDataMap,id,data,length);
}
void initCan(void){
//	TX_DataCAN();
	clearBuffer(&canDataMap,sizeof(Map));
}

void processCAN2(FDCAN_RxHeaderTypeDef *rxHeader,uint8_t *data){
	addToMap(&canDataMap,rxHeader->Identifier,data,rxHeader->DataLength);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  static uint32_t dataIndex = 0;

  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) == SET)
  { sleepcount =0;
		if(HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &RxHeader, RxCAN_1_DATA) == HAL_OK){
			    processCAN2(&RxHeader,&RxCAN_1_DATA);
		}
		if(HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0, &RxHeader,RxCAN_2_DATA) == HAL_OK){
				//processCAN2(&RxHeader,&RxCAN_2_DATA);
		}
    }

}
