/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "app_threadx.h"
#include "stm32f4xx.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_THREAD ThreadThree;
TX_EVENT_FLAGS_GROUP EventFlag;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void ThreadThree_Entry(ULONG thread_input);

void App_Delay(uint32_t Delay);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer;

  /* Create ThreadOne.  */
  if (tx_thread_create(&ThreadOne, "Thread One", ThreadOne_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadTwo.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                         APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create ThreadTwo.  */
  if (tx_thread_create(&ThreadTwo, "Thread Two", ThreadTwo_Entry, 0,
                         pointer, APP_STACK_SIZE,
                         THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
                         TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadThree.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                           APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create ThreadThree.  */
  if (tx_thread_create(&ThreadThree, "Thread Three", ThreadThree_Entry, 0,
                           pointer, APP_STACK_SIZE,
                           THREAD_THREE_PRIO, THREAD_THREE_PREEMPTION_THRESHOLD,
                           TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }


  /* Create the event flags group.  */
  if (tx_event_flags_create(&EventFlag, "Event Flag") != TX_SUCCESS)
  {
    ret = TX_GROUP_ERROR;
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/**
  * @brief  Function implementing the ThreadOne thread
  * Exponentiation to the 2 degree, delay = 100ms.
  * USE UART_1
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadOne_Entry(ULONG thread_input)
{
	ULONG   actual_flags = 0;
	while(1) {

		if (tx_event_flags_get(&EventFlag, THREAD_ONE_EVT, TX_OR_CLEAR,
								   &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
		{
		   Error_Handler();
		}

		else
			{
				UINT value = atoi(buffer_1);

				res = value*value;

				// Delay = 100ms
				App_Delay(10);

				message = itoa(res);
				const uint8_t message_length = strlen(message);

				HAL_UART_Transmit(&huart1, &message_length,1, 100);
				HAL_UART_Transmit(&huart1, message, strlen(message), 1000);
				memset(buffer_1, 0, 256);
				HAL_UART_Receive_IT(&huart1, (uint8_t*)&RX_data, 1);
			}
	}
}

/**
  * @brief  Function implementing the ThreadTwo thread.
  * Exponentiation to the 4 degree, delay = 500ms.
  * USE UART_2
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadTwo_Entry(ULONG thread_input)
{
	ULONG   actual_flags = 0;
	while(1) {

		if (tx_event_flags_get(&EventFlag, THREAD_TWO_EVT, TX_OR_CLEAR,
								   &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
		{
		   Error_Handler();
		}

		else
			{
				UINT value = atoi(buffer_2);

				res = value*value*value*value;

				// Delay = 500ms
				App_Delay(50);

				message = itoa(res);
				const uint8_t message_length = strlen(message);

				HAL_UART_Transmit(&huart2, &message_length, 1, 100);
				HAL_UART_Transmit(&huart2, message, strlen(message), 1000);
				memset(buffer_2, 0, 256);
				HAL_UART_Receive_IT(&huart2, (uint8_t*)&RX_data, 1);
			}
	}
}

/**
  * @brief  Function implementing the ThreadThree thread.
  * Exponentiation to the 6 degree, delay = 8000ms.
  * USE UART_3
  * @retval None
  */
void ThreadThree_Entry(ULONG thread_input)
{
	ULONG   actual_flags = 0;
	while(1) {

		if (tx_event_flags_get(&EventFlag, THREAD_THREE_EVT, TX_OR_CLEAR,
								   &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
		{
		   Error_Handler();
		}

		else
			{
				UINT value = atoi(buffer_3);

				res = value*value*value*value*value*value;

				// Delay = 800ms
				App_Delay(80);

				message = itoa(res);
				const uint8_t message_length = strlen(message);

				HAL_UART_Transmit(&huart3, &message_length, 1, 100);
				HAL_UART_Transmit(&huart3, message, strlen(message), 1000);
				memset(buffer_3, 0, 256);
				HAL_UART_Receive_IT(&huart3, (uint8_t*)&RX_data, 1);
			}
	}
}

/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(uint32_t Delay)
{
  UINT initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}
/* USER CODE END 1 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
 if (huart == &huart1) {
  if (size_of_rx_data_1 == 0){
	  size_of_rx_data_1 = RX_data_1[0];
	  RX_data_1[0] = 0;
	  HAL_UART_Receive_IT(&huart1, (uint8_t*)&RX_data_1, size_of_rx_data_1);
  }
  else {
	  strcpy(buffer_1, RX_data);
	  size_of_rx_data_1 = 0;
	  memset(RX_data, 0, 256);
	  HAL_UART_Receive_IT(&huart1, (uint8_t*)&RX_data_1, 1);
	  // uart3_received = 1;

	  // устанавливаем флаг
	  if (tx_event_flags_set(&EventFlag, THREAD_ONE_EVT, TX_OR) != TX_SUCCESS)
	          {
	            Error_Handler();
	          }
  }
 } else {
	 if (huart == &huart2) {
	  if (size_of_rx_data_2 == 0){
		  size_of_rx_data_2 = RX_data_2[0];
		  RX_data_2[0] = 0;
		  HAL_UART_Receive_IT(&huart2, (uint8_t*)&RX_data_2, size_of_rx_data_2);
	  }
	  else {
		  strcpy(buffer_2, RX_data);
		  size_of_rx_data_2 = 0;
		  memset(RX_data, 0, 256);
		  HAL_UART_Receive_IT(&huart2, (uint8_t*)&RX_data_2, 1);
		  // uart3_received = 1;

		  // устанавливаем флаг
		  if (tx_event_flags_set(&EventFlag, THREAD_TWO_EVT, TX_OR) != TX_SUCCESS)
		          {
		            Error_Handler();
		          }
	  }
	 }
	 else {
	 	 if (huart == &huart3) {
	 	  if (size_of_rx_data_3 == 0){
	 		  size_of_rx_data_3 = RX_data_3[0];
	 		  RX_data_3[0] = 0;
	 		  HAL_UART_Receive_IT(&huart3, (uint8_t*)&RX_data_3, size_of_rx_data_3);
	 	  }
	 	  else {
	 		  strcpy(buffer_3, RX_data);
	 		  size_of_rx_data_3 = 0;
	 		  memset(RX_data, 0, 256);
	 		  HAL_UART_Receive_IT(&huart3, (uint8_t*)&RX_data_3, 1);
	 		  // uart3_received = 1;

	 		  // устанавливаем флаг
	 		  if (tx_event_flags_set(&EventFlag, THREAD_THREE_EVT, TX_OR) != TX_SUCCESS)
	 		          {
	 		            Error_Handler();
	 		          }
	 	  }
	 	 }
	  }
 }
}
