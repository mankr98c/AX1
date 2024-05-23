/*
 * sms.h
 *
 *  Created on: May 2, 2024
 *      Author: saura
 */

#ifndef INC_SMS_H_
#define INC_SMS_H_

#include "stm32h5xx_hal.h"
#include "main.h"
#include "icache.h"
#include "memorymap.h"
#include "usart.h"
#include "gpio.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "common.h"



void clear_sms_resp_buff();
bool SMS_Init();
bool SMS_Read(int index, uint8_t* message_buffer);
void setMessageStatus(bool msgStatus);
bool getMessageStatus();
bool SMS_Delete(int index);
void onMessage();
bool SMS_Send(char *number,char *message);
#endif /* INC_SMS_H_ */
