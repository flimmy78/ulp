/*
 * miaofng@2013-2-13 initial version
 */

#include "common/vchip.h"
#include <stddef.h>
#include <string.h>

static unsigned vchip_base;
void vchip_init(void *mmr)
{
	vchip_base = (unsigned) mmr;
}

void vchip_reset(vchip_t *vchip)
{
	vchip->flag_soh = 0;
	vchip->flag_esc = 0;
}

static inline void vchip_soh(vchip_t *vchip)
{
	vchip->flag_soh = 1;
	vchip->flag_esc = 0;
	vchip->flag_cmd = 0;
	vchip->n = 0;
}

void vchip_update(vchip_t *vchip)
{
	char byte;
	vchip->txd = NULL;

	if(vchip->rxd != NULL) {
		byte = *(vchip->rxd);
		if((byte == VCHIP_SOH) && (vchip->flag_esc == 0)) {
			vchip_soh(vchip);
			return;
		}
	}

	if(! vchip->flag_soh) {
		return;
	}

	if(vchip->rxd != NULL) {
		//esc process
		if((byte == VCHIP_ESC) && (vchip->flag_esc == 0)) {
			vchip->flag_esc = 1;
			return;
		}

		vchip->flag_esc = 0;
		if(vchip->flag_cmd) {
			vchip->dat[vchip->n ++] = byte;
		}
		else {
			vchip->cmd.value = byte;
			vchip->flag_cmd = 1;
		}
	}

	switch(vchip->cmd.typ) {
	case VCHIP_AR:
		if(vchip->n == 2) {
			unsigned short ofs = 0;
			memcpy(&ofs, vchip->dat, 2);
			vchip->adr = (char *) (vchip_base + ofs);
			vchip_reset(vchip);
			vchip->txd = 0x00;
		}
		break;
	case VCHIP_AA:
		if(vchip->n == 4) {
			unsigned adr;
			memcpy(&adr, vchip->dat, 4);
			vchip->adr = (char *) adr;
			vchip_reset(vchip);
			vchip->txd = 0x00;
		}
		break;
	case VCHIP_W:
		if(vchip->n == vchip->cmd.len) { //do write here
			memcpy(vchip->adr, vchip->dat, vchip->cmd.len);
			vchip_reset(vchip);
			vchip->txd = 0x00;
		}
		break;
	case VCHIP_R:
		if(vchip->n == 0) {
			memcpy(vchip->dat, vchip->adr, vchip->cmd.len);
		}

		if(vchip->n < vchip->cmd.len) {
			vchip->txd = vchip->dat + vchip->n;
		}
		break;
	default:;
	}
}

static int __wcmd(const vchip_slave_t *slave, char cmd)
{
	slave->wb(VCHIP_SOH);
	slave->wb(VCHIP_SOH);
	slave->wb(cmd);
	return 0;
}

static int __wdat(const vchip_slave_t *slave, const void *buf, int n)
{
	const char *p = buf;
	for(int i = 0; i < n; i ++) {
		char byte = p[i];
		if((byte == VCHIP_SOH) || (byte == VCHIP_ESC)) {
			slave->wb(VCHIP_ESC);
		}
		slave->wb(byte);
	}
	return 0;
}

static int __rdat(const vchip_slave_t *slave, void *buf, int n)
{
	char *echo = buf;
	for(int i = 0; i < n; i ++) {
		char byte;
		slave->rb(&byte);
		echo[i] = byte;
	}
	return 0;
}

int vchip_outl(const vchip_slave_t *slave, unsigned addr, unsigned value)
{
	char ecode;
	__wcmd(slave, VCHIP_AR);
	__wdat(slave, &addr, 2);
	__rdat(slave, &ecode, 1);
	if(ecode == 0) {
		__wcmd(slave, VCHIP_WL);
		__wdat(slave, &value, 4);
		__rdat(slave, &ecode, 1);
	}
	return ecode;
}

int vchip_inl(const vchip_slave_t *slave, unsigned addr, unsigned *value)
{
	char ecode;
	__wcmd(slave, VCHIP_AR);
	__wdat(slave, &addr, 2);
	__rdat(slave, &ecode, 1);
	if(ecode == 0) {
		__wcmd(slave, VCHIP_RL);
		__rdat(slave, value, 4);
	}
	return 0;
}
