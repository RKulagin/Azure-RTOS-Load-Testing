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
#include "time.h"


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
TX_EVENT_FLAGS_GROUP EventFlag;

TX_QUEUE QueueUART4Sender;
TX_QUEUE QueueUART4Receiver;
TX_QUEUE QueueUART5Sender;
TX_QUEUE QueueUART5Receiver;
TX_QUEUE QueueUART6Sender;
TX_QUEUE QueueUART6Receiver;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadUART4Sender_Entry(ULONG thread_input);
void ThreadUART4Receiver_Entry(ULONG thread_input);
void ThreadUART5Sender_Entry(ULONG thread_input);
void ThreadUART5Receiver_Entry(ULONG thread_input);
void ThreadUART6Sender_Entry(ULONG thread_input);
void ThreadUART6Receiver_Entry(ULONG thread_input);
void MainThread_Entry(ULONG thread_input);
void App_Delay(uint32_t Delay);

// C[N,N] = A[N,N] * B[N,N]
void MatrixMult(UINT **A, UINT **B, UINT **C, UINT N){

	// умножаем
	for(UINT i = 0; i < N; i++)
	    for(UINT j = 0; j < N; j++)
	    {
	        C[i][j] = 0;
	        for(UINT k = 0; k < N; k++)
	        	C[i][j] += A[i][k] * B[k][j];
	    }
}

void getMatrixWithoutRowAndCol(UINT **matrix, UINT size, UINT row, UINT col, UINT **newMatrix) {
	UINT offsetRow = 0;
	UINT offsetCol = 0;
    for(UINT i = 0; i < size-1; i++) {
        if(i == row) {
            offsetRow = 1;
        }

        offsetCol = 0;
        for(UINTj = 0; j < size-1; j++) {

            if(j == col) {
                offsetCol = 1;
            }

            newMatrix[i][j] = matrix[i + offsetRow][j + offsetCol];
        }
    }
}

