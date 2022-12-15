/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.h
  * @author  MCD Application Team
  * @brief   ThreadX applicative header file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_THREADX_H__
#define __APP_THREADX_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "tx_api.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
UINT App_ThreadX_Init(VOID *memory_ptr);
void MX_ThreadX_Init(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define APP_STACK_SIZE                           512
#define APP_BYTE_POOL_SIZE                       (12 * 1024)

#define THREAD_ONE_PRIO                          1
#define THREAD_ONE_PREEMPTION_THRESHOLD          THREAD_ONE_PRIO

#define THREAD_TWO_PRIO                          5
#define THREAD_TWO_PREEMPTION_THRESHOLD          THREAD_TWO_PRIO

#define MAIN_THREAD_PRIO                         5
#define MAIN_THREAD_PREEMPTION_THRESHOLD         MAIN_THREAD_PRIO

#define THREAD_ONE_EVT                           0x01
#define THREAD_TWO_EVT                           0x02

#define QUEUE_UART3_SENDER_SIZE                  256
#define QUEUE_UART3_RECEIVER_SIZE                256
#define QUEUE_PC_SENDER_SIZE                     256
#define QUEUE_PC_RECEIVER_SIZE                   256



#define UART3_MAX_MESSAGE_SIZE                   256
/* USER CODE END PD */

/* USER CODE BEGIN 1 */
extern TX_QUEUE QueuePCReceiver;
/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_THREADX_H__ */
