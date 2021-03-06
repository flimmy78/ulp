/* led.c
 * 	miaofng@2009 initial version
 *	- maxium 8 leds are supported
 */

#include "config.h"
#include "led.h"
#include "ulp_time.h"

static time_t led_timer;
static int flag_status; /*0->off/flash, 1->on*/
static int flag_flash; /*1->flash*/
static int flag_hwstatus; /*owned by led_update thread only*/
static int flag_combine;

#ifdef CONFIG_LED_ECODE
static char led_ecode;
static char led_estep; /*0/1: idle, other->counting ..*/
static time_t led_ecode_timer;
#endif

void led_Init(void)
{
	led_timer = 0;
	flag_status = 0;
	flag_flash = 0;
	flag_hwstatus = 0;
	flag_combine = 0;
#ifdef CONFIG_LED_ECODE
	led_ecode = 0;
	led_estep = 0;
	led_ecode_timer = 0;
#endif

#if CONFIG_LED_HWDRV == 1
	led_hwInit();
#endif
	led_flash(LED_SYS);
}

void led_Update(void)
{
#ifdef CONFIG_LED_ECODE
	if(led_ecode != 0) { //ecode=2: 1110 10 10
		if(time_left(led_ecode_timer) < 0) {
			led_ecode_timer = time_get(LED_ERROR_PERIOD);
			if(led_estep < LED_ERROR_IDLE) {
				flag_status &= ~(1 << LED_ERR);
			}
			else {
				flag_status ^= 1 << LED_ERR;
			}
			led_estep ++;
			led_estep = (led_estep > (LED_ERROR_IDLE + 1 + 2 * (led_ecode - 1))) ? 0 : led_estep;
			led_Update_Immediate(flag_status, 1 << LED_ERR);
		}
	}
#endif

	if(time_left(led_timer) < 0) {
		led_timer = time_get(LED_FLASH_PERIOD);
		int status = flag_hwstatus;
		status ^= flag_flash;
		status &= flag_flash;
		status |= flag_status;
		led_Update_Immediate(status, -1);
	}
}

void led_Update_Immediate(int status, int mask)
{
	int led;
	int update;
	//int status;

	/*calcu the new status of leds*/
/* 	status = flag_hwstatus;
	status ^= flag_flash;
	status &= flag_flash;
	status |= flag_status; */

	update = status ^ flag_hwstatus;
	update &= mask;
	flag_hwstatus &= ~ mask;
	flag_hwstatus |= status & mask;

	led = 0;
	while(update > 0) {
		if(update & 1) {
			led_hwSetStatus((led_t)led, (led_status_t)(status & 1));
		}

		led ++;
		update >>= 1;
		status >>= 1;
	}
}

/*combine() method is used to auto turn off
other led, such as: led_on(LED_R) will auto
turn off LED_G and clear flash(LED_G)*/
void led_combine(int mask)
{
	flag_combine = mask;
}

static void led_combine_process(led_t led)
{
	if(flag_combine & (1 << (int)led)) {
		for(int i = 0; i < 32; i++) {
			int mask = 1 << i;
			if(flag_combine & mask) {
				if(i != (int)led) {
					/*clear turn on & flash effect*/
					flag_status &= ~ mask;
					flag_flash &= ~ mask;
				}
			}
		}

		/*clear ecode effect*/
		#if CONFIG_LED_ECODE == 1
		if(flag_combine & (1 << LED_ERR)) { //LED_ERR is in combine
			led_error(0);
		}
		#endif
	}
}

void led_on(led_t led)
{
	int i = (int)led;
	led_combine_process(led);
#ifdef CONFIG_LED_ECODE
	if(led == LED_ERR) led_error(0);
#endif

#if CONFIG_LED_DUAL
	//clear red/green/yellow
	flag_status &= ~0x07;
	flag_flash &= ~0x07;
	if(led == LED_YELLOW) {
		flag_status |= 1 << LED_ERR;
		flag_status |= 1 << LED_SYS;
	}
	else
#endif
	{
		flag_status |= 1 << i;
		flag_flash &= ~(1 << i);
	}
	//led_Update_Immediate();
}

void led_off(led_t led)
{
	int i = (int)led;
	led_combine_process(led);
#ifdef CONFIG_LED_ECODE
	if(led == LED_ERR) led_error(0);
#endif

#if CONFIG_LED_DUAL
	//clear red/green/yellow
	flag_status &= ~0x07;
	flag_flash &= ~0x07;
	if(led == LED_YELLOW) {
		flag_status &= ~(1 << LED_ERR);
		flag_status &= ~(1 << LED_SYS);
	}
	else
#endif
	{
		flag_status &= ~(1 << i);
		flag_flash &= ~(1 << i);
	}

	//led_hwSetStatus(led, LED_OFF);
}

void led_inv(led_t led)
{
	int i = (int)led;
	led_combine_process(led);
#ifdef CONFIG_LED_ECODE
	if(led == LED_ERR) led_error(0);
#endif

#if CONFIG_LED_DUAL
	flag_status &= ~(0x07 & (~(1 << i)));
	flag_flash &= ~0x07;
	if(led == LED_YELLOW) {
		flag_status ^= 1 << LED_ERR;
		flag_status ^= 1 << LED_SYS;
	}
	else
#endif
	{
		flag_status ^= 1 << i;
		flag_flash &= ~(1 << i);
	}
	//led_Update_Immediate();
}

void led_flash(led_t led)
{
	int i = (int)led;
	led_combine_process(led);
#ifdef CONFIG_LED_ECODE
	if(led == LED_ERR) led_error(0);
#endif

#if CONFIG_LED_DUAL
	flag_status &= ~(0x07 & (~(1 << i)));
	flag_flash &= ~0x07;
	if(led == LED_YELLOW) {
		flag_flash |= 1 << LED_ERR;
		flag_flash |= 1 << LED_SYS;
	}
	else
#endif
	{
		flag_status &= ~(1 << i);
		flag_flash |= 1 << i;
	}
}

#ifdef CONFIG_LED_ECODE
void led_error(int ecode)
{
	ecode = (ecode < 0) ? - ecode : ecode;
	if(ecode != led_ecode) {
		if(ecode) {
			led_combine_process(LED_ERR);

			#if CONFIG_LED_DUAL
			flag_status &= ~0x07;
			flag_flash &= ~0x07;
			#else
			flag_status &= ~(1 << LED_ERR);
			flag_flash &= ~(1 << LED_ERR);
			#endif
		}

		led_ecode = (char) ecode;
		led_estep = 0;
	}
}
#endif

#if CONFIG_LED_CMD
#include "shell/cmd.h"
#include <string.h>
#include <stdlib.h>

static int cmd_led_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"led on r   	turn on/off/inv/flash led r/g/y\n"
		"led 3  			show ecode 3\n"
	};

	if(argc == 3) {
		led_t led = (led_t) atoi(argv[2]);
		led = (argv[2][0] == 'r') ? LED_ERR : led;
		led = (argv[2][0] == 'g') ? LED_SYS : led;
		led = (argv[2][0] == 'y') ? LED_YELLOW : led;

		switch(argv[1][2]) {
		case 'f':
			led_off(led);
			break;
		case 'v':
			led_inv(led);
			break;
		case 'a':
			led_flash(led);
			break;
		default:
			led_on(led);
			break;
		}
		return 0;
	}

	if(argc == 2) {
		led_error(atoi(argv[1]));
		return 0;
	}

	printf("%s", usage);
	return 0;
}
const cmd_t cmd_led = {"led", cmd_led_func, "led debug cmds"};
DECLARE_SHELL_CMD(cmd_led)
#endif
