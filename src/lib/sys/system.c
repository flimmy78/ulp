/* board.c
 * 	miaofng@2009 initial version
 */

#include "config.h"
#include "stm32f10x.h"
#include "sys/system.h"
#include "led.h"
#include "console.h"
#include "stm32_mac.h"
#include "spi.h"

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void cpu_Init(void);

void sys_Init(void)
{
	cpu_Init();
	time_Init();
	led_Init();
	console_Init();
#if CONFIG_SYSTEM_SPI1 == 1
	spi_Init(1, SPI_MODE_POL_1| SPI_MODE_PHA_0| SPI_MODE_BW_16 | SPI_MODE_MSB);
#endif
#if CONFIG_SYSTEM_SPI2 == 1
	spi_Init(2, SPI_MODE_POL_0| SPI_MODE_PHA_0| SPI_MODE_BW_16 | SPI_MODE_MSB);
#endif

#if CONFIG_DRIVER_MAC_STM32 == 1
	stm32mac_Init();
#endif

	/*indicates board init finish*/
	led_on(LED_RED);
	mdelay(100);
	led_off(LED_RED);
	
	led_on(LED_GREEN);
	mdelay(100);
	led_off(LED_GREEN);
}

void sys_Update(void)
{
	led_Update();
	time_Update();
}

void cpu_Init(void)
{
	ErrorStatus HSEStartUpStatus;
	
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
 	
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

#ifndef STM32F10X_CL
		/* PLLCLK = 12MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);
#else
	/* Configure PLLs *********************************************************/
	/* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz,HSE = 25M */
	RCC_PREDIV2Config(RCC_PREDIV2_Div5);
	RCC_PLL2Config(RCC_PLL2Mul_8);
	
	/* Enable PLL2 */
	RCC_PLL2Cmd(ENABLE);

	/* Wait till PLL2 is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
	{}

	/* PLL configuration: PLLCLK = (PLL2 / 5) * 9 = 72 MHz */ 
	RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div5);
	RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#endif

		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
}

/*******************************************************************************
* Function Name	: NVIC_Configuration
* Description		: Configures Vector Table base location.
* Input					: None
* Output				 : None
* Return				 : None
*******************************************************************************/
void NVIC_Configuration(void)
{
#ifdef	VECT_TAB_RAM	
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else	/* VECT_TAB_FLASH	*/
	/* Set the Vector Table base location at 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);	 
#endif
}

#ifdef	DEBUG
/*******************************************************************************
* Function Name	: assert_failed
* Description		: Reports the name of the source file and the source line number
*									where the assert_param error has occurred.
* Input					: - file: pointer to the source file name
*									- line: assert_param error line source number
* Output				 : None
* Return				 : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
	/* User can add his own implementation to report the file name and line number,
		 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
