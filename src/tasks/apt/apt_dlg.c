/*
 * david@2011 initial version
 */
#include <string.h>
#include "config.h"
#include "stm32f10x.h"
#include "sys/task.h"
#include "osd/osd.h"
#include "key.h"
#include "c131_diag.h"
#include "c131_driver.h"
#include "c131.h"
#include "time.h"

//private functions
static int dlg_SelectGroup(const osd_command_t *cmd);
static int dlg_SelectSDMType(const osd_command_t *cmd);
static int dlg_SelectPWRType(const osd_command_t *cmd);
static int dlg_SelectAPTMode(const osd_command_t *cmd);
static int dlg_SelectAPTDiag(const osd_command_t *cmd);
static int dlg_SelectAPTStatus(const osd_command_t *cmd);
static int dlg_SelectAPTTest(const osd_command_t *cmd);

static int GetLinkInfo(void);
static int GetPWRType(void);
static int GetPwrINorEX(void);

static int index_load;
static char sdmtype_ram[16];	//the last char for EOC
static char type_select_ok;

static char aptmode_ram[10];
static char mode_select_ok = 0x7f;

static char aptdiag_ram[16];	//the last char for EOC
static char status_diag;

static char apttest_ram[16];	//the last char for EOC

static char status_link = 0;

static char sdm_pwr = 0;
static char led_pwr = 0;
static char pwr_type = 0;


const char str_sdmtype[] = "SDM Type Select";
const char str_sdmpwr[] = "SDM Power Select";
const char str_aptmode[] = "APT Mode Select";
const char str_aptdiag[] = "APT Diagnosis";
const char str_aptstatus[] = "APT Status";
const char str_apttest[] = "APT Func Testing";
const char str_type[] = "Type:";
const char str_mode[] = "Mode:";
const char str_info[] = "Info:";
const char str_link[] = "Link:";
const char str_led[] = "LED:";
const char str_sdm[] = "SDM:";
const char str_internal[] = "Internal";
const char str_external[] = "External";
const char str_sdmon[] = "SDM  On ";
const char str_sdmoff[] = "SDM  Off";

