/*
 * 	miaofng@2010 initial version
 *		this is service programming system interpreter !
 */

#include "config.h"
#include "util.h"
#include "common/kwp.h"
#include "common/ptp.h"
#include "common/print.h"
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"

#include "ff.h"

#define __DEBUG

static FIL util_file;
static ptp_t util_ptp;
static util_head_t util_head;
static util_inst_t *util_inst;
static int util_inst_nr;
static int util_seek;

//big endian to little endian
static int ntohl(int vb)
{
	int vl = 0;
	vl |= ((vb >> 0) & 0xff) << 24;
	vl |= ((vb >> 8) & 0xff) << 16;
	vl |= ((vb >> 16) & 0xff) << 8;
	vl |= ((vb >> 24) & 0xff) << 0;
	return vl;
}

static int ntohs(int vb)
{
	int vl = 0;
	vl |= ((vb >> 0) & 0xff) << 8;
	vl |= ((vb >> 8) & 0xff) << 0;
	return vl;
}

/*parse the utility bin file to a struct, ready for usage*/
int util_init(const char *util, const char *ptp)
{
	int br, btr;
	int ret = 0;
	FRESULT res;
	
	util_seek = 0;
	
	res = f_open(&util_file, util, FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK) {
		return - UTIL_E_OPEN;
	}

	util_ptp.fp = &util_file;
	util_ptp.read = (int (*)(void *, void *, int, int *)) f_read;
	util_ptp.seek = (int (*)(void *, int)) f_lseek;
	if(!ptp_init(&util_ptp)) {
		//read util head
		btr = sizeof(util_head_t);
		ptp_read(&util_ptp, &util_head, btr, &br);
		if(br != btr) {
			return - UTIL_E_RDHEAD;
		}
		
		//head endian convert
		util_head.cksum = ntohs(util_head.cksum);
		util_head.id = ntohs(util_head.id);
		util_head.sn = ntohl(util_head.sn);
		util_head.suffix = ntohs(util_head.suffix);
		util_head.htype = ntohs(util_head.htype);
		util_head.itype = ntohs(util_head.itype);
		util_head.offset = ntohs(util_head.offset);
		util_head.atype = ntohs(util_head.atype);
		util_head.addr = ntohl(util_head.addr);
		util_head.maxlen = ntohs(util_head.maxlen);
		
		//read all util instructions
		btr = util_head.offset - sizeof(util_head_t);
		util_inst_nr = btr / sizeof(util_inst_t);
		util_inst = MALLOC(btr);
		if(!util_inst) {
			return - UTIL_E_MEM;
		}
		ptp_read(&util_ptp, util_inst, btr, &br);
		if(br != btr) {
			return - UTIL_E_RDINST;
		}
		
		//success
#ifdef __DEBUG
		print("util_init() success\n");
		print("util_head.addr = 0x%06x\n", util_head.addr);
#endif
	}
		
	return ret;
}

int util_read(char *buf, int btr, int *br)
{
	int ret = 0;

	if(util_seek != util_head.offset) {
		util_seek = util_head.offset;
		if(ptp_seek(&util_ptp, util_seek)) {
			return - UTIL_E_SEEK;
		}
	}

	if(ptp_read(&util_ptp, buf, btr, br)) {
		return - UTIL_E_RDROUTINE;
	}
	
	return ret;
}

int util_interpret(void)
{
	int ret = 0;

#ifdef __DEBUG
	print("instruction list:\n");
	for(int i = 0; i < util_inst_nr; i ++) {
		util_inst_t *p = util_inst + i;
		print("%02d: SR_%02X(%02x, %02x, %02x, %02x), [%02x->%02d, %02x->%02d, %02x->%02d]\n", \
			p->step, p->opcode, \
			p->para[0], p->para[1], p->para[2], p->para[3], \
			p->jump[0].code, p->jump[0].step, \
			p->jump[1].code, p->jump[1].step, \
			p->jump[2].code, p->jump[2].step \
		);
	}
#endif

	return ret;
}

void util_close(void)
{
	ptp_close(&util_ptp);
	f_close(util_ptp.fp);
	FREE(util_inst);
}

int util_size(void)
{
	return (ptp_size(&util_ptp) - util_head.offset);
}

int util_addr(void)
{
	return (util_head.addr);
}

