/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("enabling I2C listen interrupt...\r\n");

  #define MASTER_REQ_READ    0x12
  #define MASTER_REQ_WRITE   0x34

  #define RXBUFFERSIZE 16

  /* Buffer used for reception */
  uint8_t aRxBuffer[RXBUFFERSIZE];
  uint8_t aTxBuffer[] = "1234567890123456";
  uint16_t hTxNumData = 0, hRxNumData = 0;
  uint8_t bTransferRequest = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    //https://github.com/STMicroelectronics/STM32CubeF1/blob/master/Projects/STM32F103RB-Nucleo/Examples/I2C/I2C_TwoBoards_AdvComIT/Src/main.c
      /* Initialize number of data variables */
      hTxNumData = 0;
      hRxNumData = 0;

      /*##-2- Slave receive request from master ################################*/
      printf("SR ?\n");
      while(HAL_I2C_Slave_Receive_IT(&hi2c2, (uint8_t*)&bTransferRequest, 1)!= HAL_OK)
      {
      }

      /*  Before starting a new communication transfer, you need to check the current
      state of the peripheral; if it’s busy you need to wait for the end of current
      transfer before starting a new one.
      For simplicity reasons, this example is just waiting till the end of the
      transfer, but application may perform other tasks while transfer operation
      is ongoing. */
      printf("RDY?\n");
      while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
      {
      }

      printf("Transfer Request 0x%x\n", bTransferRequest);
      /* If master request write operation #####################################*/
      if (bTransferRequest == MASTER_REQ_WRITE)
      {
        printf("Master Write\n");
        /*##-3- Slave receive number of data to be read ########################*/
        while(HAL_I2C_Slave_Receive(&hi2c2, (uint8_t*)&hRxNumData, 2, 0xFFFF)!= HAL_OK);

        /*  Before starting a new communication transfer, you need to check the current
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */
        printf(" - RDY?\n");
        while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
        {
        }
        printf(" - Nbytes 0x%x\n", hRxNumData);

        printf(" - RX!\n");
        /*##-4- Slave receives aRxBuffer from master ###########################*/
        while(HAL_I2C_Slave_Receive(&hi2c2, (uint8_t*)aRxBuffer, hRxNumData, 0xFFFF)!= HAL_OK);

        /*  Before starting a new communication transfer, you need to check the current
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */
        printf(" - RDY?\n");
        while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
        {
        }

        printf(" - done\n");
        /* Toggle LED2 */
        //BSP_LED_Toggle(LED2);
      }
      /* If master request write operation #####################################*/
      else if (bTransferRequest == MASTER_REQ_READ)
      {
        printf("Master Read\n");
        /*##-3- Slave receive number of data to be written #####################*/
        while(HAL_I2C_Slave_Receive(&hi2c2, (uint8_t*)&hTxNumData, 2, 0xFFFF)!= HAL_OK);

        /*  Before starting a new communication transfer, you need to check the current
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */
        printf(" - RDY?\n");
        while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
        {
        }
        printf(" - Nbytes 0x%x\n", hTxNumData);

        printf(" - TX!\n");
        /*##-4- Slave transmit aTxBuffer to master #############################*/
        while(HAL_I2C_Slave_Transmit(&hi2c2, (uint8_t*)aTxBuffer, hTxNumData, 0xFFFF)!= HAL_OK);

        /*  Before starting a new communication transfer, you need to check the current
        state of the peripheral; if it’s busy you need to wait for the end of current
        transfer before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the
        transfer, but application may perform other tasks while transfer operation
        is ongoing. */

        printf(" - RDY?\n");
        while (HAL_I2C_GetState(&hi2c2) != HAL_I2C_STATE_READY)
        {
          printf("   - current state: 0x%x\n", HAL_I2C_GetState(&hi2c2));
          HAL_Delay(1000);
        }
        printf(" - done\n");
      }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
