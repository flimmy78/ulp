/* lcd class driver header file
*  feature:
*	- gram is located inside lcd dev, so read_modify_write op is a common case
*	- support multidisplay configuration
*	- two or more identical lcd dev in a system is not allowed
*	- hardware/software scrolling is not supported yet
*	- dynamic display rotation is not supported yet
*	- only little endian is supported
*
* history:
 *	miaofng@2010 initial version
 */
#ifndef __LCD_H_
#define __LCD_H_

#include "config.h"
#include "common/glib.h"
#include "common/color.h"
#include "linux/list.h"

#define LCD_BGCOLOR_DEF WHITE
#define LCD_FGCOLOR_DEF BLACK

enum {
	LCD_ROT_000,
	LCD_ROT_090,
	LCD_ROT_180,
	LCD_ROT_270,
};

#ifdef CONFIG_LCD_ROT_090
	#define LCD_ROT_DEF LCD_ROT_090
#elif CONFIG_LCD_ROT_180
	#define LCD_ROT_DEF LCD_ROT_180
#elif CONFIG_LCD_ROT_270
	#define LCD_ROT_DEF LCD_ROT_270
#else
	#define LCD_ROT_DEF LCD_ROT_000
#endif

struct lcd_cfg_s {
	int rot;
	void *bus;
};

struct lcd_dev_s {
	//prop
	int xres; //x resolution
	int yres; //y resolution

	//init
	int (*init)(const struct lcd_cfg_s *cfg); //cfg is a lcd module specific para, maybe a lpt_bus_t or ...

	//api for char based lcd module
	int (*puts)(int x, int y, const char *str);

	//api for pixel based lcd module
	int (*setwindow)(int x0, int y0, int x1, int y1);
	int (*rgram)(void *dest, int n); //note: n indicates nr of pixel
	int (*wgram)(const void *src, int n);

	//for debug purpose
	int (*writereg)(int reg, int val);
	int (*readreg)(int reg);
};

struct lcd_s {
	const struct lcd_dev_s *dev;
	const char *name;
	int type;

	const char *font;
	int fgcolor;
	int bgcolor;
	int xres;
	int yres;
	int rot;
	struct list_head list;
};

#define LCD_TYPE_PIXEL 0
#define LCD_TYPE_CHAR 1
#define LCD_TYPE(lcd) (lcd -> type & LCD_TYPE_CHAR)

//lcd select
int lcd_add(const struct lcd_dev_s *dev, const char *name, int type);
struct lcd_s *lcd_get(const char *name); //pass NULL to get default lcd

//lcd para op
int lcd_get_res(struct lcd_s *lcd, int *x, int *y);
const char *lcd_get_font(struct lcd_s *lcd, int *w, int *h);
int lcd_set_fgcolor(struct lcd_s *lcd, int cr);
int lcd_set_bgcolor(struct lcd_s *lcd, int cr);

//lcd ops, all routines are use pixel based coordinate system
int lcd_init(struct lcd_s *lcd);
int lcd_bitblt(struct lcd_s *lcd, const void *bits, int x, int y, int w, int h);
int lcd_imageblt(struct lcd_s *lcd, const void *image, int x, int y, int w, int h);
int lcd_clear(struct lcd_s *lcd, int x, int y, int w, int h);
int lcd_clear_all(struct lcd_s *lcd);
int lcd_puts(struct lcd_s *lcd, int x, int y, const char *str);

#endif /*__LCD_H_*/
