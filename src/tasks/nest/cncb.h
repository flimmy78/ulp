/*
 * 	miaofng@2010 initial version
 */

#include "config.h"

/*signal def*/
enum {
	/*Digital Output Pins*/
	SIG1,
	SIG2,
	SIG3,
	SIG4,
	SIG5,
	SIG6,

	/*Low Side Drive Pins*/
	BAT,
	IGN,
	LSD,
	LED_F,
	LED_R,
	LED_P,
};

/*signal status*/
enum {
	//SIGn high/low level
	SIG_LO = 0,
	SIG_HI = 1,

	//LSDn float/drive
	LSD_OFF = SIG_LO,
	LSD_ON = SIG_HI,

	//common ops
	TOGGLE,
};

/*cncb api*/
int cncb_init(void);
int cncb_detect(int debounce);
int cncb_signal(int sig, int level);
