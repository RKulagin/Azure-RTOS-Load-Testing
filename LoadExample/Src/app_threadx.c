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
TX_THREAD ThreadUART3Sender;
TX_THREAD ThreadUART3Receiver;
TX_THREAD ThreadUART6Sender;
TX_THREAD ThreadUART6Receiver;
TX_EVENT_FLAGS_GROUP EventFlag;

TX_QUEUE QueueUART3Sender;
TX_QUEUE QueueUART3Receiver;
TX_QUEUE QueueUART6Sender;
TX_QUEUE QueueUART6Receiver;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadUART3Sender_Entry(ULONG thread_input);
void ThreadUART3Receiver_Entry(ULONG thread_input);
void ThreadUART6Sender_Entry(ULONG thread_input);
void ThreadUART6Receiver_Entry(ULONG thread_input);
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
  
  /* Allocate the stack for ThreadUART3Sender.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadUART3Sender.  */
  if (tx_thread_create(&ThreadUART3Sender, "UART3 Sender Thread", ThreadUART3Sender_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       UART3_SENDER_THREAD_PRIO, UART3_SENDER_THREAD_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadUART3Receiver.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadUART3Receiver. */
  if (tx_thread_create(&ThreadUART3Receiver, "UART3 Receiver Thread", ThreadUART3Receiver_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       UART3_RECEIVER_THREAD_PRIO, UART3_RECEIVER_THREAD_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadUART6Sender.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadUART6Sender.  */
  if (tx_thread_create(&ThreadUART6Sender, "UART6 Sender Thread", ThreadUART6Sender_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       UART6_SENDER_THREAD_PRIO, UART6_SENDER_THREAD_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadUART6Receiver.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadUART6Receiver. */
  if (tx_thread_create(&ThreadUART6Receiver, "UART6 Receiver Thread", ThreadUART6Receiver_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       UART6_RECEIVER_THREAD_PRIO, UART6_RECEIVER_THREAD_PREEMPTION_THRESHOLD,
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

   /* Allocate queue for UART3 Sender*/
   if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART3_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  /* Create queue for UART3 Sender*/
  if (tx_queue_create(&QueueUART3Sender, "UART3 Sender Queue", TX_1_ULONG, pointer,
                      QUEUE_UART3_SENDER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

  /* Allocate queue for UART3 Receiver. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART3_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  /* Create queue for UART3 Receiver. */
  if (tx_queue_create(&QueueUART3Receiver, "UART3 Receiver Queue", TX_1_ULONG, pointer,
                      QUEUE_UART3_RECEIVER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }
   /* Allocate queue for UART6 Sender*/
   if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART6_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

    /* Create queue for UART6 Sender*/
  if (tx_queue_create(&QueueUART6Sender, "UART6 Sender Queue", TX_1_ULONG, pointer,
                      QUEUE_UART6_SENDER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

  /* Allocate queue for UART6 Receiver. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART6_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  /* Create queue for UART6 Receiver. */
  if (tx_queue_create(&QueueUART6Receiver, "UART6 Receiver Queue", TX_1_ULONG, pointer,
                      QUEUE_UART6_RECEIVER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

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
  UNUSED(thread_input);
  /* Infinite loop */
  while (1){
    tx_thread_sleep(10000);
  }
}

/**
  * @brief  Function implementing the ThreadUART3Receiver_Entry thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadUART3Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART3ReceiverData;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART3Receiver, &QueueUART3ReceiverData, TX_WAIT_FOREVER);
    // Place for some logic
    UINT value = atoi(QueueUART3ReceiverData);
    UINT res = value*value;

    // Delay = 100ms
//    App_Delay(10);
    tx_thread_sleep(10);
    char* message = malloc(256);
    memset(message, 0, 256);
    itoa(res, message, 10);
    message[strlen(message)] = '\n';
//    const uint8_t message_length = strlen(message);


    tx_queue_send(&QueueUART3Sender, &message, TX_WAIT_FOREVER);
  }
}

/**
  * @brief  Function implementing the ThreadUART3Sender_Entry thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadUART3Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART3SenderData;
  uint8_t message_size;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART3Sender, &QueueUART3SenderData, TX_WAIT_FOREVER);
    message_size = strlen(QueueUART3SenderData);
    HAL_UART_Transmit(&huart3, &message_size, 1, 1000);
    HAL_UART_Transmit(&huart3, QueueUART3SenderData, message_size, 10000);
  }
}


/**
  * @brief  Function implementing the ThreadUART6Receiver_Entry thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadUART6Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART6ReceiverData;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART6Receiver, &QueueUART6ReceiverData, TX_WAIT_FOREVER);
    // Place for some logic
    UINT value = atoi(QueueUART6ReceiverData);
    UINT res = value*value*value;

    // Delay = 100ms
//    App_Delay(10);
    tx_thread_sleep(10);
    char* message = malloc(256);
    memset(message, 0, 256);
    itoa(res, message, 10);
    message[strlen(message)] = '\n';
//    const uint8_t message_length = strlen(message);


    tx_queue_send(&QueueUART6Sender, &message, TX_WAIT_FOREVER);
  }
}

/**
  * @brief  Function implementing the ThreadUART6Sender_Entry thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadUART6Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART6SenderData;
  uint8_t message_size;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART6Sender, &QueueUART6SenderData, TX_WAIT_FOREVER);
    message_size = strlen(QueueUART6SenderData);
    HAL_UART_Transmit(&huart6, &message_size, 1, 1000);
    HAL_UART_Transmit(&huart6, QueueUART6SenderData, message_size, 10000);
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
