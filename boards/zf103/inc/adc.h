/* adc.h
 * 	miaofng@2009 initial version
 */
#ifndef __ADC_H_
#define __ADC_H_

#include "stm32f10x.h"

//#define ADC_MODE_NORMAL
#define ADC_MODE_INJECT

void adc_init(void);
/*zf32 board: PA1/ADC_IN1, JP1 R7 POT_WHEEL 0~3v3*/
/*unit: mv*/
#define ADC_COUNT_TO_VOLTAGE(CNT) (CNT*3300/4096)
int adc_getvolt(void); 
int adc_gettemp(void);
//void ad_update(void);
#endif /*__LED_H_*/