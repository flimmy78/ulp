/*
 * 	miaofng@2012 initial version
 */

#include "ulp/sys.h"
#include "gui/gui.h"

static gwidget *main_window;
static gwidget *fixed;
static gwidget *button_lines;
static gwidget *button_ground;
static gwidget *button_mode_id;
static gwidget *button_mode_dg;
static gwidget *button_start;
static gwidget *button_keycode;
static gwidget *button_typecode;
static gwidget *button_ecode0;
static gwidget *button_ecode1;
static gwidget *button_ecode2;

static unsigned cmap_ecode[] = GUI_COLORMAP_NEW(RED, WHITE);
static unsigned cmap_tcode[] = GUI_COLORMAP_NEW(GREEN, WHITE);
static unsigned cmap_kcode[] = GUI_COLORMAP_NEW(GRAY, WHITE);
static unsigned cmap_timer[] = GUI_COLORMAP_NEW(PURPLE, WHITE);

static struct {
	unsigned char lines;
	const unsigned char *ground;
	unsigned char mode;
	unsigned char seconds;
	unsigned char start;
} oid_config;

static int main_identify_on_event(gwidget *widget, gevent *event)
{
	static char str[5];
	static char i = 0;
	if(event->type == GUI_TOUCH_BEGIN) {
		printf("x = %d, y = %d\n", event->ts.x, event->ts.y);
		if(gui_widget_touched(widget, event)) {
			if(widget == button_lines) {
				oid_config.lines = (oid_config.lines < 4) ? (oid_config.lines + 1) : 1;
				sprintf(str, "%d", oid_config.lines);
				gui_widget_set_text(widget, str);
			}
			else if(widget == button_ground) {
				switch(oid_config.ground[0]) {
				case '?':
					oid_config.ground = "N";
					break;
				case 'N':
					oid_config.ground = "Y";
					break;
				default:
					oid_config.ground = "?";
					break;
				}
				gui_widget_set_text(widget, oid_config.ground);
			}
			else if(widget == button_mode_dg) {
				oid_config.mode = 'd';
				gui_widget_set_text(button_mode_id, " ");
				gui_widget_set_text(button_mode_dg, "<");
			}
			else if(widget == button_mode_id) {
				oid_config.mode = 'i';
				gui_widget_set_text(button_mode_id, "<");
				gui_widget_set_text(button_mode_dg, " ");
			}
			else if(widget == button_start) {
				oid_config.start = (oid_config.start == 1) ? 0 : 1;
			}
			else {
				i = (i < 19) ? (i + 1) : 0;
				sprintf(str, "%d", i);
				gui_widget_set_text(widget, str);
			}
		}
	}
	return -1;
}

void main_window_init(void)
{
	extern const unsigned char image_main_window[];
	main_window = gui_window_new(GUI_WINDOW_TOPLEVEL);
	gui_widget_set_image(main_window, image_main_window);
	fixed = gui_fixed_new();
	gui_widget_add(main_window, fixed);

	button_lines = gui_button_new_with_label("4");
	button_ground = gui_button_new_with_label("?");
	button_mode_id = gui_button_new_with_label("<");
	button_mode_dg = gui_button_new_with_label(" ");
	button_start = gui_button_new();
	button_keycode = gui_button_new_with_label("--");
	button_typecode = gui_button_new_with_label("--");
	button_ecode0 = gui_button_new_with_label("--");
	button_ecode1 = gui_button_new_with_label("--");
	button_ecode2 = gui_button_new_with_label("--");

	gui_widget_set_colormap(button_keycode, cmap_kcode);
	gui_widget_set_colormap(button_typecode, cmap_tcode);
	gui_widget_set_colormap(button_ecode0, cmap_ecode);
	gui_widget_set_colormap(button_ecode1, cmap_ecode);
	gui_widget_set_colormap(button_ecode2, cmap_ecode);

	gui_widget_set_size_request(button_lines, 60, 40-4);
	gui_widget_set_size_request(button_ground, 60, 40-4);
	gui_widget_set_size_request(button_mode_id, 60, 40-4);
	gui_widget_set_size_request(button_mode_dg, 60, 40-4);
	gui_widget_set_size_request(button_start, 120, 40-4);
	gui_widget_set_size_request(button_keycode, 116, 40-4);
	gui_widget_set_size_request(button_typecode, 116, 40-4);
	gui_widget_set_size_request(button_ecode0, 116, 40-4);
	gui_widget_set_size_request(button_ecode1, 116, 40-4);
	gui_widget_set_size_request(button_ecode2, 116, 40-4);
	gui_fixed_put(fixed, button_lines, 260, 40*1 + 2);
	gui_fixed_put(fixed, button_ground, 260, 40*2 + 2);
	gui_fixed_put(fixed, button_mode_dg, 260, 40*3 + 2);
	gui_fixed_put(fixed, button_mode_id, 260, 40*4 + 2);
	gui_fixed_put(fixed, button_start, 200, 40*5 + 2);
	gui_fixed_put(fixed, button_keycode, 80, 40*1 + 2);
	gui_fixed_put(fixed, button_typecode, 80, 40*2 + 2);
	gui_fixed_put(fixed, button_ecode0, 80, 40*3 + 2);
	gui_fixed_put(fixed, button_ecode1, 80, 40*4 + 2);
	gui_fixed_put(fixed, button_ecode2, 80, 40*5 + 2);
	gui_event_connect(button_lines, main_identify_on_event);
	gui_event_connect(button_ground, main_identify_on_event);
	gui_event_connect(button_mode_id, main_identify_on_event);
	gui_event_connect(button_mode_dg, main_identify_on_event);
	gui_event_connect(button_start, main_identify_on_event);
	gui_event_connect(button_keycode, main_identify_on_event);
	gui_event_connect(button_typecode, main_identify_on_event);
	gui_event_connect(button_ecode0, main_identify_on_event);
	gui_event_connect(button_ecode1, main_identify_on_event);
	gui_event_connect(button_ecode2, main_identify_on_event);
	gui_window_show(main_window);
}

void main(void)
{
	sys_init();
	gui_init(NULL);
	main_window_init();
	while(1) {
		sys_update();
		gui_update();
	}
}
