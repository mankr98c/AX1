/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    fdcan.c
  * @brief   This file provides code for the configuration
  *          of the FDCAN instances.
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
#include "fdcan.h"

/* USER CODE BEGIN 0 */

FDCAN_RxHeaderTypeDef            RxHeader;
FDCAN_TxHeaderTypeDef            TxHeader, TxHeader1;

/* USER CODE END 0 */

FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;

/* FDCAN1 init function */
void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = ENABLE;
  hfdcan1.Init.NominalPrescaler = 1;
  hfdcan1.Init.NominalSyncJumpWidth = 13;
  hfdcan1.Init.NominalTimeSeg1 = 86;
  hfdcan1.Init.NominalTimeSeg2 = 13;
  hfdcan1.Init.DataPrescaler = 4;
  hfdcan1.Init.DataSyncJumpWidth = 9;
  hfdcan1.Init.DataTimeSeg1 = 15;
  hfdcan1.Init.DataTimeSeg2 = 9;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
  FDCAN_FilterTypeDef sFilterConfig = {0};              /* clearing all the filter configuration */

  	    /* FDCAN Tx header structure configuration */
  	  TxHeader.Identifier          = 0x123;                /*  taking default identifier  */
  	  TxHeader.IdType              = FDCAN_EXTENDED_ID;    /*  0 and 0x7FF, if IdType is FDCAN_STANDARD_ID , 0 and 0x1FFFFFFF, if IdType is FDCAN_EXTENDED_ID  */
  	  TxHeader.TxFrameType         = FDCAN_DATA_FRAME;     /*  Specifies the frame type of the message that will be transmitted.  */
  	  TxHeader.DataLength          = FDCAN_DLC_BYTES_8;    /*  the length of the transmitted frame 8 bytes.  */
  	  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;     /*  FDCAN_error_state_indicator,node is error active  */
  	  TxHeader.BitRateSwitch       = FDCAN_BRS_OFF;        /*  frames transmitted/received without bit rate switching  */
  	  TxHeader.FDFormat            = FDCAN_CLASSIC_CAN;    /*  Frame transmitted/received in Classic CAN format  */
  	  TxHeader.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;   /*  Do not store Tx events */
  	  TxHeader.MessageMarker       = 0;

  	//  /* Configure Rx filter */
  	  sFilterConfig.IdType       = FDCAN_EXTENDED_ID;
  	  sFilterConfig.FilterIndex  = 1;
  	  sFilterConfig.FilterType   = FDCAN_FILTER_RANGE;//
  	  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  	  sFilterConfig.FilterID1    = 0x800;
  	  sFilterConfig.FilterID2    = 0x835;


  	  /* Configure the FDCAN reception filter */
  	  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  	  {
  	    Error_Handler();
  	  }

  	  /* Start the FDCAN module */
  	  if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  	  {
  	    Error_Handler();
  	  }
  	  /* Enable interrupts */
  	  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  	  {
  	    Error_Handler();
  	  }
  	  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_TX_FIFO_EMPTY, 0) != HAL_OK) {
  	  		/* Notification Error */
  	  		Error_Handler();
  	  	}
  /* USER CODE END FDCAN1_Init 2 */

}
/* FDCAN2 init function */
void MX_FDCAN2_Init(void)
{

  /* USER CODE BEGIN FDCAN2_Init 0 */

  /* USER CODE END FDCAN2_Init 0 */

  /* USER CODE BEGIN FDCAN2_Init 1 */

  /* USER CODE END FDCAN2_Init 1 */
  hfdcan2.Instance = FDCAN2;
  hfdcan2.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan2.Init.AutoRetransmission = ENABLE;
  hfdcan2.Init.TransmitPause = DISABLE;
  hfdcan2.Init.ProtocolException = ENABLE;
  hfdcan2.Init.NominalPrescaler = 1;
  hfdcan2.Init.NominalSyncJumpWidth = 13;
  hfdcan2.Init.NominalTimeSeg1 = 86;
  hfdcan2.Init.NominalTimeSeg2 = 13;
  hfdcan2.Init.DataPrescaler = 4;
  hfdcan2.Init.DataSyncJumpWidth = 9;
  hfdcan2.Init.DataTimeSeg1 = 15;
  hfdcan2.Init.DataTimeSeg2 = 9;
  hfdcan2.Init.StdFiltersNbr = 0;
  hfdcan2.Init.ExtFiltersNbr = 0;
  hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN2_Init 2 */
  FDCAN_FilterTypeDef sFilterConfig = {0};              /* clearing all the filter configuration */

  	    /* FDCAN Tx header structure configuration */
  	  TxHeader1.Identifier          = 0x234;                /*  taking default identifier  */
  	  TxHeader1.IdType              = FDCAN_EXTENDED_ID;    /*  0 and 0x7FF, if IdType is FDCAN_STANDARD_ID , 0 and 0x1FFFFFFF, if IdType is FDCAN_EXTENDED_ID  */
  	  TxHeader1.TxFrameType         = FDCAN_DATA_FRAME;     /*  Specifies the frame type of the message that will be transmitted.  */
  	  TxHeader1.DataLength          = FDCAN_DLC_BYTES_8;    /*  the length of the transmitted frame 8 bytes.  */
  	  TxHeader1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;     /*  FDCAN_error_state_indicator,node is error active  */
  	  TxHeader1.BitRateSwitch       = FDCAN_BRS_OFF;        /*  frames transmitted/received without bit rate switching  */
  	  TxHeader1.FDFormat            = FDCAN_CLASSIC_CAN;    /*  Frame transmitted/received in Classic CAN format  */
  	  TxHeader1.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;   /*  Do not store Tx events */
  	  TxHeader1.MessageMarker       = 0;

  	//  /* Configure Rx filter */
  	  sFilterConfig.IdType       = FDCAN_EXTENDED_ID;
  	  sFilterConfig.FilterIndex  = 1;
  	  sFilterConfig.FilterType   = FDCAN_FILTER_RANGE;//
  	  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  	  sFilterConfig.FilterID1    = 0x800;
  	  sFilterConfig.FilterID2    = 0x835;


  	  /* Configure the FDCAN reception filter */
  	  if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK)
  	  {
  	    Error_Handler();
  	  }

  	  /* Start the FDCAN module */
  	  if (HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
  	  {
  	    Error_Handler();
  	  }
  	  /* Enable interrupts */
  	  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  	  {
  	    Error_Handler();
  	  }
  	  if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_TX_FIFO_EMPTY, 0) != HAL_OK) {
  	  		/* Notification Error */
  	  		Error_Handler();
  	  	}
  /* USER CODE END FDCAN2_Init 2 */

}

