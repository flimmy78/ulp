/*
*
* miaofng@2018-01-31 routine for ford online hub tester
* 1, fdsig includes: mux, audio and dmm <optional, in case of external dmm not used>
* 2, mux is like "hotkey" of cmd cmd gpio
*/
#include "ulp/sys.h"
#include "led.h"
#include <string.h>
#include <ctype.h>
#include "shell/shell.h"
#include "common/fifo.h"
#include "stm32f10x.h"
#include "gpio.h"
#include "pwm.h"
#include "tm770x.h"
#include "nvm.h"

#define CONFIG_DMM_CAL 0
#define CONFIG_MUX_API 1 //mode switch cmds
#define CONFIG_AUX_API 1 //audio modes switch cmds
#define CONFIG_DMM_API 1 //tm7707 emulated dmm cmds
#define CONFIG_DMM_DRV 1 //tm7707 hw driver

typedef struct {
	const char *name;
	unsigned ainx	: 1; //always TM_AIN1(=0)
	unsigned gain	: 8; //2^(0~7)
	unsigned dg409	: 2; //0~3
	unsigned dg408	: 5; //0~7|NSS
	unsigned diagx	: 3; //0~7
} dmm_ch_t;

typedef struct { //y = g*x + d
	float g;
	float d;
} dmm_cal_t;

#define BANK_18V 0 //0~+/-18V
#define BANK_06V 1 //0~+/-06V
#define BANK_1V5 3 //0~+/-1V5
#define BANK_ACS 2 //-0V5~5V5

static int sys_ecode = 0;
static const char *sys_emsg = "";

/*following parameters will be modified by dmm_switch_ch*/
const dmm_ch_t *dmm_ch = NULL; //current selected channel
dmm_cal_t *dmm_cal = NULL; //current calibration coefficient

