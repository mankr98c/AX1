/*
 * common.c
 *
 *  Created on: Feb 21, 2024
 *      Author: 91944
 */
#include "common.h"

UART_HandleTypeDef *printUart;
int printUartSet=0;
bool enablePrintUartIdle=true;
uint8_t printUart_ResponseBuff[PRINT_RESBUFF_SIZE]={};
void setupPrintUart(UART_HandleTypeDef *uart,bool debug){
	printUart=uart;
	if(debug)
		printUartSet=1;
//	openPrintUartIdle();
}
void printc(uint8_t delay,const char* msg, ...) {
	if(printUartSet){
			char message[2024];
			va_list args;
			va_start(args, msg);
			vsnprintf(message, sizeof(message), msg, args);
			va_end(args);
			HAL_UART_Transmit(printUart, (uint8_t*)message, strlen(message), delay);
	}
}
void print(uint8_t delay,uint8_t* data,int size){
	if(printUartSet){
		HAL_UART_Transmit(printUart,data,size, delay);
	}
}
void clearBuffer(uint8_t* buffer, size_t size) {
    // Use memset to fill the buffer with zeros
    memset(buffer, 0, size);
}
void printhex(uint8_t delay,uint8_t* data,int size){
	if(printUartSet){
		for (int i = 0; i < size; i++) {
				char hex[4];  // Buffer to store the hexadecimal representation of each byte
				sprintf(hex, "%02X ", data[i]);  // Format each byte as a two-digit hexadecimal number
				HAL_UART_Transmit(printUart, (uint8_t*)hex, strlen(hex), delay);
			}
	}
}
bool splitString(uint8_t *string,char *spliter,uint8_t opbuffer[][100]){
	uint8_t *line=strstr(string,spliter);
	int i=0;
	if(line==NULL){
		memcpy(opbuffer[i++],string,strlen(string));
	}
	while(line!=NULL){
		uint8_t length = line - string;
		char substring[length];
		memset(substring,0,length);
		if (substring == NULL) {
				printc(MinDelay,"Memory allocation failed\n");
				return false;
		}
		memcpy(substring,string,length);
		string=line+strlen(spliter);
//		printc(MinDelay,"sub string \n %s\n",substring);
		if(substring==NULL)
			return false;
		memcpy((uint8_t*)opbuffer[i++],substring,length);
		line=strstr(string,spliter);
		if(line==NULL){
//			printc(MinDelay,"last line %s\n",string);
			memcpy(opbuffer[i++],string,strlen(string));
		}
	}
	return true;
}
void switchPrintIdle()
{
	if(enablePrintUartIdle)
		enablePrintUartIdle = false;
	else
		enablePrintUartIdle = true;
}
void openPrintUartIdle(){
//	if(enablePrintUartIdle)
    	//HAL_UARTEx_ReceiveToIdle_IT(printUart,printUart_ResponseBuff,PRINT_RESBUFF_SIZE);
}
void print_RxEventCallback(uint16_t size){
	 printc(MinDelay,"Callback From Print Uart %d %s\n",size,printUart_ResponseBuff);
	 onPrintUartCmd(printUart_ResponseBuff);
	 clearBuffer(printUart_ResponseBuff,PRINT_RESBUFF_SIZE);
 }
