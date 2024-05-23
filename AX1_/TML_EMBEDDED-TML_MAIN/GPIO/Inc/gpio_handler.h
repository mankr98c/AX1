/*
 * gpio_handler.h
 *
 *  Created on: May 9, 2024
 *      Author: 91944
 */

#ifndef INC_GPIO_HANDLER_H_
#define INC_GPIO_HANDLER_H_
#include"common.h"
#include "gpio.h"

void Turn_OFF_Modules(void);
void Turn_ON_Modules(void);
void GO_TO_SLEEP(void);
void WAKEUP(void);
void config_IT(void);
void On_Wakeup_Config(void);
void Configure_RTC_WakeUp_Timer(void);
void SL_WK_GPIO_Int(void);
bool getIgnition(void);
bool getTemperStatus(void);
void Disable_EXTI0(void);
bool setTime(uint8_t hours,uint8_t minute,uint8_t second);
bool setDate(uint8_t date,uint8_t month,uint8_t year);
bool getSleepStatus();
void setSleppStatus(bool sleep);
//void getDate(RTC_DateTypeDef *opDate);
//void getTime(RTC_TimeTypeDef *opTime);
void On_Wakeup_Config();
#endif /* INC_GPIO_HANDLER_H_ */