#define dmm_ch_list_sz (sizeof(dmm_ch_list) / sizeof(dmm_ch_list[0]))
const dmm_ch_t dmm_ch_list[] = {
	//DG408#1
	{.dg408=0x10, .dg409=BANK_18V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VBAT+"},
	{.dg408=0x11, .dg409=BANK_18V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VBAT-"},
	{.dg408=0x12, .dg409=BANK_18V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VDIM"},
	{.dg408=0x13, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VUSB1"},
	{.dg408=0x14, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VUSB3C"},
	{.dg408=0x15, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VUSB2C"},
	{.dg408=0x16, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VUSB3"},
	{.dg408=0x17, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VUSB2"},

	//DG408#2
	{.dg408=0x20, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VDIAG"},
	{.dg408=0x21, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VIREF"},
	{.dg408=0x22, .dg409=BANK_1V5, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VCDP"},
	{.dg408=0x23, .dg409=BANK_06V, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VREF_+3V"},
	{.dg408=0x24, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VIBAT"},
	{.dg408=0x25, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VIELOAD1"},
	{.dg408=0x26, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VIELOAD2"},
	{.dg408=0x27, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="VIELOAD3"},
	//virtual channel
	{.dg408=0x24, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="IBAT"},
	{.dg408=0x25, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="IELOAD1"},
	{.dg408=0x26, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="IELOAD2"},
	{.dg408=0x27, .dg409=BANK_ACS, .diagx=0, .ainx=TM_AIN1, .gain=1, .name="IELOAD3"},

	//DIAG@CD4051
	{.dg408=0x20, .dg409=BANK_06V, .diagx=1, .ainx=TM_AIN1, .gain=1, .name="VAUXL"},
	{.dg408=0x20, .dg409=BANK_06V, .diagx=2, .ainx=TM_AIN1, .gain=1, .name="VAUXR"},
	{.dg408=0x20, .dg409=BANK_06V, .diagx=3, .ainx=TM_AIN1, .gain=1, .name="VAUXD"},
	{.dg408=0x20, .dg409=BANK_06V, .diagx=4, .ainx=TM_AIN1, .gain=1, .name="VAUXG"},
	{.dg408=0x20, .dg409=BANK_06V, .diagx=5, .ainx=TM_AIN1, .gain=1, .name="VOTG"},
};

static dmm_cal_t dmm_cals[] = {
	{.d = +0.0, .g = 108.2/8.200}, //BANK@BANK_18V, 8K2|100K
	{.d = +0.0, .g = 133.0/33.00}, //BANK@BANK_06V, 33K|100K
	{.d = +2.5, .g = 200.0/100.0}, //BANK@BANK_ACS, 100K|100K
	{.d = +0.0, .g = 100.0/100.0}, //BANK@BANK_1V5
};

//dmm drv
#if CONFIG_DMM_DRV == 1
static void dmm_hw_init(void);
static int dmm_hw_config(int ainx, int gain);
static int dmm_hw_poll(void);
static int dmm_hw_read(float *result);
#endif

//dmm api
static int dmm_switch_bank(int bank);
static int dmm_switch_ch(const char *ch_name);
#if CONFIG_DMM_API == 1
static void dmm_init(void);
static int dmm_read(float *v);
#define dmm_poll dmm_hw_poll
#endif

//cal api
#if CONFIG_DMM_CAL == 1
static int dmm_cal_bank(int bank);
static int dmm_cal_ieload(const char *ch);
#endif

void __sys_init(void)
{
	//GPIO_BIND(GPIO_OD1, PA08, HUBW_RST)
	//GPIO_BIND(GPIO_OD1, PA11, HUBL_RST)
	GPIO_BIND(GPIO_PP0, PA00, DIAG_VEH)
	GPIO_BIND(GPIO_PP0, PA01, DIAG_DET)
	GPIO_BIND(GPIO_PP0, PA02, DIAG_LOP)
	GPIO_BIND(GPIO_PP0, PA03, DIAG_SLR)

	GPIO_BIND(GPIO_PP1, PA08, VPM_EN)
	GPIO_BIND(GPIO_PP0, PA11, USB1_S1)
	GPIO_BIND(GPIO_PP0, PA12, USB1_S0)

	//PB
	GPIO_BIND(GPIO_OD1, PB05, HUBH_RST#)
	GPIO_BIND(GPIO_PP0, PB12, DG409_S0)
	GPIO_BIND(GPIO_PP0, PB13, DG409_S1)
	GPIO_BIND(GPIO_PP0, PB14, DG408_S4)
	GPIO_BIND(GPIO_PP0, PB15, DG408_S3)

	//PC
	GPIO_BIND(GPIO_PP0, PC06, USB3_S1)
	GPIO_BIND(GPIO_PP0, PC07, USB3_S0)
	GPIO_BIND(GPIO_PP0, PC08, USB2_S1)
	GPIO_BIND(GPIO_PP0, PC09, USB2_S0)
	GPIO_BIND(GPIO_PP1, PC12, USBx_EN#)
	GPIO_BIND(GPIO_PP0, PC13, DIAG_S2)
	GPIO_BIND(GPIO_PP0, PC14, DIAG_S1)
	GPIO_BIND(GPIO_PP0, PC15, DIAG_S0)

	//PD
	GPIO_BIND(GPIO_PP0, PD00, USB2C_CDP)
	GPIO_BIND(GPIO_PP1, PD01, USB2C_EN#)
	GPIO_BIND(GPIO_PP0, PD08, DG408_S2)
	GPIO_BIND(GPIO_PP0, PD09, DG408_S1)
	GPIO_BIND(GPIO_PP0, PD10, DG408_S0)
	GPIO_BIND(GPIO_PP0, PD11, VCDP_S2)
	GPIO_BIND(GPIO_PP0, PD12, VCDP_S1)
	GPIO_BIND(GPIO_PP0, PD13, VCDP_S0)
	GPIO_BIND(GPIO_PP0, PD14, USB3C_CDP)
	GPIO_BIND(GPIO_PP0, PD15, USB3C_EN#)

	//PE
	GPIO_BIND(GPIO_PP0, PE01, VUSB0_EN)
	GPIO_BIND(GPIO_OD1, PE02, HUBL_RST#)
	GPIO_BIND(GPIO_PP0, PE03, HOST_SEL)
	GPIO_BIND(GPIO_PP1, PE04, USB0_EN#)
	GPIO_BIND(GPIO_OD1, PE05, HUBW_RST#)

	GPIO_BIND(GPIO_PP0, PE08, EVBATN)
	GPIO_BIND(GPIO_PP0, PE09, EVBATP)
	GPIO_BIND(GPIO_PP0, PE10, EVUSB0)
	GPIO_BIND(GPIO_PP0, PE11, ELOAD1)
	GPIO_BIND(GPIO_PP0, PE12, ELOAD2)
	GPIO_BIND(GPIO_PP0, PE13, ELOAD3)
	GPIO_BIND(GPIO_PP0, PE14, ELOAD2C)
	GPIO_BIND(GPIO_PP0, PE15, ELOAD3C)

	//LED(on board)
	GPIO_BIND(GPIO_PP0, PC00, LED_R)
	GPIO_BIND(GPIO_PP0, PC01, LED_G)
}

//dmm hardware driver
#if CONFIG_DMM_DRV == 1
#define DMM_MCLK_HZ 4000000
#define DMM_ODAT_HZ 100
#define DMM_ISR_Y() do {EXTI->PR = EXTI_Line4; NVIC_EnableIRQ(EXTI4_IRQn);} while(0)
#define DMM_ISR_N() NVIC_DisableIRQ(EXTI4_IRQn)

static tm770x_t *dmm_hw;
static fifo_t dmm_hw_fifo;
static int dmm_hw_ainx;
static int dmm_hw_gain;
static float dmm_hw_vref = 3.0;

static int dmm_hw_config(int ainx, int gain)
{
	dmm_hw_ainx = ainx;
	dmm_hw_gain = gain;

	DMM_ISR_N();
	tm770x_config(dmm_hw, dmm_hw_gain, TM_BUF_N, TM_BIPOLAR);
	tm770x_cal_self(dmm_hw, dmm_hw_ainx);
	fifo_dump(&dmm_hw_fifo);
	DMM_ISR_Y();
	return 0;
}

/* return nitems available inside fifo
*/
static int dmm_hw_poll(void)
{
	DMM_ISR_N();
	int nitems = fifo_size(&dmm_hw_fifo);
	DMM_ISR_Y();
	return nitems;
}

/* this routine will not consider ina128 & dg408 & dg409
return nitems read, out para *result is tm7707 input voltage with unit: v
*/
static int dmm_hw_read(float *result)
{
	int digv, npop = 0;
	DMM_ISR_N();
	npop = fifo_pop(&dmm_hw_fifo, &digv);
	DMM_ISR_Y();

	if(npop > 0) {
		digv = (digv - 0x800000) / dmm_hw_gain;
		*result = dmm_hw_vref * digv / (1 << 23);
	}
	return npop;
}

void EXTI4_IRQHandler(void)
{
	int data = tm770x_read(dmm_hw, dmm_hw_ainx);
	EXTI->PR = EXTI_Line4;

	//success only when fifo has space, or npush = 0
	int npush = fifo_push(&dmm_hw_fifo, data);
}

static void dmm_hw_init(void)
{
	fifo_init(&dmm_hw_fifo, 16);
	dmm_hw = sys_malloc(sizeof(tm770x_t));
	sys_assert(dmm_hw != NULL);
	dmm_hw->bus = &spi1,
	dmm_hw->mclk_hz = DMM_MCLK_HZ;
	dmm_hw->pin_nrdy = gpio_bind(GPIO_IPU, "PC04", "TM_RDY#");
	dmm_hw->pin_nrst = gpio_bind(GPIO_PP0, "PC05", "TM_RST#");
	dmm_hw->pin_nss = gpio_bind(GPIO_PP1, "PA04", "TM_NSS#");

	//EXTI of nrdy
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	EXTI_InitTypeDef EXTI_InitStruct;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource4);
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_SetPriority(SysTick_IRQn, 0);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMM_ISR_N();

	//init tm7707 mclk=4MHz, range: 500Khz ~ 5MHz
	const pwm_cfg_t mclk_cfg = {.hz = DMM_MCLK_HZ, .fs = 1};
	pwm33.init(&mclk_cfg);
	pwm33.set(1);

	tm770x_init(dmm_hw, DMM_ODAT_HZ);
	//tm770x_config(&dmm_hw, TM_GAIN(4), TM_BUF_N, TM_UNIPOLAR);
	//tm770x_cal_self(&dmm_hw, TM_AIN1);
	//tm770x_cal_self(&dmm_hw, TM_AIN2);
	dmm_hw_config(TM_AIN1, TM_GAIN(1));
}
#endif

static const dmm_ch_t *dmm_ch_search(const char *ch_name)
{
	const dmm_ch_t *ch = NULL;
	for(int i = 0; i < dmm_ch_list_sz; i ++) {
		if(!strcmp(ch_name, dmm_ch_list[i].name)) {
			ch = &dmm_ch_list[i];
			break;
		}
	}
	return ch;
}

static int dmm_switch_bank(int bank)
{
	gpio_set("DG409_S1", bank & 0x02);
	gpio_set("DG409_S0", bank & 0x01);

#if CONFIG_DMM_API == 1
	dmm_cal = &dmm_cals[bank];
#endif
	return 0;
}

static int dmm_switch_ch(const char *ch_name)
{
	const dmm_ch_t *ch = dmm_ch_search(ch_name);
	if(ch == NULL) {
		sys_emsg = "invalid channel name";
		return -1;
	}

	//turn off all channels
	gpio_set("DG408_S4", 0);
	gpio_set("DG408_S3", 0);
	sys_mdelay(1);

	//cd4051
	gpio_set("DIAG_S2", ch->diagx & 0x04);
	gpio_set("DIAG_S1", ch->diagx & 0x02);
	gpio_set("DIAG_S0", ch->diagx & 0x01);

	//dg408
	gpio_set("DG408_S4", ch->dg408 & 0x20);
	gpio_set("DG408_S3", ch->dg408 & 0x10);
	gpio_set("DG408_S2", ch->dg408 & 0x04);
	gpio_set("DG408_S1", ch->dg408 & 0x02);
	gpio_set("DG408_S0", ch->dg408 & 0x01);

	//dg409
	dmm_switch_bank(ch->dg409);

#if CONFIG_DMM_DRV == 1
	sys_mdelay(1);
	dmm_hw_config(ch->ainx, ch->gain);
#endif

	dmm_ch = ch;
	return 0;
}

static int dmm_cal_bank(int bank)
{
	float x0 = 0, x1 = dmm_hw_vref;
	float y0 = 0, y1 = 0, y;
	int N = 20;

	//apply 0v(VBAT-) to input
	dmm_switch_ch("VBAT-");
	dmm_switch_bank(bank);
	dmm_hw_config(TM_AIN1, 1);

	for(int i = 0; i < N; i ++) {
		int n = dmm_read(&y);
		if(n == 0) {
		}

		printf("dmm: %+.6f => %+.6f V, uncal\n", x0, y);
		y0 += y;
	}

	//apply VREF_+3V0 to input
	dmm_switch_ch("VREF_+3V");
	dmm_switch_bank(bank);
	dmm_hw_config(TM_AIN1, 1);

	for(int i = 0; i < N; i ++) {
		int n = dmm_read(&y);
		if(n == 0) {
		}

		printf("dmm: %+.6f => %+.6f V, uncal\n", x1, y);
		y1 += y;
	}

	y0 /= N;
	y1 /= N;
	printf("dmm: x0=%+.6f, y0=%+.6f, uncal\n", x0, y0);
	printf("dmm: x1=%+.6f, y1=%+.6f, uncal\n", x1, y1);

	/*linearity correction
	y0 * g + d = x0(idea value)
	y1 * g + d = x1(idea value)

	(y1 - y0) * g = (x1 - x0) => g = (x1 - x0) / (y1 - y0)
	d = x0 - y0 * g
	*/
	float g = (x1 - x0) / (y1 - y0);
	float d = x0 - y0 * g;
	printf("dmm: g=%+.6f, d=%+.6f\n", g, d);

	/*write back
	y = (x * g0 + d0) * g + d = x * (g0 * g) + (d0 * g + d)
	g1 = g0 * g
	d1 = d0 * g + d
	*/
	float g1 = dmm_cal->g * g;
	float d1 = dmm_cal->d * g + d;

	printf("dmm: g=%+.6f, d=%+.6f, storing ...\n", g1, d1);
	dmm_cal->g = g1;
	dmm_cal->d = d1;
	return 0;
}

//dmm algo, include mux selection
static void dmm_init(void)
{
#if CONFIG_DMM_DRV == 1
	dmm_hw_init();
#endif

#if CONFIG_DMM_API == 1
	//self calibration
	dmm_cal_bank(BANK_18V);
	dmm_cal_bank(BANK_06V);
	dmm_cal_bank(BANK_ACS);
#endif

	dmm_switch_ch("VREF_+3V");
}

#if CONFIG_DMM_API == 1
/*wait until data available, or timeout occurs
*/
static int dmm_read(float *result)
{
	time_t deadline = time_get(1000 / DMM_ODAT_HZ * 2);
	while(dmm_poll() == 0) {
		sys_update();
		if(time_left(deadline) < 0) {
			sys_emsg = "dmm_read timeout error!";
			return 0;
		}
	}

	float v = 0;
	int n = dmm_hw_read(&v);
	if((n > 0) && (dmm_cal != NULL)) {
		v = v * dmm_cal->g + dmm_cal->d;
		*result = v;
	}
	return n;
}
#endif

#if CONFIG_DMM_CAL == 1
enum {
	VW_CAL_IBAT,
	VW_CAL_IELOAD1,
	VW_CAL_IELOAD2,
	VW_CAL_IELOAD3,
	VW_CAL_ELOAD1,
	VW_CAL_ELOAD2,
	VW_CAL_ELOAD3,
	NR_OF_CALS,
};

static void vw_cal_init(void)
{
	if(nvm_is_null()) {
		for(int i = 0; i < NR_OF_CALS; i ++) {
			vw_cals[i].g = 1.0;
			vw_cals[i].d = 0.0;
		}
	}
	hexdump("cal", vw_cals, sizeof(vw_cals));
}
#endif /*CONFIG_DMM_CAL == 1*/

void sig_init(void)
{
	dmm_init();
}

void sig_update(void)
{
}

void main()
{
	sys_init();
	shell_mute(NULL);
	printf("fdsig v1.x, build: %s %s\n\r", __DATE__, __TIME__);
	sig_init();
	while(1){
		sys_update();
		sig_update();
	}
}

int cmd_xxx_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"*IDN?		to read identification string\n"
		"*RST		instrument reset\n"
		"*UUID?		query the stm32 uuid\n"
	};

	if(!strcmp(argv[0], "*?")) {
		printf("%s", usage);
		return 0;
	}
	else if(!strcmp(argv[0], "*IDN?")) {
		printf("<+0, fdsig v1.x, build: %s %s\n\r", __DATE__, __TIME__);
		return 0;
	}
	else if(!strcmp(argv[0], "*RST")) {
		printf("<+0, No Error\n\r");
		mdelay(50);
		NVIC_SystemReset();
	}
	else if(!strcmp(argv[0], "*UUID?")) {
		unsigned uuid = *(unsigned *)(0X1FFFF7E8);
		printf("<%+d\n", uuid);
		return 0;
	}
	else {
		printf("<-1, Unknown Command\n\r");
		return 0;
	}
	return 0;
}

#if CONFIG_MUX_API == 1
typedef struct {
	const char *name;
	struct {
		unsigned char EVBATN: 1;
		unsigned char EVBATP: 1;
		unsigned char EVUSB0: 1;
		unsigned char ELOAD1: 1;
		unsigned char ELOAD2: 1;
		unsigned char ELOAD3: 1;
		unsigned char ELOAD2C: 1;
		unsigned char ELOAD3C: 1;
	} rly;
	struct {
		unsigned char upx_ven : 1; //high effective
		unsigned char upx_en : 1; //high effective
		unsigned char usbx_en : 1; //high effective
		unsigned char ds2c_en : 1; //high effective
		unsigned char ds3c_en : 1; //high effective
		unsigned char usb1 : 2; //mux sel @ds#1
		unsigned char usb2 : 2; //mux sel @ds#2
		unsigned char usb3 : 2; //mux sel @ds#3
		unsigned char ds2c : 1; //mux sel @ds#c2-
		unsigned char ds3c : 1; //mux sel @ds#c3-
	} mux;
} mode_t;

#define RLY_OFF  {.EVBATN=0, .EVBATP=0, .EVUSB0=0, .ELOAD1=0, .ELOAD2=0, .ELOAD3=0, .ELOAD2C=0, .ELOAD3C=0}
#define RLY_BYP  {.EVBATN=0, .EVBATP=1, .EVUSB0=0, .ELOAD1=0, .ELOAD2=0, .ELOAD3=0, .ELOAD2C=0, .ELOAD3C=0}
#define RLY_PMK  {.EVBATN=1, .EVBATP=1, .EVUSB0=0, .ELOAD1=1, .ELOAD2=1, .ELOAD3=1, .ELOAD2C=0, .ELOAD3C=0}
#define RLY_PMKC {.EVBATN=1, .EVBATP=1, .EVUSB0=0, .ELOAD1=0, .ELOAD2=0, .ELOAD3=0, .ELOAD2C=1, .ELOAD3C=1}

#define MUX_OFF  {.upx_ven=0, .upx_en=0, .usbx_en=0, .usb1=0, .usb2=0, .usb3=0, .ds2c_en=0, .ds2c=0, .ds3c_en=0, .ds3c=0}
#define MUX_IDL  {.upx_ven=1, .upx_en=1, .usbx_en=0, .usb1=0, .usb2=0, .usb3=0, .ds2c_en=0, .ds2c=0, .ds3c_en=0, .ds3c=0}
#define MUX_PMK  {.upx_ven=1, .upx_en=1, .usbx_en=1, .usb1=0, .usb2=0, .usb3=0, .ds2c_en=0, .ds2c=0, .ds3c_en=0, .ds3c=0}
#define MUX_CDP  {.upx_ven=1, .upx_en=1, .usbx_en=1, .usb1=3, .usb2=3, .usb3=3, .ds2c_en=1, .ds2c=1, .ds3c_en=1, .ds3c=1}
#define MUX_H21  {.upx_ven=1, .upx_en=1, .usbx_en=1, .usb1=2, .usb2=2, .usb3=0, .ds2c_en=0, .ds2c=0, .ds3c_en=0, .ds3c=0}
#define MUX_H12  {.upx_ven=1, .upx_en=1, .usbx_en=1, .usb1=1, .usb2=1, .usb3=0, .ds2c_en=0, .ds2c=0, .ds3c_en=0, .ds3c=0}
#define MUX_H13  {.upx_ven=1, .upx_en=1, .usbx_en=1, .usb1=1, .usb2=0, .usb3=1, .ds2c_en=0, .ds2c=0, .ds3c_en=0, .ds3c=0}
#define MUX_H12C {.upx_ven=1, .upx_en=1, .usbx_en=1, .usb1=1, .usb2=0, .usb3=0, .ds2c_en=1, .ds2c=0, .ds3c_en=0, .ds3c=0}
#define MUX_H13C {.upx_ven=1, .upx_en=1, .usbx_en=1, .usb1=1, .usb2=0, .usb3=0, .ds2c_en=0, .ds2c=0, .ds3c_en=1, .ds3c=0}

const mode_t mode_list[] = {
	{.name = "off" , .rly = RLY_OFF , .mux = MUX_OFF },
	{.name = "rev" , .rly = RLY_BYP , .mux = MUX_OFF },
	{.name = "iqz" , .rly = RLY_BYP , .mux = MUX_OFF },
	{.name = "idl" , .rly = RLY_BYP , .mux = MUX_IDL },
	{.name = "pmk" , .rly = RLY_PMK , .mux = MUX_PMK },
	{.name = "pmkc", .rly = RLY_PMKC, .mux = MUX_PMK },
	{.name = "cdp" , .rly = RLY_PMK , .mux = MUX_CDP },
	{.name = "h21" , .rly = RLY_PMK , .mux = MUX_H21 },
	{.name = "h12" , .rly = RLY_PMK , .mux = MUX_H12 },
	{.name = "h13" , .rly = RLY_PMK , .mux = MUX_H13 },
	{.name = "h12c", .rly = RLY_PMK , .mux = MUX_H12C},
	{.name = "h13c", .rly = RLY_PMK , .mux = MUX_H13C},
};

const mode_t *mode_search(const char *name)
{
	const mode_t *mode = NULL;
	int n = sizeof(mode_list) / sizeof(mode_t);
	for(int i = 0; i < n; i ++) {
		if(!strcmp(name, mode_list[i].name)) {
			mode = &mode_list[i];
		}
	}
	return mode;
}

static int cmd_mode_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"mode off		all disconnect\n"
		"mode rev		reverse voltage applied\n"
		"mode iqz		quiescent current mode, only vbat applied\n"
		"mode idl		idle current mode, only vbat and upstream usb connected\n"
		"mode pmk		all downstream = passmark mode\n"
		"mode pmkc		the same as pmk mode, except eload = type-c\n"
		"mode cdp		all downstream = cdp mode\n"
		"mode h12		host-to-host mode, 1->2\n"
		"mode h12c		host-to-host mode, 1->c2-\n"
		"mode h13		host-to-host mode, 1->3\n"
		"mode h13c		host-to-host mode, 1->c3-\n"
		"mode h21		host-to-host mode, 2->1\n"
	};

	if(argc >= 2) {
		const mode_t *mode = mode_search(argv[1]);
		if(mode != NULL) {
			//handle relay settings
			gpio_set("EVBATN", mode->rly.EVBATN);
			gpio_set("EVBATP", mode->rly.EVBATP);
			//gpio_set("EVUSB0", mode->rly.EVUSB0);
			gpio_set("ELOAD1", mode->rly.ELOAD1);
			gpio_set("ELOAD2", mode->rly.ELOAD2);
			gpio_set("ELOAD3", mode->rly.ELOAD3);
			gpio_set("ELOAD2C", mode->rly.ELOAD2C);
			gpio_set("ELOAD3C", mode->rly.ELOAD3C);

			//handle mux settings
			gpio_set("VUSB0_EN", mode->mux.upx_ven);
			gpio_set("USB0_EN#", !mode->mux.upx_en);
			gpio_set("USBx_EN#", !mode->mux.usbx_en);
			gpio_set("USB2C_EN#", !mode->mux.ds2c_en);
			gpio_set("USB3C_EN#", !mode->mux.ds3c_en);
			gpio_set("USB1_S1", mode->mux.usb1 & 0x02);
			gpio_set("USB1_S0", mode->mux.usb1 & 0x01);
			gpio_set("USB2_S1", mode->mux.usb2 & 0x02);
			gpio_set("USB2_S0", mode->mux.usb2 & 0x01);
			gpio_set("USB3_S1", mode->mux.usb3 & 0x02);
			gpio_set("USB3_S0", mode->mux.usb3 & 0x01);
			gpio_set("USB2C_CDP", mode->mux.ds2c & 0x01);
			gpio_set("USB3C_CDP", mode->mux.ds3c & 0x01);

			printf("<+0, OK!\n");
			return 0;
		}
	}

	printf("<-1, operation no effect!\n");
	printf("%s", usage);
	return 0;
}

cmd_t cmd_mode = {"mode", cmd_mode_func, "mode i/f cmds"};
DECLARE_SHELL_CMD(cmd_mode)

static int cmd_host_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"host win		host = win, pdi mode\n"
		"host linux		host = lnx, fct mode\n"
	};

	if(argc >= 2) {
		int win = !strcmp(argv[1], "win");
		int lnx = !strcmp(argv[1], "linux");
		if(win | lnx) {
			gpio_set("HOST_SEL", lnx);
			printf("<+0, OK!\n");
			return 0;
		}
	}

	printf("<-1, operation no effect!\n");
	printf("%s", usage);
	return 0;
}

cmd_t cmd_host = {"host", cmd_host_func, "host i/f cmds"};
DECLARE_SHELL_CMD(cmd_host)
#endif

#if CONFIG_AUX_API == 1
static int cmd_aux_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"aux test L|R		enter into playback mode, mic = L|R\n"
		"aux loop L|R		enter into loopback mode, mic = L|R\n"
		"aux diag [det=gnd]		enter into diagnose mode\n"
	};

	int test = 0;
	int loop = 0;
	int diag = 0;
	int gdet = 0; //aux_det = gnd
	int micr = 0; //mic_in = R
	if(argc >= 2) {
		test = !strcmp(argv[1], "test");
		loop = !strcmp(argv[1], "loop");
		diag = !strcmp(argv[1], "diag");
		if(argc >= 3) {
			if(diag) gdet = !strcmp(argv[2], "det=gnd");
			if(test|loop) micr = !strcmp(argv[2], "R");
		}
	}

	if(test|loop|diag) {
		gpio_set("DIAG_LOP", loop);
		gpio_set("DIAG_SLR", micr);
		gpio_set("DIAG_VEH", diag);
		gpio_set("DIAG_DET", gdet);
		printf("<+0, OK!\n");
		return 0;
	}

	printf("<-1, operation no effect!\n");
	printf("%s", usage);
	return 0;
}

cmd_t cmd_aux = {"aux", cmd_aux_func, "aux i/f cmds"};
DECLARE_SHELL_CMD(cmd_aux)
#endif

static int cmd_dmm_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"dmm ch [ch_name]	get/set dmm measure channel\n"
#if CONFIG_DMM_API == 1
		"dmm read		read result voltage/current\n"
		"dmm poll		poll nr of results in fifo\n"
#endif
	};

	int ecode = 0;
	if(argc >= 2) {
		if(!strcmp(argv[1], "ch")) {
			if(argc == 2) { //list current ch_list and current ch
				if(dmm_ch != NULL) printf("<+0, %s\n", dmm_ch->name);
				else printf("<+0, NONE\n");
				return 0;
			}
			ecode = dmm_switch_ch(argv[2]);
		}
#if CONFIG_DMM_API == 1
		if(!strcmp(argv[1], "poll")) {
			int nitems = dmm_poll();
			printf("<%+d, samples available in fifo\n", nitems);
			return 0;
		}
		if(!strcmp(argv[1], "read")) {
			float v = 0.0;
			int n = dmm_read(&v);
			printf("<%+d, %+.6f V\n", n, v);
			return 0;
		}
#endif
	}

	if(argc == 1) {
		printf("%s\n", usage);
		printf("dmm_ch_list[%d] = {\n", dmm_ch_list_sz);
		for(int i = 0; i < dmm_ch_list_sz; i ++) {
			printf("    %-10s: {.dg408=0x%02x, .dg409=%d, .diagx=%d, .gain=%03d},\n",
				dmm_ch_list[i].name,
				dmm_ch_list[i].dg408,
				dmm_ch_list[i].dg409,
				dmm_ch_list[i].diagx,
				dmm_ch_list[i].gain
			);
		}
		printf("}\n");
	}
	else {
		if(ecode) printf("<%+d, %s\n", ecode, sys_emsg);
		else printf("<+0, OK\n");
	}
	return 0;
}

cmd_t cmd_dmm = {"dmm", cmd_dmm_func, "dmm i/f cmds"};
DECLARE_SHELL_CMD(cmd_dmm)

#if CONFIG_DMM_CAL == 1
static int cmd_cal_func(int argc, char *argv[])
{
	const char *usage = {
		"usage:\n"
		"cal ch [ch_name]	get/set dmm measure channel\n"
	};

	if(argc == 1) {
		printf("%s\n", usage);
		printf("dmm_ch_list[%d] = {\n", dmm_ch_list_sz);
		for(int i = 0; i < dmm_ch_list_sz; i ++) {
			printf("    %-10s: {.dg408=0x%02x, .dg409=%d, .diagx=%d, .gain=%03d},\n",
				dmm_ch_list[i].name,
				dmm_ch_list[i].dg408,
				dmm_ch_list[i].dg409,
				dmm_ch_list[i].diagx,
				dmm_ch_list[i].gain
			);
		}
		printf("}\n");
	}
	else {
		if(ecode) printf("<%+d, %s\n", ecode, sys_emsg);
		else printf("<+0, OK\n");
	}
	return 0;
}

cmd_t cmd_cal = {"cal", cmd_cal_func, "cal i/f cmds"};
DECLARE_SHELL_CMD(cmd_cal)
#endif