UINT matrixDet(UINT **matrix, UINT size) {
	UINT det = 0;
	UINT degree = 1;

    if(size == 1) {
        return matrix[0][0];
    } else if(size == 2) {
        return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
    }
    else {
    	UINT **newMatrix = (UINT**)malloc(N * sizeof(UINT*));
    	for (UINT i = 0; i < size; i++) {
    		newMatrix[i] = (UINT*)malloc(N * sizeof(UINT));
    	}

        for(UINT j = 0; j < size; j++) {
            getMatrixWithoutRowAndCol(matrix, size, 0, j, newMatrix);
            det = det + (degree * matrix[0][j] * matrixDet(newMatrix, size-1));
            degree = -degree;
        }

        for(UINT i = 0; i < size-1; i++) {
        	free(newMatrix[i]);
        }

        free(newMatrix);
    }

    return det;
}

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
  
  /* Allocate the stack for ThreadUART4Sender.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  
  /* Create ThreadUART4Sender.  */
  if (tx_thread_create(&ThreadUART4Sender, "UART4 Sender Thread", ThreadUART4Sender_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       UART4_SENDER_THREAD_PRIO, UART4_SENDER_THREAD_PREEMPTION_THRESHOLD,
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
  if (tx_thread_create(&ThreadUART4Receiver, "UART4 Receiver Thread", ThreadUART4Receiver_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       UART4_RECEIVER_THREAD_PRIO, UART4_RECEIVER_THREAD_PREEMPTION_THRESHOLD,
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
  if (tx_thread_create(&ThreadUART5Receiver, "UART5 Receiver Thread", ThreadUART5Receiver_Entry, 0,
                       pointer, APP_STACK_SIZE,
                       UART5_RECEIVER_THREAD_PRIO, UART5_RECEIVER_THREAD_PREEMPTION_THRESHOLD,
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
  if (tx_thread_create(&ThreadUART5Sender, "UART5 Sender Thread", ThreadUART5Sender_Entry, 0,  
                       pointer, APP_STACK_SIZE, 
                       UART5_SENDER_THREAD_PRIO, UART5_SENDER_THREAD_PREEMPTION_THRESHOLD,
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

   /* Allocate queue for UART4 Sender. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART4_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create queue for UART4 Sender. */
  if (tx_queue_create(&QueueUART4Sender, "UART4 Sender Queue", TX_1_ULONG, pointer,
                      QUEUE_UART4_SENDER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

  /* Allocate queue for UART4 Receiver. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART4_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create queue for UART4 Receiver. */
  if (tx_queue_create(&QueueUART4Receiver, "UART4 Receiver Queue", TX_1_ULONG, pointer,
                      QUEUE_UART4_RECEIVER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

  /* Allocate queue for UART5 Sender. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART5_SENDER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create queue for UART5 Sender. */
  if (tx_queue_create(&QueueUART5Sender, "UART5 Sender Queue", TX_1_ULONG, pointer,
                      QUEUE_UART5_SENDER_SIZE) != TX_SUCCESS)
  {
    ret = TX_QUEUE_ERROR;
  }

  /* Allocate queue for UART5 Receiver. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                      QUEUE_UART5_RECEIVER_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  /* Create queue for UART5 Receiver. */
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

  // выделяем память
  UINT **A = (UINT**)malloc(N * sizeof(UINT*));
  UINT **B = (UINT**)malloc(N * sizeof(UINT*));
  UINT **C = (UINT**)malloc(N * sizeof(UINT*));
  for (i = 0; i < N; i++)
  {
	  A[i] = (UINT*)malloc(N * sizeof(UINT));
	  B[i] = (UINT*)malloc(N * sizeof(UINT));
	  C[i] = (UINT*)malloc(N * sizeof(UINT));
  }

  // заполняем случайными значениями
  srand(time(NULL));
  for (UINT i = 0; i < N; i++)
	  for (UINT j = 0; j < N; j++) {
		  A[i][j] = rand() % 256;
		  B[i][j] = rand() % 256;
      }


  while(1) {

	  int N = 5;

	  for (UINT ind = 0; ind < 2; ind++){
		  // C = A * B
		  MatrixMult(A, B, C, N);

		  // C = B * A
		  MatrixMult(B, A, C, N);

		  // B = A * A
		  MatrixMult(A, A, B, N);

		  // A = C * C
		  MatrixMult(C, C, A, N);
	  }

	  UINT det = matrixDet(C, N);

	  char* message = malloc(256);
	  memset(message, 0, 256);
	  itoa(det, message, 10);
	  message[strlen(message)] = '\n';

	  tx_queue_send(&QueueUART5Sender, &message, TX_WAIT_FOREVER);

   }

  // освобождаем память
  for (UINT i = 0; i < N; i++)
  {
    	free(A[i]);
    	free(B[i]);
    	free(C[i]);
  }

  free(A);
  free(B);
  free(C);
}

/**
  * @brief  Function implementing the ThreadUART4Receiver_Entry thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadUART4Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART4ReceiverData;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART4Receiver, &QueueUART4ReceiverData, TX_WAIT_FOREVER);
    // Place for some logic
    UINT value = atoi(QueueUART4ReceiverData);
    UINT res = value*value;

    // Delay = 100ms
//    App_Delay(10);
    tx_thread_sleep(10);
    char* message = malloc(256);
    memset(message, 0, 256);
    itoa(res, message, 10);
    message[strlen(message)] = '\n';
//    const uint8_t message_length = strlen(message);

    tx_queue_send(&QueueUART4Sender, &message, TX_WAIT_FOREVER);
  }
}

/**
  * @brief  Function implementing the ThreadUART4Sender_Entry thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadUART4Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART4SenderData;
  uint8_t message_size;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART4Sender, &QueueUART4SenderData, TX_WAIT_FOREVER);
    message_size = strlen(QueueUART4SenderData);
    HAL_UART_Transmit(&huart4, &message_size, 1, 1000);
    HAL_UART_Transmit(&huart4, QueueUART4SenderData, message_size, 10000);
  }
}

/**
  * @brief  Function implementing the ThreadUART5Receiver_Entry thread.
  * @param  thread_input: Not used 
  * @retval None
  */
void ThreadUART5Receiver_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART5ReceiverData;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART5Receiver, &QueueUART5ReceiverData, TX_WAIT_FOREVER);
    // Place for some logic
    UINT value = atoi(QueueUART5ReceiverData);
    UINT res = value*value;

    // Delay = 100ms
    // App_Delay(10);
    tx_thread_sleep(10);
    char* message = malloc(256);
    memset(message, 0, 256);
    itoa(res, message, 10);
    message[strlen(message)] = '\n';
//    const uint8_t message_length = strlen(message);

    tx_queue_send(&QueueUART5Sender, &message, TX_WAIT_FOREVER);
  }
}

/**
  * @brief  Function implementing the ThreadUART5Sender_Entry thread.
  * @param  thread_input: Not used
  * @retval None
  */
void ThreadUART5Sender_Entry(ULONG thread_input){
  UNUSED(thread_input);
  uint8_t* QueueUART5SenderData;
  uint8_t message_size;
  /* Infinite loop */
  while (1){
    tx_queue_receive(&QueueUART5Sender, &QueueUART5SenderData, TX_WAIT_FOREVER);
    message_size = strlen(QueueUART5SenderData);
    HAL_UART_Transmit(&huart5, &message_size, 1, 1000);
    HAL_UART_Transmit(&huart5, QueueUART5SenderData, message_size, 10000);
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
