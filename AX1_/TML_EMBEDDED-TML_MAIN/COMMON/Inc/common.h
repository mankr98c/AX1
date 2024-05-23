/*
 * common.h
 *
 *  Created on: Feb 21, 2024
 *      Author: 91944
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_
//use this for set up uart in which you want to print the value
#include<stdio.h>
#include "stm32h5xx_hal.h"
#include<string.h>
#include <stdio.h>
#include <stdarg.h>
#include<stdbool.h>

#define MinDelay 10
#define PRINT_RESBUFF_SIZE 500
void clearBuffer(uint8_t* buffer, size_t size);
void setupPrintUart(UART_HandleTypeDef *uart,bool debug);
void printc(uint8_t delay,const char* msg, ...);
void printhex(uint8_t delay,uint8_t* data,int size);
void print(uint8_t delay,uint8_t* data,int size);
bool splitString(uint8_t *string,char *spliter,uint8_t opbuffer[][100]);
void openPrintUartIdle();


#endif /* INC_COMMON_H_ */