const osd_item_t items_sdmtype[] = {
	{2, 0, 15, 1, (int)str_sdmtype, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{0, 2, 16, 1, (int)sdmtype_ram, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	{19, 2, 1, 1, (int)&type_select_ok, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	NULL,
};

const osd_item_t items_pwrtype[] = {
	{2, 4, 16, 1, (int)str_sdmpwr, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{0, 6, 4, 1, (int)GetPWRType, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_ALWAYS, ITEM_RUNTIME_V},
	{11, 6, 8, 1, (int)GetPwrINorEX, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_ALWAYS, ITEM_RUNTIME_V},
	NULL,
};

const osd_item_t items_aptmode[] = {
	{2, 8, 15, 1, (int)str_aptmode, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{0, 10, 10, 1, (int)aptmode_ram, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	{19, 10, 1, 1, (int)&mode_select_ok, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	NULL,
};

const osd_item_t items_aptdiag[] = {
	{3, 12, 14, 1, (int)str_aptdiag, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{0, 14, 5, 1, (int)str_info, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{5, 14, 15, 1, (int)aptdiag_ram, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	NULL,
};

const osd_item_t items_aptstatus[] = {
	{4, 16, 10, 1, (int)str_aptstatus, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{0, 17, 5, 1, (int)str_type, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{5, 17, 15, 1, (int)sdmtype_ram, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	{0, 18, 5, 1, (int)str_mode, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{5, 18, 10, 1, (int)aptmode_ram, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	{0, 19, 5, 1, (int)str_link, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{5, 19, 15, 1, (int)GetLinkInfo, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_ALWAYS, ITEM_RUNTIME_V},
	NULL,
};

const osd_item_t items_apttest[] = {
	{2, 20, 16, 1, (int)str_apttest, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{0, 22, 5, 1, (int)str_info, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_NEVER, ITEM_RUNTIME_NONE},
	{5, 22, 15, 1, (int)apttest_ram, ITEM_DRAW_TXT, ITEM_ALIGN_LEFT, ITEM_UPDATE_AFTERCOMMAND, ITEM_RUNTIME_NONE},
	NULL,
};


const osd_command_t cmds_type[] = {
	{.event = KEY_UP, .func = dlg_SelectGroup},
	{.event = KEY_DOWN, .func = dlg_SelectGroup},
	{.event = KEY_RIGHT, .func = dlg_SelectSDMType},
	{.event = KEY_LEFT, .func = dlg_SelectSDMType},
	{.event = KEY_ENTER, .func = dlg_SelectSDMType},
	{.event = KEY_RESET, .func = NULL},
	NULL,
};

const osd_command_t cmds_pwr[] = {
	{.event = KEY_UP, .func = dlg_SelectGroup},
	{.event = KEY_DOWN, .func = dlg_SelectGroup},
	{.event = KEY_RIGHT, .func = dlg_SelectPWRType},
	{.event = KEY_LEFT, .func = dlg_SelectPWRType},
	{.event = KEY_ENTER, .func = dlg_SelectPWRType},
	{.event = KEY_RESET, .func = NULL},
	NULL,
};

const osd_command_t cmds_mode[] = {
	{.event = KEY_UP, .func = dlg_SelectGroup},
	{.event = KEY_DOWN, .func = dlg_SelectGroup},
	{.event = KEY_RIGHT, .func = dlg_SelectAPTMode},
	{.event = KEY_LEFT, .func = dlg_SelectAPTMode},
	{.event = KEY_ENTER, .func = dlg_SelectAPTMode},
	{.event = KEY_RESET, .func = NULL},
	NULL,
};

const osd_command_t cmds_diag[] = {
	{.event = KEY_UP, .func = dlg_SelectGroup},
	{.event = KEY_DOWN, .func = dlg_SelectGroup},
	{.event = KEY_ENTER, .func = dlg_SelectAPTDiag},
	{.event = KEY_RIGHT, .func = NULL},
	{.event = KEY_LEFT, .func = NULL},
	{.event = KEY_RESET, .func = NULL},
	NULL,
};

const osd_command_t cmds_status[] = {
	{.event = KEY_UP, .func = dlg_SelectGroup},
	{.event = KEY_DOWN, .func = dlg_SelectGroup},
	{.event = KEY_RIGHT, .func = dlg_SelectAPTStatus},
	{.event = KEY_LEFT, .func = dlg_SelectAPTStatus},
	{.event = KEY_RESET, .func = NULL},
	{.event = KEY_ENTER, .func = dlg_SelectAPTStatus},
	NULL,
};

const osd_command_t cmds_test[] = {
	{.event = KEY_UP, .func = dlg_SelectGroup},
	{.event = KEY_DOWN, .func = dlg_SelectGroup},
	{.event = KEY_ENTER, .func = dlg_SelectAPTTest},
	{.event = KEY_RIGHT, .func = NULL},
	{.event = KEY_LEFT, .func = NULL},
	{.event = KEY_RESET, .func = NULL},
	NULL,
};

const osd_group_t c131_grps[] = {
	{.items = items_sdmtype, .cmds = cmds_type, .order = 0, .option = GROUP_DRAW_FULLSCREEN},
	{.items = items_pwrtype, .cmds = cmds_pwr, .order = 1, .option = GROUP_DRAW_FULLSCREEN},
	{.items = items_aptmode, .cmds = cmds_mode, .order = 2, .option = GROUP_DRAW_FULLSCREEN},
	{.items = items_aptdiag, .cmds = cmds_diag, .order = 3, .option = GROUP_DRAW_FULLSCREEN},
	{.items = items_aptstatus, .cmds = cmds_status, .order = 4, .option = GROUP_DRAW_FULLSCREEN},
	{.items = items_apttest, .cmds = cmds_test, .order = 5, .option = GROUP_DRAW_FULLSCREEN},
	NULL,
};

osd_dialog_t c131_dlg = {
	.grps = c131_grps,
	.cmds = NULL,
	.func = NULL,
	.option = 0,
};

static int dlg_SelectGroup(const osd_command_t *cmd)
{
	int result = -1;

	if(cmd->event == KEY_UP)
		result = osd_SelectPrevGroup();
	else
		result = osd_SelectNextGroup();

	return result;
}

static int dlg_SelectSDMType(const osd_command_t *cmd)
{
	if (cmd->event == KEY_LEFT) {
		do {
			index_load --;
			if (index_load < 0)
				index_load = NUM_OF_LOAD - 1;
		} while(c131_GetSDMType(index_load, sdmtype_ram));
	} else if(cmd->event == KEY_RIGHT) {
		do {
			index_load ++;
			if (index_load >= NUM_OF_LOAD) 
				index_load = 0;
		} while(c131_GetSDMType(index_load, sdmtype_ram));
	} else if(cmd->event == KEY_ENTER) {
		c131_ConfirmLoad(index_load);
	}
	if (c131_GetCurrentLoadIndex() == index_load)
		type_select_ok = 0xff;
	else
		type_select_ok = 0x7f;

	return 0;
}

static int dlg_SelectPWRType(const osd_command_t *cmd)
{
	if (cmd->event == KEY_LEFT) {
		pwr_type = 0;
	} else if(cmd->event == KEY_RIGHT) {
		pwr_type = 1;
	} else if(cmd->event == KEY_ENTER) {
		if (pwr_type == 0) {
			sdm_pwr = ~sdm_pwr;
		} else {
			led_pwr = ~led_pwr;
		}
	}

	return 0;
}
static int GetPWRType(void)
{
	if (pwr_type == 0) {
		return (int)str_sdm;
	} else {
		return (int)str_led;
	}
}

static int GetPwrINorEX(void)
{	if (pwr_type == 0) {
		if (sdm_pwr == 0) {
			return (int)str_internal;
		} else {
			return (int)str_external;
		}
	} else {
		if (led_pwr == 0) {
			return (int)str_internal;
		} else {
			return (int)str_external;
		}
	}
}

static int dlg_SelectAPTMode(const osd_command_t *cmd)
{
	if (cmd->event == KEY_LEFT) {
		memset(aptmode_ram, 0x00, 10);
		strcpy(aptmode_ram, "Normal   ");
		if (c131_GetMode() == C131_MODE_NORMAL)
			mode_select_ok = 0xff;
		else
			mode_select_ok = 0x7f;
	} else if(cmd->event == KEY_RIGHT) {
		memset(aptmode_ram, 0x00, 10);
		strcpy(aptmode_ram, "Simulator");
		if (c131_GetMode() == C131_MODE_SIMULATOR)
			mode_select_ok = 0xff;
		else
			mode_select_ok = 0x7f;
	} else if(cmd->event == KEY_ENTER) {
		if (strcmp(aptmode_ram, "Simulator") == 0) {
			c131_SetMode(C131_MODE_SIMULATOR);
		} else {
			c131_SetMode(C131_MODE_NORMAL);
		}
		mode_select_ok = 0xff;
	}

	return 0;
}

static int dlg_SelectAPTDiag(const osd_command_t *cmd)
{
	if (cmd->event == KEY_ENTER) {
		if (status_diag == DIAGNOSIS_NOTYET) {
			//power on for IGN_LED
			Enable_SDMPWR();
			c131_DiagSW();
			c131_DiagLED();
			//c131_DiagLOOP();
			Disable_SDMPWR();

			strcpy(aptdiag_ram, "Diagnose  Over");
			status_diag = DIAGNOSIS_OVER;
		}
	}

	return 0;
}


static int dlg_SelectAPTStatus(const osd_command_t *cmd)
{
	return 0;
}

static int GetLinkInfo(void)
{
	if (c131_GetLinkInfo() == SDM_UNEXIST)
		return (int)str_sdmoff;
	else
		return (int)str_sdmon;
}

static int dlg_SelectAPTTest(const osd_command_t *cmd)
{
	if (cmd->event == KEY_ENTER) {
		if ((c131_GetTestStatus() == C131_TEST_NOTYET) && status_link) {
			strcpy(apttest_ram, "SDM in Testing");
			c131_SetTeststatus(C131_TEST_ONGOING);
			//for initializing the status
			c131_ConfirmLoad(c131_GetCurrentLoadIndex());
			c131_relay_Update();
			Enable_SDMPWR();
		}
	}

	return 0;
}