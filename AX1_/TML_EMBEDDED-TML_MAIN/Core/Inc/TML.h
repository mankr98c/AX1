/*
 * TML.h
 *
 *  Created on: Apr 9, 2024
 *      Author: 91944
 */

#ifndef INC_TML_H_
#define INC_TML_H_
#include"stdbool.h"
#include"LC86.h"
#include"CAN.h"
#include"FLASH_MEM.h"
#include"fifo.h"
#include"gpio_handler.h"
#include"ledblink.h"
#include "stack.h"
void init();
void loop();
void setTickTime();
int getTickTime();
void setIMEI(char *imei);
void gsmParameterUpdate();
void fotaUpdateCallback(char*);
char* getUpdateVersion();
bool getPrintUartCmdStatus();
void setPrintUartCmdStatus(bool tmpStatus);
bool task2();
#endif /* INC_TML_H_ */
