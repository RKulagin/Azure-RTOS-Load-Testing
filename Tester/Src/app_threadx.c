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
TX_THREAD MainThread;
TX_THREAD ThreadOne;
TX_EVENT_FLAGS_GROUP EventFlag;

GPIO_InitTypeDef gpio;
USART_InitTypeDef usart;
uint8_t sendData[8];
uint8_t bytesToSend = 8;
uint8_t sendDataCounter = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void MainThread_Entry(ULONG thread_input);
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

  /* Allocate the stack for MainThread.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create MainThread.  */
  if (tx_thread_create(&MainThread, "Main Thread", MainThread_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       MAIN_THREAD_PRIO, MAIN_THREAD_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }
  
  /* Allocate the stack for ThreadOne.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadOne.  */
  if (tx_thread_create(&ThreadOne, "Thread One", ThreadOne_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
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
  * @brief  Function implementing the MainThread thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void MainThread_Entry(ULONG thread_input)
{
  (void) thread_input;
  uint8_t count = 0;
  
  while(1)
    {
      BSP_LED_Toggle(LED_BLUE);
      /* Delay for 500ms (App_Delay is used to avoid context change). */
      App_Delay(50);
      count ++;
      if (count == 6)
      {
        count = 0;
        if (tx_event_flags_set(&EventFlag, THREAD_MAIN_EVT, TX_OR) != TX_SUCCESS)
        {
          Error_Handler();
        }
      }
    }
  
}

/**
  * @brief  Function implementing the ThreadOne thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadOne_Entry(ULONG thread_input)
{
	ULONG   actual_flags = 0;
	while (1)
	  {
	    if (tx_event_flags_get(&EventFlag, THREAD_MAIN_EVT, TX_OR_CLEAR,
	                           &actual_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
	    {
	      Error_Handler();
	    }
	    else
	    {
	    	BSP_LED_Toggle(LED_GREEN);
	    	      /* Delay for 500ms (App_Delay is used to avoid context change). */
	    	  // App_Delay(50);



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

