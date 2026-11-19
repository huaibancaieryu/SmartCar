/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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
//int8_t Speed=0;

uint8_t RxData = 0x00;          // 接收缓冲区：存1个字节

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//_1电机A
void SetSpeedA(int8_t Speed)//有符号 8 位整数,可表示-128 ~ 127，所以SetSpeedA() 可以传正数、负数
{
	// Speed > 0 ：电机A正转
// Speed < 0 ：电机A反转
	if(Speed>=0)
	{
		// 正转方向
        // PA4 = 高电平，PA5 = 低电平
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		
		// 设置 TIM2 通道3 的PWM比较值
        // Speed越大，高电平时间越长，电机转得越快
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, Speed);
	}
	
	
	else
	{// 反转方向
        // PA4 = 低电平，PA5 = 高电平
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		
		// Speed是负数，但是PWM比较值不能填负数
        // 所以这里用 -Speed，把负数变成正数
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, -Speed);
	}
}

//_1电机B
void SetSpeedB(int8_t Speed)
{
    if(Speed >= 0) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);   // BIN1=H
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); // BIN2=L
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, Speed);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, -Speed);
    }
}


//_2电机C
void SetSpeedC(int8_t Speed)
{
	if(Speed>=0)
	{
		// 正转方向
        // PB4 = 高电平，PB5 = 低电平
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
		
		
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, Speed);
	}
	
	
	else
	{// 反转方向
        
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		
		
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, -Speed);
	}
}

//_2电机D
void SetSpeedD(int8_t Speed)
{
    if(Speed >= 0) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);   
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); 
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, Speed);
    } 
		else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, -Speed);
    }
}

void Advance(void) //小车前进
{
    SetSpeedA(30);
    SetSpeedB(30);
    SetSpeedC(30);
    SetSpeedD(30);
}

void Recoil(void) //小车后退
{
    SetSpeedA(-30);
    SetSpeedB(-30);
    SetSpeedC(-30);
    SetSpeedD(-30);
}

void Left_turn(void) //小车左转
{
    SetSpeedA(-30);
    SetSpeedB(-30);
    SetSpeedC(30);
    SetSpeedD(30);
}

void Right_turn(void) //小车右转
{
    SetSpeedA(30);
    SetSpeedB(30);
    SetSpeedC(-30);
    SetSpeedD(-30);
}

void Stop(void) //小车停止
{
    SetSpeedA(0);
    SetSpeedB(0);
    SetSpeedC(0);
    SetSpeedD(0);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	// 上电后，先让两个电机速度为0
// 刚开机先别乱动，保证安全
	SetSpeedA(0);
	SetSpeedB(0);
	
	SetSpeedC(0);
	SetSpeedD(0);

//这里的5不是在任何ARR下都固定等于5%
// 它本质上是PWM比较值CCR
// 因为我的TIM2周期ARR设置为99，那么CCR=5就是5%占空比

////	SetSpeedA(30);// 电机A %占空比.-100~100！
////	HAL_Delay(2000);
//	
//	SetSpeedA(-30);
//	HAL_Delay(2000);
////	
////	SetSpeedB(30);// 电机B %占空比。-100~100！
////	HAL_Delay(2000);
//	
//	SetSpeedB(-30);
//	HAL_Delay(2000);
//	
//	SetSpeedC(30);
//	
//	SetSpeedD(30);


// 启动 TIM2 通道3\4 的PWM输出
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);// PWMA
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);// PWMB
	
	// 启动 TIM3 通道3\4 的PWM输出
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);// PWMA
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);// PWMB


	//开启接收中断
	HAL_UART_Receive_IT(&huart1, &RxData, 1);
	
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//	/*ģ���ʼ��*/
//	OLED_Init();		//OLED��ʼ��
//	OLED_Clear();
//	OLED_ShowString(1, 1, "speed:");	//1��1����ʾ�ַ���speed��
//	/*OLED��ʾ*/




  while (1)
  {
//	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1) == GPIO_PIN_RESET)
//	{
//		//��������
//		HAL_Delay(20);
//		while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET);
//		HAL_Delay(20);
//		Speed += 1;
//		if (Speed > 2)
//		{
//			Speed = -2;
//		}
//	}
		
//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//  HAL_Delay(500);
		
	HAL_Delay(100);
//	OLED_ShowSignedNum(1, 7, Speed*50, 6);
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
//接收完成回调（每收到1个字节，HAL自动调用这里）
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
			// 调试：把收到的原始字节发回手机APP，看看到底是什么值
        HAL_UART_Transmit(&huart1,&RxData,1,100);
        // RxData 就是蓝牙发来的那个字节
        // 比如：0x01=前进, 0x02=后退, 0x03=左转...
        
        switch (RxData)
        {
            case 0x01: Advance();break;
            case 0x02: Recoil();break;
            case 0x03: Left_turn();break;
            case 0x04: Right_turn();break;
            case 0x05: Stop();break;
            default:   Stop();break;
        }

        // 重新开启下一次接收（必须加！否则只能收一次）
        HAL_UART_Receive_IT(&huart1,&RxData,1);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
