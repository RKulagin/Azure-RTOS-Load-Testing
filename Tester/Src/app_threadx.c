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
TX_THREAD ThreadPCSender;
TX_THREAD ThreadPCReceiver;
TX_EVENT_FLAGS_GROUP EventFlag;

TX_QUEUE QueueUART3Sender;
TX_QUEUE QueueUART3Receiver;
TX_QUEUE QueuePCSender;
TX_QUEUE QueuePCReceiver;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void MainThread_Entry(ULONG thread_input);
void ThreadUART3Sender_Entry(ULONG thread_input);
void ThreadUART3Receiver_Entry(ULONG thread_input);
void ThreadPCSender_Entry(ULONG thread_input);
void ThreadPCReceiver_Entry(ULONG thread_input);
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
  if (tx_thread_create(&ThreadUART3Sender, "Thread UART3 Sender", ThreadUART3Sender_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
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
  
  /* Create ThreadUART3Receiver.  */
  if (tx_thread_create(&ThreadUART3Receiver, "Thread UART3 Receiver", ThreadUART3Receiver_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadPCSender.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadPCSender.  */
  if (tx_thread_create(&ThreadPCSender, "Thread PC Sender", ThreadPCSender_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  /* Allocate the stack for ThreadPCReceiver.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadUART3Receiver.  */
  if (tx_thread_create(&ThreadPCReceiver, "Thread PC Receiver", ThreadPCReceiver_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       THREAD_TWO_PRIO, THREAD_TWO_PREEMPTION_THRESHOLD,
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

  /* Allocate queue for UART3 Receiver*/
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       QUEUE_UART3_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  /* Create queue for UART3 Receiver*/
  if (tx_queue_create(&QueueUART3Receiver, "UART3 Receiver Queue", TX_1_ULONG, pointer,
                      QUEUE_UART3_RECEIVER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

  /* Allocate queue for PC Sender*/
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       QUEUE_PC_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  /* Create queue for PC Sender*/
  if (tx_queue_create(&QueuePCSender, "PC Sender Queue", TX_1_ULONG, pointer,
                      QUEUE_PC_SENDER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

  /* Allocate queue for PC Receiver*/
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       QUEUE_PC_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  /* Create queue for PC Receiver*/
  if (tx_queue_create(&QueuePCReceiver, "PC Receiver Queue", TX_1_ULONG, pointer,
                      QUEUE_PC_RECEIVER_SIZE) != TX_SUCCESS)
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
}

/**
 * @brief   Function implementing the ThreadUART3Sender thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART3Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART3SenderData = malloc(UART3_MAX_MESSAGE_SIZE);
  // Wait until QueueUART3Sender is not empty
  while(1){
    tx_queue_receive(&QueueUART3Sender, &QueueUART3SenderData, TX_WAIT_FOREVER);
    HAL_UART_Transmit(&huart3, QueueUART3SenderData, strlen(QueueUART3SenderData), 1000);
  }
  free (QueueUART3SenderData);
}

/**
 * @brief   Function implementing the ThreadUART3Receiver thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART3Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART3ReceiverData;
  // Wait until QueueUART3Receiver is not empty
  while(1){
    tx_queue_receive(&QueueUART3Receiver, &QueueUART3ReceiverData, TX_WAIT_FOREVER);
    tx_queue_send(&QueuePCSender, &QueueUART3ReceiverData, TX_WAIT_FOREVER);
  }
}

/**
 * @brief   Function implementing the ThreadPCSender thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadPCSender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueuePCSenderData;
  // Wait until QueuePCSender is not empty
  while(1){
    tx_queue_receive(&QueuePCSender, &QueuePCSenderData, TX_WAIT_FOREVER);
    CDC_Transmit_FS(QueuePCSenderData, strlen(QueuePCSenderData));
  }
}

/**
 * @brief   Function implementing the ThreadPCReceiver thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadPCReceiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueuePCReceiverData;
  // Wait until QueuePCReceiver is not empty
  while(1){
    tx_queue_receive(&QueuePCReceiver, &QueuePCReceiverData, TX_WAIT_FOREVER);
    tx_queue_send(&QueueUART3Sender, &QueuePCReceiverData, TX_WAIT_FOREVER);
  }
}

/**
  * @brief  Function implementing the ThreadTwo thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadTwo_Entry(ULONG thread_input)
{
  UNUSED(thread_input);
  

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
