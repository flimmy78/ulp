/*
 * 	peng.guo@2012 initial version
 */

#include "config.h"
#include "ulp/debug.h"
#include "ulp_time.h"
#include "stm32f10x_gpio.h"
#include "sys/task.h"
#include "mbi5025.h"
#include "dac.h"
#include "shell/cmd.h"
#include "stm32f10x.h"
#include "led.h"

static const mbi5025_t ict_mbi5025 = {
		.bus = &spi2,
		.idx = SPI_CS_DUMMY,
		.load_pin = SPI_CS_PD8,
		.oe_pin = SPI_CS_PD9,
};

void ict_Init(void)
{
	led_Init();
	led_flash(LED_GREEN);
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); /*72Mhz/8 = 9Mhz*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_71Cycles5); //9Mhz/(71.5 + 12.5) = 107.1Khz
	ADC_Cmd(ADC1, ENABLE);

	ADC_DeInit(ADC2);
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADC2, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 1, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 2, ADC_SampleTime_71Cycles5); //9Mhz/(71.5 + 12.5) = 107.1Khz
	ADC_Cmd(ADC2, ENABLE);

	ADC_AnalogWatchdogThresholdsConfig(ADC1, 0xCCC,0x000);
	ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_2);
	ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
	ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);

	ADC_AnalogWatchdogThresholdsConfig(ADC2, 0xCCC,0x000);
	ADC_AnalogWatchdogSingleChannelConfig(ADC2, ADC_Channel_7);
	ADC_AnalogWatchdogCmd(ADC2, ADC_AnalogWatchdog_SingleRegEnable);
	ADC_ITConfig(ADC2, ADC_IT_AWD, DISABLE);

	// DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;
	// DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;
	// DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
	// DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_StructInit(&DAC_InitStructure);
	DAC_Init(DAC_Channel_1,&DAC_InitStructure); 

	// DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_2047;
	DAC_StructInit(&DAC_InitStructure);
	DAC_Init(DAC_Channel_2,&DAC_InitStructure); 

	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_Cmd(DAC_Channel_2, ENABLE);
	DAC_SetChannel2Data(DAC_Align_12b_R, 0xCCC);

	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1));

	ADC_ResetCalibration(ADC2);
	while (ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while (ADC_GetCalibrationStatus(ADC2));

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);

	mbi5025_Init(&ict_mbi5025);
	mbi5025_EnableOE(&ict_mbi5025);
}

void ADC1_2_IRQHandler(void)
{
	int a;
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_AWD) == SET)
		a=1;
	ADC_ITConfig(ADC1, ADC_IT_AWD, DISABLE);
	ADC_ITConfig(ADC2, ADC_IT_AWD, DISABLE);
	ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
	ADC_ClearITPendingBit(ADC2, ADC_IT_AWD);
	//set relay
}

void main(void)
{

	task_Init();
	ict_Init();
	while(1) {
		task_Update();
	}
}

static int cmd_ict_func(int argc, char *argv[])
{
	int temp = 0;
	int i;

	const char * usage = { \
		" usage:\n" \
		" ict relay xx xx xx ..., write value to set relay \n" \
		" ict power_1 5123, set power_1 value 0~10000mv \n" \
		" ict power_2 5123, set power_2 value 0~10000mv \n" \
	};

	if (argc > 1) {
		if (argv[1][0] == 'r') {
			for (i = 0; i < (argc - 2); i++) {
				sscanf(argv[2+i], "%x", &temp);
				mbi5025_WriteByte(&ict_mbi5025, temp);
			}
			printf("%d Bytes Write Successful!\n", argc-2);
		}
		if (argv[1][0] == 'p') {
			if(atoi(argv[3]) < 0 || atoi(argv[3]) > 10)
				printf("input error ,please input a value between 0 and 10!\n");
			else {
				if(argv[1][6] == '1')
					dac_ch1.write(atoi(argv[3]) *(0xfff) /10000);
				if(argv[1][6] == '2')
					dac_ch2.write(atoi(argv[3]) *(0xfff) /10000);
			}
		}
	}

	if(argc < 2) {
		printf(usage);
		return 0;
	}

	return 0;
}
const cmd_t cmd_ict = {"ict", cmd_ict_func, "ict cmd"};
DECLARE_SHELL_CMD(cmd_ict)