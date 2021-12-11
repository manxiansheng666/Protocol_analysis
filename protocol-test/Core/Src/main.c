/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ring_buffer.h"
#include "debug.h"
#include "protocol_analysis.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define CACHE_LEN 200
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
ring_buffer ring_buf;
uint8_t data_cache[CACHE_LEN];
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
	uint8_t num;
	uint8_t data[200];
	uint8_t tx_buf[200];
	message_head *msg_head;
	sk_buff skb;
	uint8_t *data_p;
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
HAL_UART_Receive_DMA(&huart1,rx_buffer,BUFFER_SIZE);
Ring_Buffer_Init(&ring_buf,data_cache,CACHE_LEN);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_Delay(5);
	  HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
	  
	  
	  while((num = if_find_frame_end(&ring_buf,data)))
	  {
		  msg_head = (message_head *)data;
		  if(!(msg_head->bigin == HEAD && 1))//判断帧头是否正确和校验值是否正确
			continue;
		  
		  switch(msg_head->command)
		  {
			  case 0x00:
				  myprintf("No.0 command!\r\n");
			  break;
			  
			  case 0x01:
				  door_control(msg_head->subCmd,msg_head->data,msg_head->dataLength);
			  break;
			  
			  case 0x02:
				sk_buff_init(&skb,tx_buf,200);
				skb_reserve(&skb,50);
			  smd_door door = {0x11,0x22};
			  if((data_p = skb_put(&skb,2)) != NULL)
			  {
				memcpy(data_p,&door,sizeof(smd_door));
			  }
			  
			  message_head head = {0xaabb,0x22,0x33,0x11223344,0x55,0x66,0x77,0x88};
			  if((data_p = skb_push(&skb,sizeof(message_head))) != NULL)
			  {
				memcpy(data_p,&head,sizeof(message_head));
			  }
			  
			  frame_end end = {0x99,0xbbaa};
			  if((data_p = skb_put(&skb,sizeof(frame_end))) != NULL)
			  {
				memcpy(data_p,&end,sizeof(frame_end));
			  }
			  HAL_UART_Transmit(&huart1,skb.data,skb.data_len,1000);
			  
			  break;
			  
			  default:
				break;
		  }
		  
		  
		  debug_printf("num is %d\r\n",num);
		  debug_printf("head is %x\r\n",msg_head->bigin);
		  debug_printf("version is %d\r\n",msg_head->version);
		  debug_printf("msgid is %x\r\n",msg_head->msgID);
		  debug_printf("time is %x\r\n",msg_head->timePoint);
		  debug_printf("msgtype is %d\r\n",msg_head->msgType);
		  debug_printf("command is %d\r\n",msg_head->command);
		  debug_printf("subcmd is %d\r\n",msg_head->subCmd);
		  debug_printf("datalength is %d\r\n",msg_head->dataLength);
		  
	  }
	  
//	  myprintf("last num is %d\r\n",Ring_Buffer_Get_Lenght(&ring_buf));
	  
//	  HAL_UART_Transmit(&huart1,"hello",5,1000);
	  
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart1.Instance == USART1)
	{
	    rx_cpt_nums = BUFFER_SIZE - hdma_usart1_rx.Instance->CNDTR;
//		HAL_UART_Transmit(&huart1,rx_buffer,rx_cpt_nums,1000);
		
		Ring_Buffer_Write_String(&ring_buf,rx_buffer,rx_cpt_nums);
		
		HAL_UART_Receive_DMA(&huart1,rx_buffer,BUFFER_SIZE);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