static uint32_t HAL_RCC_FDCAN_CLK_ENABLED=0;

void HAL_FDCAN_MspInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspInit 0 */

  /* USER CODE END FDCAN1_MspInit 0 */
    /* FDCAN1 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* FDCAN1 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN1_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspInit 1 */

  /* USER CODE END FDCAN1_MspInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspInit 0 */

  /* USER CODE END FDCAN2_MspInit 0 */
    /* FDCAN2 clock enable */
    HAL_RCC_FDCAN_CLK_ENABLED++;
    if(HAL_RCC_FDCAN_CLK_ENABLED==1){
      __HAL_RCC_FDCAN_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* FDCAN2 interrupt Init */
    HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE BEGIN FDCAN2_MspInit 1 */

  /* USER CODE END FDCAN2_MspInit 1 */
  }
}

void HAL_FDCAN_MspDeInit(FDCAN_HandleTypeDef* fdcanHandle)
{

  if(fdcanHandle->Instance==FDCAN1)
  {
  /* USER CODE BEGIN FDCAN1_MspDeInit 0 */

  /* USER CODE END FDCAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN1 GPIO Configuration
    PA11     ------> FDCAN1_RX
    PA12     ------> FDCAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    /* FDCAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN1_IT0_IRQn);
  /* USER CODE BEGIN FDCAN1_MspDeInit 1 */

  /* USER CODE END FDCAN1_MspDeInit 1 */
  }
  else if(fdcanHandle->Instance==FDCAN2)
  {
  /* USER CODE BEGIN FDCAN2_MspDeInit 0 */

  /* USER CODE END FDCAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_FDCAN_CLK_ENABLED--;
    if(HAL_RCC_FDCAN_CLK_ENABLED==0){
      __HAL_RCC_FDCAN_CLK_DISABLE();
    }

    /**FDCAN2 GPIO Configuration
    PB12     ------> FDCAN2_RX
    PB13     ------> FDCAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

    /* FDCAN2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(FDCAN2_IT0_IRQn);
  /* USER CODE BEGIN FDCAN2_MspDeInit 1 */

  /* USER CODE END FDCAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
