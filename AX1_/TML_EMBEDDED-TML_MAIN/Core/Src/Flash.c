/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    flash.c
  * @brief   This file provides code for the configuration
  *          of the flash instances.
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
#include "flash.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* FLASH init function */
void MX_FLASH_Init(void)
{

  /* USER CODE BEGIN FLASH_Init 0 */

  /* USER CODE END FLASH_Init 0 */

  FLASH_OBProgramInitTypeDef pOBInit = {0};

  /* USER CODE BEGIN FLASH_Init 1 */

  /* USER CODE END FLASH_Init 1 */
  if (HAL_FLASH_Unlock() != HAL_OK)
  {
    Error_Handler();
  }

  /* Option Bytes settings */

  if (HAL_FLASH_OB_Unlock() != HAL_OK)
  {
    Error_Handler();
  }

   //!!! HAL_FLASHEx_OBProgram is commented because some parameters are missing
  //pOBInit.OptionType = OPTIONBYTE_PROD_STATE|OPTIONBYTE_USER;
  //pOBInit.USERType = ;
  //pOBInit.USERConfig = ;
    //HAL_FLASHEx_OBProgram(&pOBInit);
  if (HAL_FLASH_OB_Lock() != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_FLASH_Lock() != HAL_OK)
  {
    Error_Handler();
  }

  /* Launch Option Bytes Loading */
  /*HAL_FLASH_OB_Launch(); */

  /* USER CODE BEGIN FLASH_Init 2 */

  /* USER CODE END FLASH_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
