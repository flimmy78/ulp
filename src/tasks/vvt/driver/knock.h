/*
 * 	miaofng@2010 initial version
 *		provide vvt signal: ks1~4
 */
#ifndef __KNOCK_H_
#define __KNOCK_H_

typedef enum {
	KS1 = 0,
	KS2,
	KS3,
	KS4,
	NR_OF_KS
} knock_ch_t;

extern unsigned short vvt_adc3[5];

void knock_Init(void);
void knock_Update(void);
void knock_SetFreq(short hz);
void knock_SetVolt(knock_ch_t ch, short mv);
int knock_GetPattern(void);
void knock_Enable(int en);

#endif /*__KNOCK_H_*/

