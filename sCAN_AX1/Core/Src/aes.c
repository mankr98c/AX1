/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    aes.c
  * @brief   This file provides code for the configuration
  *          of the AES instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "aes.h"

/* USER CODE BEGIN 0 */
#include "main.h"
uint32_t pKeyAES1[4];
extern char ComKeys [8][33];
extern char StatKeys[3][33];
void sKey_update(int key)   // update the key based input
{
	 hexStringToUint32Array(StatKeys[key], pKeyAES1);
	 hcryp.Init.pKey = (uint32_t *)pKeyAES1;

	    // Print the converted values
	    printc(100,"staticKey[]: %s\n", StatKeys[key]);
//	    printc(100,("pKeyAES[0]: 0x%08X\n", pKeyAES1[0]);
//	    printc(100,("pKeyAES[1]: 0x%08X\n", pKeyAES1[1]);
//	    printc(100,("pKeyAES[2]: 0x%08X\n", pKeyAES1[2]);
//	    printc(100,("pKeyAES[3]: 0x%08X\n", pKeyAES1[3]);
}
void cKey_update(int key)   // update the key based input
{
	 hexStringToUint32Array(ComKeys[key], pKeyAES1);
	 hcryp.Init.pKey = (uint32_t *)pKeyAES1;
    // Print the converted values
	    printc(100,"ComKeys[]: %s\n", ComKeys[key]);
//	    printc(100,"pKeyAES[0]: 0x%08X\n", pKeyAES1[0]);
//	    printc(100,"pKeyAES[1]: 0x%08X\n", pKeyAES1[1]);
//	    printc(100,"pKeyAES[2]: 0x%08X\n", pKeyAES1[2]);
//	    printc(100,"pKeyAES[3]: 0x%08X\n", pKeyAES1[3]);
}
void hexStringToUint32Array(const char *hexString, uint32_t *uintArray) {
    int len = strlen(hexString);
    int i, j;

    for (i = 0, j = 0; i < len; i += 8, j++) {
        sscanf(hexString + i, "%8x", &uintArray[j]);
    }
}
/* USER CODE END 0 */

CRYP_HandleTypeDef hcryp;
uint32_t pKeyAES[4] = {0x00000000,0x00000000,0x00000000,0x00000000};

/* AES init function */
void MX_AES_Init(void)
{

  /* USER CODE BEGIN AES_Init 0 */

  /* USER CODE END AES_Init 0 */

  /* USER CODE BEGIN AES_Init 1 */

  /* USER CODE END AES_Init 1 */
  hcryp.Instance = AES;
  hcryp.Init.DataType = CRYP_NO_SWAP;
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
  hcryp.Init.pKey = (uint32_t *)pKeyAES;
  hcryp.Init.Algorithm = CRYP_AES_ECB;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_WORD;
  hcryp.Init.HeaderWidthUnit = CRYP_HEADERWIDTHUNIT_WORD;
  hcryp.Init.KeyIVConfigSkip = CRYP_KEYIVCONFIG_ALWAYS;
  hcryp.Init.KeyMode = CRYP_KEYMODE_NORMAL;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN AES_Init 2 */

  /* USER CODE END AES_Init 2 */

}

void HAL_CRYP_MspInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==AES)
  {
  /* USER CODE BEGIN AES_MspInit 0 */

  /* USER CODE END AES_MspInit 0 */
    /* AES clock enable */
    __HAL_RCC_AES_CLK_ENABLE();
  /* USER CODE BEGIN AES_MspInit 1 */

  /* USER CODE END AES_MspInit 1 */
  }
}

void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==AES)
  {
  /* USER CODE BEGIN AES_MspDeInit 0 */

  /* USER CODE END AES_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_AES_CLK_DISABLE();
  /* USER CODE BEGIN AES_MspDeInit 1 */

  /* USER CODE END AES_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
