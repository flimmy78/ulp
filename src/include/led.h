/* led.h
 * 	miaofng@2009 initial version
 */
#ifndef __LED_H_
#define __LED_H_

#define LED_FLASH_PERIOD	1000 /*unit ms*/

typedef enum {
	LED_GREEN = 0,
	LED_RED,
	LED_YELLOW,
	NR_OF_LED
} led_t;

typedef enum {
	LED_OFF = 0,
	LED_ON
} led_status_t;

void led_Init(void);
void led_Update(void);
void led_on(led_t led);
void led_off(led_t led);
void led_inv(led_t led);
void led_flash(led_t led);

/*hw led driver routines*/
void led_hwInit(void);
void led_hwSetStatus(led_t led, led_status_t status);
#endif /*__LED_H_*/
