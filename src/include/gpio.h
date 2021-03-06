/*
 * miaofng@2017-06-10 initial version
 */

#ifndef __GPIO_H_
#define __GPIO_H_

#include "common/debounce.h"

typedef struct gpio_s {
	const void *drv;
	const char *name; //"LED_RED"
	const char *bind; //"PA0" or "PB10" or "mcp0:PA0"
	unsigned mode : 8;
	unsigned high : 1; //mirror register for gpio_inv()
	unsigned invt : 1; //inverted polar

#if CONFIG_GPIO_FILTER == 1
	struct debounce_s gfilt;
#endif
	int (*cb_on_set)(const struct gpio_s *gpio, int high);
} gpio_t;

/*
gpio is named as: PA0 port A, pin 0 note:
PA1 == PA01 == PA001 == PA0001
    == PA.1 == PA.01, not supported yet
*/

#define GPIO_BIND_INV(mode, gpio, name) gpio_bind_inv(mode, #gpio, #name);
#define GPIO_BIND(mode, gpio, name) gpio_bind(mode, #gpio, #name);
#define GPIO_FILT(name, ms) gpio_filt(#name, ms);
#define GPIO_SET(name, high) gpio_set(#name, high);
#define GPIO_GET(name) gpio_get(#name)

/*gpio mode list*/
enum {
	/*input*/
	GPIO_AIN, //adc input
	GPIO_DIN,
	GPIO_IPD, //pull down
	GPIO_IPU, //pull up

	/*output*/
	GPIO_PP0, //pp & init out 0
	GPIO_PP1, //pp & init out 1
	GPIO_OD0, //od & init out 0
	GPIO_OD1, //od & init out 1
};

#define IS_GPI(mode) (mode < GPIO_PP0)
#define IS_GPO(mode) (mode >= GPIO_PP0)

//bind := "PA0" or "PB10", return handle in case no error
int gpio_bind(int mode, const char *bind, const char *name);
int gpio_bind_inv(int mode, const char *bind, const char *name);

//filt the pulse widht <ms, 0 = disable
int gpio_filt(const char *name, int ms);

//add the callback before the gpio output level change, NULL to disable
int gpio_on_set(const char *name, int (*cb)(const gpio_t *gpio, int high));

int gpio_set(const char *name, int high);
int gpio_get(const char *name);
int gpio_inv(const char *name);
int gpio_wimg(int img, int msk);
int gpio_rimg(int msk);
int gpio_wbits(const void *img, const void *msk, int nbits);
int gpio_rbits(void *img, const void *msk, int nbits);

#define GPIO_NONE -1 //not exist

int gpio_handle(const char *name); //return hgpio or GPIO_NONE
int gpio_set_h(int gpio, int high);
int gpio_get_h(int gpio);
int gpio_inv_h(int gpio);

#if CONFIG_GPIO_MCP == 1
#include "mcp23s17.h"
void gpio_mcp_init(const mcp23s17_t *mcp_bus);
#endif

/*usage demo:

void main(void) {
	GPIO_BIND(GPIO_PP0, PE10, UPx_VCC_EN)
	GPIO_BIND(GPIO_DIN, PE08, TRIG_IN)

	GPIO_SET(UPx_VCC_EN, 0)
	GPIO_GET(TRIG_IN)
	...
}

*/

//priv
typedef struct gpio_drv_s {
	const char *name;
	const char *bind;
	int (*config)(const gpio_t *gpio);
	int (*set)(const gpio_t *gpio, int high);
	int (*get)(const gpio_t *gpio);
} gpio_drv_t;

extern const gpio_drv_t gpio_stm32;
#if CONFIG_GPIO_MCP == 1
extern const gpio_drv_t gpio_mcp;
#endif

//auto called by sys_init
void gpio_init(void);
#endif /* __GPIO_H_ */
