/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

#include <stdio.h>

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c2;

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 64;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();

    /* I2C2 interrupt Init */
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C2)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();

    /**I2C2 GPIO Configuration
    PB10     ------> I2C2_SCL
    PB11     ------> I2C2_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

    /* I2C2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

// emulated I2C RAM
static uint8_t ram[256];
static uint8_t offset;  // index of current RAM cell
static uint8_t first=1; // first byte --> new offset

#define PRINTF(...) printf(__VA_ARGS__)

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
  PRINTF("LCB\n");
  first = 1;
  HAL_I2C_EnableListen_IT(hi2c); // slave is ready again
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
  PRINTF("ACB %s\n", TransferDirection==I2C_DIRECTION_RECEIVE ? "rx" : "tx" );
  //HAL_GPIO_WritePin( LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET );
  if( TransferDirection==I2C_DIRECTION_TRANSMIT ) {
    if( first ) {
      HAL_I2C_Slave_Seq_Receive_IT(hi2c, &offset, 1, I2C_NEXT_FRAME);
    } else {
      HAL_I2C_Slave_Seq_Receive_IT(hi2c, &ram[offset], 1, I2C_NEXT_FRAME);
    }
  } else {
    HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &ram[offset], 1, I2C_NEXT_FRAME);
  }
  //HAL_GPIO_WritePin( LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET );
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  //HAL_GPIO_WritePin( PA6_GPIO_Port, PA6_Pin, GPIO_PIN_SET );
  if(first) {
    PRINTF("RXCB: offset <== %3d\n", offset );
    first = 0;
  } else {
    PRINTF("RXCB: ram[%3d] <== %3d\n", offset,  ram[offset] );
    offset++;
  }
  HAL_I2C_Slave_Seq_Receive_IT(hi2c, &ram[offset], 1, I2C_NEXT_FRAME);
  //HAL_GPIO_WritePin(PA6_GPIO_Port, PA6_Pin, GPIO_PIN_RESET );
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
  //HAL_GPIO_WritePin( PA7_GPIO_Port, PA7_Pin, GPIO_PIN_SET );
  PRINTF("TXCB: ram[%3d] ==> %3d\n", offset, ram[offset] );
  offset++;
  HAL_I2C_Slave_Seq_Transmit_IT(hi2c, &ram[offset], 1, I2C_NEXT_FRAME);
  //HAL_GPIO_WritePin( PA7_GPIO_Port, PA7_Pin, GPIO_PIN_RESET );
}

/* USER CODE END 1 */
