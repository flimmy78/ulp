/*
*
*  miaofng@2014-5-10   initial version
*
*/

#include "ulp/sys.h"
#include "irc.h"
#include "vm.h"
#include "nvm.h"
#include "led.h"
#include <string.h>
#include "uart.h"
#include "shell/shell.h"
#include "can.h"
#include "err.h"
#include "led.h"
#include "bsp.h"
#include "mxc.h"
#include "rut.h"
#include "dps.h"

static const can_bus_t *irc_bus = &can1;
static int irc_update_called = 0;

void irc_init(void)
{
	board_init();

	rut_init();
	mxc_init();
	mxc_reset(MXC_SLOT_ALL);
	dps_init();
	irc_update_called = 0;
}

int irc_send(const can_msg_t *msg)
{
	int ecode = -IRT_E_CAN;
	time_t deadline = time_get(IRC_CAN_MS);
	irc_bus->flush();
	if(!irc_bus->send(msg)) { //wait until message are sent
		while(time_left(deadline) > 0) {
			if(irc_bus->poll(CAN_POLL_TBUF) == 0) {
				ecode = 0;
				break;
			}
		}
	}
	return ecode;
}

static int irc_can_dispatch(can_msg_t *msg)
{
	int id = CAN_TYPE(msg->id);
	switch(id) {
	case CAN_ID_MXC:
		mxc_can_handler(msg);
		break;
	default:
		break;
	}
	return 0;
}

void irc_can_handler(void)
{
	can_msg_t msg;
	while(!irc_bus->recv(&msg)) {
		irc_can_dispatch(&msg);
	}
}

void irc_update(void)
{
	sys_update();
	irc_update_called ++;
	if(irc_update_called == 1) {
		mxc_update();
		int n = mxc_scan(NULL, MXC_FAIL);
		if(n > 0) {
			irc_error(-IRT_E_SLOT_LOST);
		}
	}
	irc_update_called --;
}

static int irc_is_opc(void)
{
	int opc = vm_is_opc();
	return !opc;
}

static void irc_abort(void)
{
	vm_abort();
}

static int irc_mode(int mode)
{
	rut_mode(IRC_MODE_OFF);
	int ecode = mxc_mode(mode);
	rut_mode(mode);
	return ecode;
}

int main(void)
{
	sys_init();
	led_flash(LED_GREEN);
	printf("irc sw v1.2, build: %s %s\n\r", __DATE__, __TIME__);
	irc_init();
	vm_init();

	dps_config(DPS_LV, 0.0);
	dps_config(DPS_IS, 0.0);
	dps_config(DPS_HV, 0.0);

	while(1) {
		irc_update();
		vm_update();
	}
}

#include "shell/cmd.h"

int cmd_xxx_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"*IDN?		to read identification string\n"
		"*OPC?		operation is completed?\n"
		"*ERR?		error code & info\n"
		"*RST		instrument reset\n"
		"ABORT		abort current fail operation\n"
	};

	int ecode = 0;
	if(!strcmp(argv[0], "*IDN?")) {
		printf("<Linktron Technology,IRT16X3254,%s,%s\n\r", __DATE__, __TIME__);
		return 0;
	}
	else if(!strcmp(argv[0], "*OPC?")) {
		int opc = irc_is_opc();
		printf("<%+d\n\r", opc);
		return 0;
	}
	else if(!strcmp(argv[0], "*ERR?")) {
		_irc_error_print(irc_error_get(), NULL, 0);
		return 0;
	}
	else if(!strcmp(argv[0], "*RST")) {
		mxc_reset(MXC_SLOT_ALL);
		board_reset();
	}
	else if(!strcmp(argv[0], "ABORT")) {
		irc_abort();
		printf("<%+d\n\r", 0);
		return 0;
	}
	else if(!strcmp(argv[0], "*?")) {
		printf("%s", usage);
		return 0;
	}
	else {
		ecode = -IRT_E_CMD_FORMAT;
	}

	irc_error_print(ecode);
	return 0;
}

static int cmd_mode_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"MODE <mode>\n"
		"	<mode> = [HVR|L4R|W4R|L2R|L2T|PRB|RMX|VHV|VLV|IIS]\n"
	};

	int ecode = 0;
	int lv = (argc > 2) ? atoi(argv[2]) : 0;
	int is = (argc > 3) ? atoi(argv[3]) : 0;
	const char *name[] = {
		"HVR", "L4R", "W4R", "L2T", "RPB", "RMX",
		"RX2", "VHV", "VLV", "IIS", "OFF",
	};
	const int mode_list[] = {
		IRC_MODE_HVR, IRC_MODE_L4R,
		IRC_MODE_W4R, IRC_MODE_L2T,
		IRC_MODE_RPB, IRC_MODE_RMX,
		IRC_MODE_RX2, IRC_MODE_VHV,
		IRC_MODE_VLV, IRC_MODE_IIS,
		IRC_MODE_OFF,
	};

	if(!strcmp(argv[1], "HELP")) {
		printf("%s", usage);
		return 0;
	}

	ecode = -IRT_E_CMD_FORMAT;
	for(int i = 0; i < sizeof(mode_list) / sizeof(int); i ++) {
		if(!strcmp(argv[1], name[i])) {
			int mode = mode_list[i];
			ecode = -IRT_E_OP_REFUSED;
			if(irc_is_opc()) {
				ecode = irc_mode(mode);
				break;
			}
		}
	}
	irc_error_print(ecode);
	return 0;
}
const cmd_t cmd_mode = {"MODE", cmd_mode_func, "change irc work mode"};
DECLARE_SHELL_CMD(cmd_mode)

