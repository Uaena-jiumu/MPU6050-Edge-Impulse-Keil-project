/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "I2C_mpu.h"
#include "usart_transmit.h"
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
/* USER CODE BEGIN Variables */
int16_t test_buf[200][3];

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for AcquireTask */
osThreadId_t AcquireTaskHandle;
const osThreadAttr_t AcquireTask_attributes = {
  .name = "AcquireTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for InferTask */
osThreadId_t InferTaskHandle;
const osThreadAttr_t InferTask_attributes = {
  .name = "InferTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myQueue */
osMessageQueueId_t myQueueHandle;
const osMessageQueueAttr_t myQueue_attributes = {
  .name = "myQueue"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartAcquireTask(void *argument);
void StartInferTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue */
  myQueueHandle = osMessageQueueNew (10, 6, &myQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of AcquireTask */
  AcquireTaskHandle = osThreadNew(StartAcquireTask, NULL, &AcquireTask_attributes);

  /* creation of InferTask */
  InferTaskHandle = osThreadNew(StartInferTask, NULL, &InferTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartAcquireTask */
/**
* @brief Function implementing the AcquireTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAcquireTask */
void StartAcquireTask(void *argument)
{
  /* USER CODE BEGIN StartAcquireTask */
	  AccSample_t sample;
		uint16_t count = 0;
	// 从队列取数据
       for(;;)
    {
        // 从队列取数据（如果没有数据，这里会自动挂起等待，不占CPU）
        if (osMessageQueueGet(myQueueHandle, &sample, NULL, 100) == osOK)
        {
            // 把数据存进测试数组里
            test_buf[count][0] = sample.x;
            test_buf[count][1] = sample.y;
            test_buf[count][2] = sample.z;
            
            count++;
            
            // 攒满 200 个
            if (count >= 200)
            {
                count = 0;
                uart_send_str("---WINDOW_START---\r\n");
                
                // 把这一批数据通过串口发出去
                for (int i = 0; i < 200; i++) {
                    uart_send_int(test_buf[i][0]);
                    uart_send_str(",");
                    uart_send_int(test_buf[i][1]);
                    uart_send_str(",");
                    uart_send_int(test_buf[i][2]);
                    uart_send_str("\r\n");
                }
                uart_send_str("---WINDOW_END---\r\n");
            }
        }
    }
  /* USER CODE END StartAcquireTask */
}

/* USER CODE BEGIN Header_StartInferTask */
/**
* @brief Function implementing the InferTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartInferTask */
void StartInferTask(void *argument)
{
  /* USER CODE BEGIN StartInferTask */
  /* Infinite loop */
  for(;;)
  {
		// 暂时空跑，以后这里接 Edge Impulse 的推理代码
    osDelay(1000);
  }
  /* USER CODE END StartInferTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

