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
TX_THREAD ThreadUART4Sender;
TX_THREAD ThreadUART4Receiver;
TX_THREAD ThreadUART5Sender;
TX_THREAD ThreadUART5Receiver;
TX_THREAD ThreadUART6Sender;
TX_THREAD ThreadUART6Receiver;
TX_THREAD ThreadPCSender;
TX_THREAD ThreadPCReceiver;
TX_EVENT_FLAGS_GROUP EventFlag;

TX_QUEUE QueueUART4Sender;
TX_QUEUE QueueUART4Receiver;
TX_QUEUE QueueUART5Sender;
TX_QUEUE QueueUART5Receiver;
TX_QUEUE QueueUART6Sender;
TX_QUEUE QueueUART6Receiver;
TX_QUEUE QueuePCSender;
TX_QUEUE QueuePCReceiver;

TX_QUEUE QueueUART4TimeStart;
TX_QUEUE QueueUART4TimeFinish;
TX_QUEUE QueueUART5TimeStart;
TX_QUEUE QueueUART5TimeFinish;
TX_QUEUE QueueUART6TimeStart;
TX_QUEUE QueueUART6TimeFinish;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void MainThread_Entry(ULONG thread_input);
void ThreadUART4Sender_Entry(ULONG thread_input);
void ThreadUART4Receiver_Entry(ULONG thread_input);
void ThreadUART5Sender_Entry(ULONG thread_input);
void ThreadUART5Receiver_Entry(ULONG thread_input);
void ThreadUART6Sender_Entry(ULONG thread_input);
void ThreadUART6Receiver_Entry(ULONG thread_input);
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
  
  // THREADS BEGIN
  
		  /* Create MainThread.  */
		  if (tx_thread_create(&MainThread, "Main Thread", MainThread_Entry, 0,
							   pointer, APP_STACK_SIZE,
							   MAIN_THREAD_PRIO, MAIN_THREAD_PREEMPTION_THRESHOLD,
							   TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
		  {
			ret = TX_THREAD_ERROR;
		  }

		  /* Allocate the stack for ThreadUART4Sender.  */
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create ThreadUART4Sender.  */
		  if (tx_thread_create(&ThreadUART4Sender, "Thread UART4 Sender", ThreadUART4Sender_Entry, 0,
							   pointer, APP_STACK_SIZE,
							   THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
							   TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
		  {
			ret = TX_THREAD_ERROR;
		  }

		  /* Allocate the stack for ThreadUART4Receiver.  */
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create ThreadUART4Receiver.  */
		  if (tx_thread_create(&ThreadUART4Receiver, "Thread UART4 Receiver", ThreadUART4Receiver_Entry, 0,
							   pointer, APP_STACK_SIZE,
							   THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
							   TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
		  {
			ret = TX_THREAD_ERROR;
		  }

		  /* Allocate the stack for ThreadUART5Sender.  */
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create ThreadUART5Sender.  */
		  if (tx_thread_create(&ThreadUART5Sender, "Thread UART5 Sender", ThreadUART5Sender_Entry, 0,
							   pointer, APP_STACK_SIZE,
							   THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
							   TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
		  {
			ret = TX_THREAD_ERROR;
		  }

		  /* Allocate the stack for ThreadUART5Receiver.  */
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create ThreadUART5Receiver.  */
		  if (tx_thread_create(&ThreadUART5Receiver, "Thread UART5 Receiver", ThreadUART5Receiver_Entry, 0,
							   pointer, APP_STACK_SIZE,
							   THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
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
		  if (tx_thread_create(&ThreadUART6Sender, "Thread UART6 Sender", ThreadUART6Sender_Entry, 0,
							   pointer, APP_STACK_SIZE,
							   THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
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

		  /* Create ThreadUART6Receiver.  */
		  if (tx_thread_create(&ThreadUART6Receiver, "Thread UART6 Receiver", ThreadUART6Receiver_Entry, 0,
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

  // THREADS END

  // EVENT FLAGS AND QUEUE BEGIN

		  /* Create the event flags group.  */
		  if (tx_event_flags_create(&EventFlag, "Event Flag") != TX_SUCCESS)
		  {
			ret = TX_GROUP_ERROR;
		  }
		  /* USER CODE END App_ThreadX_Init */

		  /* Allocate queue for UART4 Sender*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   QUEUE_UART4_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create queue for UART4 Sender*/
		  if (tx_queue_create(&QueueUART4Sender, "UART4 Sender Queue", TX_1_ULONG, pointer,
							  QUEUE_UART4_SENDER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  /* Allocate queue for UART4 Receiver*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   QUEUE_UART4_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create queue for UART4 Receiver*/
		  if (tx_queue_create(&QueueUART4Receiver, "UART4 Receiver Queue", TX_1_ULONG, pointer,
							  QUEUE_UART4_RECEIVER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  /* Allocate queue for UART5 Sender*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   QUEUE_UART5_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create queue for UART5 Sender*/
		  if (tx_queue_create(&QueueUART5Sender, "UART5 Sender Queue", TX_1_ULONG, pointer,
							  QUEUE_UART5_SENDER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  /* Allocate queue for UART5 Receiver*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   QUEUE_UART5_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create queue for UART5 Receiver*/
		  if (tx_queue_create(&QueueUART5Receiver, "UART5 Receiver Queue", TX_1_ULONG, pointer,
							  QUEUE_UART5_RECEIVER_SIZE) != TX_SUCCESS)
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

		  /* Allocate queue for UART6 Receiver*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							   QUEUE_UART6_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }

		  /* Create queue for UART6 Receiver*/
		  if (tx_queue_create(&QueueUART6Receiver, "UART6 Receiver Queue", TX_1_ULONG, pointer,
							  QUEUE_UART6_RECEIVER_SIZE) != TX_SUCCESS)
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

  // EVENT FLAGS AND QUEUE END
  
  // TIME QUEUE BEGIN

		  // UART 4
  
		  /* Allocate queue for Times Start for UART4*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							  QUEUE_UART4_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }
		  /* Create queue for Times Start for UART4*/
		  if (tx_queue_create(&QueueUART4TimeStart, "UART4 Time Start Queue", TX_1_ULONG, pointer,
							  QUEUE_UART4_SENDER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  /* Allocate queue for Times Finish for UART4*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							  QUEUE_UART4_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }
		  /* Create queue for Times Finish for UART4*/
		  if (tx_queue_create(&QueueUART4TimeFinish, "UART4 Time Finish Queue", TX_1_ULONG, pointer,
							  QUEUE_UART4_RECEIVER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  // UART 5

		  /* Allocate queue for Times Start for UART5*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							  QUEUE_UART5_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }
		  /* Create queue for Times Start for UART5*/
		  if (tx_queue_create(&QueueUART5TimeStart, "UART5 Time Start Queue", TX_1_ULONG, pointer,
							  QUEUE_UART5_SENDER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  /* Allocate queue for Times Finish for UART5*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							  QUEUE_UART5_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }
		  /* Create queue for Times Finish for UART5*/
		  if (tx_queue_create(&QueueUART5TimeFinish, "UART5 Time Finish Queue", TX_1_ULONG, pointer,
							  QUEUE_UART5_RECEIVER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  // UART 6

		  /* Allocate queue for Times Start for UART6*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							  QUEUE_UART6_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }
		  /* Create queue for Times Start for UART6*/
		  if (tx_queue_create(&QueueUART6TimeStart, "UART6 Time Start Queue", TX_1_ULONG, pointer,
							  QUEUE_UART6_SENDER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }

		  /* Allocate queue for Times Finish for UART6*/
		  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
							  QUEUE_UART6_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
		  {
			ret = TX_POOL_ERROR;
		  }
		  /* Create queue for Times Finish for UART6*/
		  if (tx_queue_create(&QueueUART6TimeFinish, "UART6 Time Finish Queue", TX_1_ULONG, pointer,
							  QUEUE_UART6_RECEIVER_SIZE) != TX_SUCCESS)
		  {
			ret = TX_QUEUE_ERROR;
		  }
  
  // TIME QUEUE END

  /* Error Handler.  */
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
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
 * @brief   Function implementing the ThreadUART4Sender thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART4Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART4SenderData = malloc(UART4_MAX_MESSAGE_SIZE);
  uint8_t message_size;
  ULONG actual_flags;
  // Wait until QueueUART3Sender is not empty
  tx_event_flags_get(&EventFlag, EVENT_FLAG_RUN_UART4, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
  while(1){
    tx_queue_receive(&QueueUART4Sender, &QueueUART4SenderData, TX_WAIT_FOREVER);
    QueueUART4SenderData[3] = 0;
    message_size = strlen(QueueUART4SenderData);

    tx_queue_send(&QueueUART5TimeStart, tx_time_get(), TX_WAIT_FOREVER);

    HAL_UART_Transmit(&huart4, &message_size, 1, 1000);
    HAL_UART_Transmit(&huart4, QueueUART4SenderData, message_size, 10000);
    tx_thread_sleep(10);
  }
  free (QueueUART4SenderData);
}

/**
 * @brief   Function implementing the ThreadUART4Receiver thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART4Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART4ReceiverData;
  ULONG QueueUART4ReceiverStart;
  ULONG QueueUART4ReceiverFinish;

  // Wait until QueueUART3Receiver is not empty
  while(1){
    uint8_t *message = malloc(UART4_MAX_MESSAGE_SIZE+strlen("UART4:"));
    strcpy(message, "UART4:");
    tx_queue_receive(&QueueUART4Receiver, &QueueUART4ReceiverData, TX_WAIT_FOREVER);
    strcat(message, QueueUART4ReceiverData);
    tx_queue_send(&QueuePCSender, &message, TX_WAIT_FOREVER);

    uint8_t *message1 = malloc(sizeof(ULONG)+strlen("UART4ST:"));
    strcpy(message1, "UART4ST:");
    tx_queue_receive(&QueueUART4TimeStart, QueueUART4ReceiverStart, TX_WAIT_FOREVER);
    itoa(QueueUART4ReceiverStart, message1, 10);
    strcat(message1, QueueUART4ReceiverStart);
    tx_queue_send(&QueuePCSender, &message1, TX_WAIT_FOREVER);

    uint8_t *message2 = malloc(sizeof(ULONG)+strlen("UART4RV:"));
    strcpy(message2, "UART4RV:");
    tx_queue_receive(&QueueUART4TimeFinish, QueueUART4ReceiverFinish, TX_WAIT_FOREVER);
    itoa(QueueUART4ReceiverFinish, message2, 10);
    strcat(message2, QueueUART4ReceiverFinish);
    tx_queue_send(&QueuePCSender, &message2, TX_WAIT_FOREVER);

    free(QueueUART4ReceiverData);
  }
}

/**
 * @brief   Function implementing the ThreadUART5Sender thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART5Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART5SenderData = malloc(UART5_MAX_MESSAGE_SIZE);
  uint8_t message_size;
  ULONG actual_flags;
  // Wait until QueueUART3Sender is not empty
  tx_event_flags_get(&EventFlag, EVENT_FLAG_RUN_UART5, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
  while(1){
    tx_queue_receive(&QueueUART5Sender, &QueueUART5SenderData, TX_WAIT_FOREVER);
    QueueUART5SenderData[3] = 0;
    message_size = strlen(QueueUART5SenderData);

    tx_queue_send(&QueueUART5TimeStart, tx_time_get(), TX_WAIT_FOREVER);

    HAL_UART_Transmit(&huart5, &message_size, 1, 1000);
    HAL_UART_Transmit(&huart5, QueueUART5SenderData, message_size, 10000);
    tx_thread_sleep(10);
  }
  free (QueueUART5SenderData);
}

/**
 * @brief   Function implementing the ThreadUART5Receiver thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART5Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART5ReceiverData;
  ULONG QueueUART5ReceiverStart;
  ULONG QueueUART5ReceiverFinish;

  // Wait until QueueUART5Receiver is not empty
  while(1){
    uint8_t *message = malloc(UART5_MAX_MESSAGE_SIZE+strlen("UART5:"));
    strcpy(message, "UART5:");
    tx_queue_receive(&QueueUART5Receiver, &QueueUART5ReceiverData, TX_WAIT_FOREVER);
    strcat(message, QueueUART5ReceiverData);
    tx_queue_send(&QueuePCSender, &message, TX_WAIT_FOREVER);

    uint8_t *message1 = malloc(UART5_MAX_MESSAGE_SIZE+strlen("UART5ST:"));
    strcpy(message1, "UART5ST: ");
    tx_queue_receive(&QueueUART5TimeStart, QueueUART5ReceiverStart, TX_WAIT_FOREVER);
    itoa(QueueUART5ReceiverStart, message1, 10);
    strcat(message1, QueueUART5ReceiverStart);
    tx_queue_send(&QueuePCSender, &message1, TX_WAIT_FOREVER);

    uint8_t *message2 = malloc(UART5_MAX_MESSAGE_SIZE+strlen("UART5RV:"));
    strcpy(message2, "UART5RV:");
    tx_queue_receive(&QueueUART5TimeFinish, QueueUART5ReceiverFinish, TX_WAIT_FOREVER);
    itoa(QueueUART5ReceiverFinish, message2, 10);
    strcat(message2, QueueUART5ReceiverFinish);
    tx_queue_send(&QueuePCSender, &message2, TX_WAIT_FOREVER);

    free(QueueUART5ReceiverData);
  }
}


/**
 * @brief   Function implementing the ThreadUART6Sender thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART6Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART6SenderData;
  uint8_t message_size;
  ULONG actual_flags;
  // Wait until QueueUART6Sender is not empty
  tx_event_flags_get(&EventFlag, EVENT_FLAG_RUN_UART6, TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);
  while(1){
    tx_queue_receive(&QueueUART6Sender, &QueueUART6SenderData, TX_WAIT_FOREVER);
    QueueUART6SenderData[3] = 0;
    message_size = strlen(QueueUART6SenderData);

    tx_queue_send(&QueueUART6TimeStart, tx_time_get(), TX_WAIT_FOREVER);

    HAL_UART_Transmit(&huart6, &message_size, 1, 1000);
    HAL_UART_Transmit(&huart6, QueueUART6SenderData, message_size, 10000);
    tx_thread_sleep(10);
  }
}

/**
 * @brief   Function implementing the ThreadUART6Receiver thread.
 * @param   thread_input: Not used
 * @retval  None
*/
void ThreadUART6Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t *QueueUART6ReceiverData;
  ULONG QueueUART6ReceiverStart;
  ULONG QueueUART6ReceiverFinish;

  // Wait until QueueUART6Receiver is not empty
  while(1){
    uint8_t *message = malloc(UART6_MAX_MESSAGE_SIZE+strlen("UART6:"));
    strcpy(message, "UART6:");
    tx_queue_receive(&QueueUART6Receiver, &QueueUART6ReceiverData, TX_WAIT_FOREVER);
    strcat(message, QueueUART6ReceiverData);
    tx_queue_send(&QueuePCSender, &message, TX_WAIT_FOREVER);

    uint8_t *message1 = malloc(UART6_MAX_MESSAGE_SIZE+strlen("UART6ST:"));
    strcpy(message1, "UART6ST:");
    tx_queue_receive(&QueueUART6TimeStart, QueueUART6ReceiverStart, TX_WAIT_FOREVER);
    itoa(QueueUART6ReceiverStart, message1, 10);
    strcat(message1, QueueUART6ReceiverStart);
    tx_queue_send(&QueuePCSender, &message1, TX_WAIT_FOREVER);

    uint8_t *message2 = malloc(UART6_MAX_MESSAGE_SIZE+strlen("UART6RV:"));
    strcpy(message2, "UART6RV:");
    tx_queue_receive(&QueueUART6TimeFinish, QueueUART6ReceiverFinish, TX_WAIT_FOREVER);
    itoa(QueueUART6ReceiverFinish, message2, 10);
    strcat(message2, QueueUART6ReceiverFinish);
    tx_queue_send(&QueuePCSender, &message2, TX_WAIT_FOREVER);

    free(QueueUART6ReceiverData);
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
    if (QueuePCReceiverData[0] == '4' && QueuePCReceiverData[1] == ','){
        uint8_t *Message = malloc((strlen(QueuePCReceiverData)-1) * sizeof(uint8_t));
        memcpy(Message, QueuePCReceiverData + 2, strlen(QueuePCReceiverData)-2);
        Message[strlen(QueuePCReceiverData)-2] = 0;
        tx_queue_send(&QueueUART4Sender, &Message, TX_WAIT_FOREVER);
    } else if (QueuePCReceiverData[0] == '5' && QueuePCReceiverData[1] == ','){
        uint8_t *Message = malloc((strlen(QueuePCReceiverData)-1) * sizeof(uint8_t));
        memcpy(Message, QueuePCReceiverData + 2, strlen(QueuePCReceiverData)-2);
        Message[strlen(QueuePCReceiverData)-2] = 0;
        tx_queue_send(&QueueUART5Sender, &Message, TX_WAIT_FOREVER);
    } else if (QueuePCReceiverData[0] == '6' && QueuePCReceiverData[1] == ','){
        uint8_t *Message = malloc((strlen(QueuePCReceiverData)-1) * sizeof(uint8_t));
        memcpy(Message, QueuePCReceiverData + 2, strlen(QueuePCReceiverData)-2);
        Message[strlen(QueuePCReceiverData)-2] = 0;
        tx_queue_send(&QueueUART6Sender, &Message, TX_WAIT_FOREVER);
    } else if (strcmp(QueuePCReceiverData, "Hello") == 0){
          uint8_t *Message = malloc((strlen("Azure RTOS Tester v0.1\n")+1) * sizeof(uint8_t));
          strcpy(Message, "Azure RTOS Tester v0.1\n");
          Message[strlen("Azure RTOS Tester v0.1\n")] = 0;
          tx_queue_send(&QueuePCSender, &Message, TX_WAIT_FOREVER);
    } else if (strcmp(QueuePCReceiverData, "Run") == 0){
    		  if (tx_event_flags_set(&EventFlag, 7, TX_OR) != HAL_OK){
    	          uint8_t *Message = malloc((strlen("Can't start\n")+1) * sizeof(uint8_t));
    	          strcpy(Message, "Can't start\n");
    	          Message[strlen("Can't start\n")] = 0;
    	          tx_queue_send(&QueuePCSender, &Message, TX_WAIT_FOREVER);
    		  }
    	  }
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
