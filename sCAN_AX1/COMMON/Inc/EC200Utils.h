


#ifndef INC_EC200UTILS_H_
#define INC_EC200UTILS_H_

#include "common.h"
#include "stm32h5xx_hal.h"
#define RESBUFF_SIZE 2048
#define EC200MINRESONSETIME 1000
/******************************************* END Defination ********************************************/

/*******************************************PIN SETUP**************************************************/
#define GNSS_ON()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET) // Set PD2 high GNSS ON/OFF
#define GNSS_OFF() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET)

#define GPIO_LTE_ON()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)
#define GPIO_LTE_OFF() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET) // Set PC9 high LTE ON/OFF SET=OFF,RESET=ON

#define GPIO_3V3_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET) // Set PB3 high 3.3V ON/OFF
#define GPIO_3V3_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)

#define GPS_LIGHT_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define GPS_LIGHT_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)

#define GSM_LIGHT_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
#define GSM_LIGHT_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)

#define PWR_LIGHT_ON() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET)
#define PWR_LIGHT_OFF() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET)
/*******************************************END SETUP**************************************************/

void enableIdle();
bool EC200_Init(void);
void E200_CpltCallback(void);
void E200_RxHAlfCpltCallback(void);
void Ec200_RxEventCallback(uint16_t size);
bool EC200_Transmit(const char *command,char *resBuff,int timout);
bool setupEc200(UART_HandleTypeDef *uart);
void setupEc200MQTT(UART_HandleTypeDef *uart);
bool wait_for_response(char *resBuff,int timeout);
void switchIdle();
bool get_MCC_MNC_CellID(char *opMMC,char *opMNC,char *opCELL,char *CellID);
#endif
