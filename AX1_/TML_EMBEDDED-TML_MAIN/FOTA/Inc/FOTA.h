/*
 * FOTA.h
 *
 *  Created on: Apr 8, 2024
 *      Author: Manky
 */

#ifndef INC_FOTA_H_
#define INC_FOTA_H_

#include "main.h" // Include necessary header file
#include "stdbool.h"
#include "EC200Utils.h"
#include "common.h"

#define header "$MMI"
#define URL "https://intouchdev.mapmyindia.com/IntouchFOTACOTA/"

#define TIMEOUT_MS 60000 // Timeout set to 1 minute (60,000 milliseconds)
#define EC_TOUT (int*)40   // AT-commands Response Timeout
#define Delimiter '*'
#define KeyCode 1

#define PACKET_SIZE 1024
#define CRC_POLYNOMIAL 0x104C11DB7
#define CRC_INITIAL_VALUE 0xFFFFFFFF
#define CRC_XOR_OUT 0xFFFFFFFF

/****************************************************** CONFIG SECTOR ADDRESS *****************************************************/
//										 Memory range  Sector 179 0x08066000   -    0x08067FFF
#define Config_Data ((uint32_t)0x08166000) //Sector 179
#define S_Key_ADDR ((uint32_t)0x08166010)

// For REFERENCE
// #define S_Key2_ADDR ((uint32_t)0x08066020)
// #define S_Key3_ADDR ((uint32_t)0x08066030)

#define C_Key_ADDR ((uint32_t)0x08166080)

// For REFERENCE
//#define C_Key2_ADDR ((uint32_t)0x08066050)
//#define C_Key3_ADDR ((uint32_t)0x08066060)
//#define C_Key4_ADDR ((uint32_t)0x08066070)
//#define C_Key5_ADDR ((uint32_t)0x08066080)
//#define C_Key6_ADDR ((uint32_t)0x08066090)
//#define C_Key7_ADDR ((uint32_t)0x080660A0)
//#define C_Key8_ADDR ((uint32_t)0x080660B0)

#define App1_LOC_Addr ((uint32_t)0x081660C0)   // GI
#define App2_LOC_Addr ((uint32_t)0x081661D0)
#define App3_LOC_Addr ((uint32_t)0x081661E0)

#define Last_App_LOC ((uint32_t)0x081661F0)

#define Current_App_LOC ((uint32_t)0x08168000) // Sector 180

#define Imei_Addr ((uint32_t)0x08166120)

//   											Unallocated Range from 0x08167000

/*___________________________________________________END OF MEMORY ALLOCATION ____________________________________________________*/

/****************************************************** CONFIG SECTOR ADDRESS *****************************************************/
//										 Memory range  Sector 51 0x08066000   -    0x08067FFF
#define B_Config_Data ((uint32_t)0x08066000) //Sector 51
#define B_S_Key_ADDR ((uint32_t)0x08066010)

// For REFERENCE
// #define S_Key2_ADDR ((uint32_t)0x08066020)
// #define S_Key3_ADDR ((uint32_t)0x08066030)

#define B_C_Key_ADDR ((uint32_t)0x08066080)

// For REFERENCE
//#define C_Key2_ADDR ((uint32_t)0x08066050)
//#define C_Key3_ADDR ((uint32_t)0x08066060)
//#define C_Key4_ADDR ((uint32_t)0x08066070)
//#define C_Key5_ADDR ((uint32_t)0x08066080)
//#define C_Key6_ADDR ((uint32_t)0x08066090)
//#define C_Key7_ADDR ((uint32_t)0x080660A0)
//#define C_Key8_ADDR ((uint32_t)0x080660B0)

#define B_App1_LOC_Addr ((uint32_t)0x080660C0)   // GI
#define B_App2_LOC_Addr ((uint32_t)0x080661D0)
#define B_App3_LOC_Addr ((uint32_t)0x080661E0)

#define B_Last_App_ver_Addr ((uint32_t)0x080661F0)

#define B_Current_App_LOC ((uint32_t)0x08066100)

#define B_Imei_Addr ((uint32_t)0x08066120)

//   											Unallocated Range from 0x08067000

/*___________________________________________________END OF Backup MEMORY ALLOCATION ____________________________________________________*/

void Config_Update(char *cmnds, char *Data);
int Response_Check(char Buff[]);
void Message_Type(int Msg_ID);
bool processMessage(int messageType);
void Seprate_String(char Buff[], int Start_Location, int index, char *data[]);
void Extract_Data_from_Buffer(int Start_loc, int End_loc, char Buff[],
		char *extractedData, int *size);
void string_to_hex_array(const char hex_string[], uint8_t hex_array[]);
int Hex_Check(char Buff[], int hexValue);
void Fetch_Size(uint8_t buffer[], int content_length);
//bool EC20_Transmit(const char *command,char *resBuff);
//bool wait_for_response(char *resBuff);
uint8_t Calculate_CRC8(uint8_t *data, uint32_t size);
//void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
//void Ec200_RxEventCallback(uint16_t size);
void FOTA_Seprate_String(char Buff[], int Start_Location, int index,
		char *data[], char Encpt_Data[]);
bool setStaticKeys(char *key1, char *key2, char *key3);
bool setComKeys(char *key1, char *key2, char *key3, char *key4, char *key5,
		char *key6, char *key7, char *key8);
bool EC200_Config_Update(const char *command);
#endif /* INC_FOTA_H_ */
