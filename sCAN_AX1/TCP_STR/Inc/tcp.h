/*
 * tcp.h
 *
 *  Created on: Apr 23, 2024
 *      Author: saura
 */

#ifndef TCP_H_
#define TCP_H_

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



extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart6;


bool TCP_init();
bool TCP_Open();
bool TCP_config();
void setTcpServerIP(char *ip);
void setTcpPort(int port);
int  getTcpPort();
void getTcpServerIP(char *opIP);
bool TCP_Send(const char *data);




#endif /* TCP_H_ */
