/*
 *  miaofng@2010 initial version
 */

#ifndef __CAN_H_
#define __CAN_H_

#include "config.h"

typedef struct {
	unsigned baud;
	char silent;
} can_cfg_t;

typedef struct {
	int id;
	int mask;
	int flag;
} can_filter_t;

#define CAN_CFG_DEF { \
	.baud = 500000, \
	.silent = 0, \
}

#define CAN_FLAG_EXT (1 << 0) /*ENABLE 29bit CAN ID*/
#define CAN_FLAG_RTR (1 << 1) /*Remote Transmission Request*/

typedef struct {
	int id; /*11/29bit*/
	char dlc; /*0~8*/
	char data[8];
	char flag;
} can_msg_t;

void can_msg_print(const can_msg_t *msg, char *str);

typedef struct {
	int (*init)(const can_cfg_t *cfg);
	int (*send)(const can_msg_t *msg); //non block, check -> ?busy ret -> send
	int (*recv)(can_msg_t *msg); //non block, check -> ?empty ret-> recv
	int (*filt)(can_filter_t const *filter, int n);
	void (*flush)(void); //flush tx & rx

#ifdef CONFIG_CAN_ENHANCED
	/*when enhanced mode is on, traditional recv is redirect to RFIFO0 only*/
	int (*erecv)(int rfifo, can_msg_t *msg);
	int (*efilt)(int rfifo, can_filter_t const *filter, int n);
#endif
} can_bus_t;

extern const can_bus_t can0;
extern const can_bus_t can1;

#endif /* __CAN_H_ */
