/*
 * 	miaofng@2009 initial version
 */

#include "config.h"
#include "cmd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "motor.h"

static int cmd_speed_func(int argc, char *argv[])
{
	int speed;
	
	if(argc < 2) {
		printf("uasge:\n");
		printf(" speed 100\n");
		return 0;
	}
	
	speed = (short)atoi(argv[1]);
	printf("setting motor speed to %dHz\n", speed);
	speed = NOR_SPEED(speed);
	motor_SetSpeed((short)speed);
	return 0;
}
cmd_t cmd_speed = {"speed", cmd_speed_func, "set motor speed in Hz"};

static int cmd_rpm_func(int argc, char *argv[])
{
	int rpm, speed;
	
	if(argc < 2) {
		printf("uasge:\n");
		printf(" rpm 100\n");
		return 0;
	}
	
	rpm = (short)atoi(argv[1]);
	printf("setting motor speed to %dRPM\n", rpm);
	speed = RPM_TO_SPEED(rpm);
	speed = NOR_SPEED(speed); 
	motor_SetSpeed((short)speed);
	return 0;
}
cmd_t cmd_rpm = {"rpm", cmd_rpm_func, "set motor speed in rpm"};

static int cmd_motor_func(int argc, char *argv[])
{
	int rs, ls, pn;
	if(argc < 4) {
		printf( \
			"uasge:\n" \
			" motor rs(Ohm) ls(mH) pn\n" \
			" motor 68 100 8\n" \
		);
		return 0;
	}
	
	rs = atoi(argv[1]);
	ls = atoi(argv[2]);
	pn = atoi(argv[3]);
	
	printf("rs = %dOhm\nls = %dmH\npn = %d\n", rs, ls, pn);
	motor->rs = NOR_RES(rs);
	motor->ld = NOR_IND(ls);
	motor->lq = NOR_IND(ls);
	motor->pn = pn;
	return 0;
}
cmd_t cmd_motor = {"motor", cmd_motor_func, "set motor paras"};

static void cmd_pid_usage(void)
{
	printf( \
			"uasge:\n" \
			" pid type kp ki\n" \
			" type = speed/flux/torque\n" \
			" pid speed 150 25\n" \
	);
}

static int cmd_pid_func(int argc, char *argv[])
{
	int kp,ki;
	
	if(argc < 4) {
		cmd_pid_usage();
		return 0;
	}
	
	kp = atoi(argv[2]);
	ki = atoi(argv[3]);
	
	if(!strcmp(argv[1], "speed")) {
		pid_Config(pid_speed, kp, ki, 0);
	}
	else if(!strcmp(argv[1], "flux")) {
		pid_Config(pid_flux, kp, ki, 0);
	}
	else if(!strcmp(argv[1], "torque")) {
		pid_Config(pid_torque, kp, ki, 0);
	}
	else {
		cmd_pid_usage();
	}
	return 0;
}
cmd_t cmd_pid = {"pid", cmd_pid_func, "set pid paras"};

