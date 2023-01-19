/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "crc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "sine_approx.h"
#include "sine_approx_data.h"
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
char RX_BUFF[BUFF_SIZE];
char TX_BUFF[MSG_BUFF];
int RX_BUFF_HEAD, RX_BUFF_TAIL;
uint8_t rx_data;
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
	char buff[BUFF_SIZE];
	int buf_len = 0;
	ai_error ai_err;
	ai_i32 nbatch;
	uint32_t timestamp;
	float y_val;

	uint8_t *tmp = 1;

	AI_ALIGNED(4) ai_u8 activations[AI_SINE_APPROX_DATA_ACTIVATIONS_SIZE];

	AI_ALIGNED(4) ai_i8 in_data[AI_SINE_APPROX_IN_1_SIZE_BYTES];
	AI_ALIGNED(4) ai_i8 out_data[AI_SINE_APPROX_OUT_1_SIZE_BYTES];

	ai_handle sine_model = AI_HANDLE_NULL;

	ai_buffer ai_input[AI_SINE_APPROX_IN_NUM] = AI_SINE_APPROX_IN;
	ai_buffer ai_output[AI_SINE_APPROX_OUT_NUM] = AI_SINE_APPROX_OUT;

	ai_network_params ai_params = {
			AI_SINE_APPROX_DATA_WEIGHTS(ai_sine_approx_data_weights_get()),
			AI_SINE_APPROX_DATA_ACTIVATIONS(activations)
	};

	ai_input[0].n_batches = 1;
	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].n_batches = 1;
	ai_output[0].data = AI_HANDLE_PTR(out_data);
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
  MX_CRC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  buf_len = sprintf(buff, "\r\n\r\nSTM32 X-CUBE-AI Test\r\n");

  HAL_UART_Transmit_IT(&huart1, (uint8_t *)buff, buf_len);

  ai_err = ai_sine_approx_create(&sine_model, AI_SINE_APPROX_DATA_CONFIG);
  if (ai_err.type != AI_ERROR_NONE)
  {
	  buf_len = sprintf(buff, "Error: could not create NN instance\r\n");
	  HAL_UART_Transmit(&huart1, (uint8_t *)buff, buf_len, 100);
	  while(1);
  }

  if (!ai_sine_approx_init(sine_model, &ai_params))
  {
    buf_len = sprintf(buff, "Error: could not initialize NN\r\n");
    HAL_UART_Transmit(&huart1, (uint8_t *)buff, buf_len, 100);
    while(1);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	for (uint32_t i = 0; i < AI_SINE_APPROX_IN_1_SIZE; i++)
	{
	  ((ai_float *)in_data)[i] = (ai_float)2.0f;
	}

	// Perform inference
	nbatch = ai_sine_approx_run(sine_model, &ai_input[0], &ai_output[0]);
	if (nbatch != 1) {
	  buf_len = sprintf(buff, "Error: could not run inference\r\n");
	  HAL_UART_Transmit_IT(&huart1, (uint8_t *)buff, buf_len);
	}

	// Read output (predicted y) of neural network
	y_val = ((float *)out_data)[0];

	// Print output of neural network along with inference time (microseconds)
	buf_len = sprintf(buff,"Output: %f \r\n",y_val);
	HAL_UART_Transmit_IT(&huart1, (uint8_t *)buff, buf_len);

	// Wait before doing it again
	HAL_Delay(1000);
    /* USER CODE END WHILE */

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
