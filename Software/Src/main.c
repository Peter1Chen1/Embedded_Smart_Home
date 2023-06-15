/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2023 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//总线
#define MYAHB1         0x40020000
#define MYAHB2         0x48000000
//外围控制器
#define MYRCC          (MYAHB1 + 0x1000)
#define MYGPIOB        (MYAHB2 + 0x400)
//外围控制器中的寄存器
#define MYRCC_AHBENR   (*(int *)(MYRCC + 0x14))
	
#if 0

#define MYGPIOB_MODER     (*(int *)(MYGPIOB + 0x00))
#define MYGPIOB_OTYPER    (*(int *)(MYGPIOB + 0x04))
#define MYGPIOB_OSPEEDR   (*(int *)(MYGPIOB + 0x08))
#define MYGPIOB_PUPDR     (*(int *)(MYGPIOB + 0x0c))
#define MYGPIOB_ODR       (*(int *)(MYGPIOB + 0x14))

#else

typedef struct MYGPIO
{
	int mygpio_moder;
	int mygpio_otyper;
	int mygpio_ospeedr;
	int mygpio_pupdr;
	int mygpio_idr;
	int mygpio_odr;
	
} MYGPIO_t;

#endif

unsigned char buf[16] = {0};
int flag_uart = 0;

//该函数会被printf调用
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return EOF;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  //UNUSED(huart);

	HAL_UART_Receive_IT(huart, buf, 1);
	flag_uart = 1;
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_UART_RxCpltCallback can be implemented in the user file.
   */
}

void mygpiob_init(int pin)
{
	MYGPIO_t *mygpio = (MYGPIO_t *)MYGPIOB;
	mygpio->mygpio_moder &= ~(0x03 << 2 * pin);
	mygpio->mygpio_moder |= (0x01 << 2 * pin);
	
	mygpio->mygpio_otyper &= ~(0x01 << 1 * pin);
	
	mygpio->mygpio_ospeedr &= ~(0x03 << 2 * pin);
	mygpio->mygpio_ospeedr |= ~(0x01 << 2 * pin);
	
	mygpio->mygpio_pupdr &= ~(0x03 << 2 * pin);
}

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
	HAL_UART_Receive_IT(&huart1, buf, 1);

  /* USER CODE BEGIN 2 */
	
	//printf("hello world\r\n");
	
//使能GPIOB端口的时钟
	MYRCC_AHBENR |= (0x01 << 1 * 18);
	
	//设置GPIOB端口参数
#if 0
	MYGPIOB_MODER &= ~(0x03 << 2 * 2);
	MYGPIOB_MODER |= (0x01 << 2 * 2);
	
	MYGPIOB_OTYPER &= ~(0x01 << 1 * 2);
	
	MYGPIOB_OSPEEDR &= ~(0x03 << 2 * 2);
	MYGPIOB_OSPEEDR |= ~(0x01 << 2 * 2);
	
	MYGPIOB_PUPDR &= ~(0x03 << 2 * 2);
	
	MYGPIOB_ODR &= ~(0x01 << 1 * 2);//点亮
	MYGPIOB_ODR |= (0x01 << 1 * 2);//关闭
#else
	
	MYGPIO_t *mygpio = (MYGPIO_t *)MYGPIOB;
	mygpio->mygpio_moder &= ~(0x03 << 2 * 2);
	mygpio->mygpio_moder |= (0x01 << 2 * 2);
	
	mygpio->mygpio_otyper &= ~(0x01 << 1 * 2);
	
	mygpio->mygpio_ospeedr &= ~(0x03 << 2 * 2);
	mygpio->mygpio_ospeedr |= ~(0x01 << 2 * 2);
	
	mygpio->mygpio_pupdr &= ~(0x03 << 2 * 2);
	
	mygpio->mygpio_odr &= ~(0x01 << 1 * 2);//点亮
	//mygpio->mygpio_odr |= (0x01 << 1 * 2);//关闭
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);//点亮
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);//关闭
	
	mygpiob_init(0);
	mygpiob_init(2);
	mygpiob_init(7);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);//关闭
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);//关闭
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);//关闭

#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		if (flag_uart == 1)
		{
			switch (buf[0])
			{
				case '1':
					//开1灯
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);//点亮
					printf("1");
				  break;
				
				case '2':
					//关1灯
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);//关闭
					printf("2");
					break;
				
				case '3':
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);//点亮
					printf("3");
					//开2灯
					break;
				
				case '4':
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);//关闭
					printf("4");
					//关2灯
					break;
				
				case '5':
					//开风扇
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);//打开
					printf("5");
					break;
				
				case '6':
					//关风扇
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);//关闭
					printf("6");
					break;
				
				default:
					printf("0");
				  break;
				
			}
			
			//printf("recv: %c\n", buf[0]);
			flag_uart = 0;
		}
			
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
