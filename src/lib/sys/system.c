/* board.c
 * 	miaofng@2009 initial version
 */

#include "config.h"
#include "sys/system.h"
#include "led.h"
#include "console.h"
#include "spi.h"
#include "nvm.h"
#include "wdt.h"

void sys_Init(void)
{
	SystemInit();
	wdt_init(CONFIG_WDT_PERIOD);
	time_Init();
#if CONFIG_DRIVER_LED == 1
	led_Init();
#endif
#if (CONFIG_IAR_REDIRECT == 1) || (CONFIG_TASK_SHELL == 1)
	console_Init();
#endif
#if (CONFIG_FLASH_NVM == 1)
	nvm_init();
#endif
}

void sys_Update(void)
{
#if CONFIG_DRIVER_LED == 1
	led_Update();
#endif
	wdt_update();
}

__weak void SystemInit (void)
{
}