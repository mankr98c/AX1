
#ifndef __FLASH_H
#define __FLASH_H
#include "main.h"

#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x081E0000) /* Base @ of SECTOR 1, 128 Kbytes */

#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_7   /* Start @ of user Flash area */


#define  Start_Address			(ADDR_FLASH_SECTOR_1)


extern uint32_t FirstSector,
                NbOfSectors,
                BankNumber,
                Address,
                SectorError;

uint32_t GetSector(uint32_t Address);
uint32_t GetBank(uint32_t Address);
uint32_t Check_Program(uint32_t StartAddress, uint32_t EndAddress, uint32_t *Data);
void Program_Flash(void);
void HAL_FLASH_READ(char *Buff, uint32_t Addr, int Buff_Lnt);
void HAL_FLASH_Write_Buff(uint32_t FlashAddress, uint8_t *Data, uint32_t DataSize);
void HAL_FLASH_ERASE(uint32_t FlashEraseAddress);
void Write_Config(void);
void Get_Config(void);
void Get_Factory(void);
void Flash_BKP_Parameters(void);
char* GET_DID_Param(uint8_t Parameter_No);
void Erase_sectors(uint32_t FlashAddress);
void GET_DID_Parameter(uint8_t Parameter_No, char* buffer);
uint64_t reverseByteOrder(uint64_t input);
int FLASH_Read_Buff(uint32_t Read_Address, char Read_Buff[],
		uint32_t ReadDataSize);
extern FLASH_EraseInitTypeDef EraseInitStruct;  // Erase structure

#endif /* __FLASH_H */
