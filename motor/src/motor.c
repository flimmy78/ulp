/*
 * 	miaofng@2009 initial version
 */

#include "config.h"
#include "board.h"
#include "motor.h"
#include <stdio.h>
#include <stdlib.h>

motor_t *motor;
pid_t *pid_speed;
pid_t *pid_torque;
pid_t *pid_flux;
vector_t Iab, I, Idq;
vector_t V, Vdq;

/*private*/
motor_status_t stm;
static time_t stop_timer;

void motor_Init(void)
{
	motor = malloc(sizeof(motor_t));
	
	/*default smo_motor para, change me!!!*/
	motor->rs = (short) NOR_RES(0);
	motor->ld = (short) NOR_IND(0);
	motor->lq = (short) NOR_IND(0);
	motor->pn = 8;
	motor->start_time = CONFIG_MOTOR_START_TIME;
	motor->start_speed = NOR_SPEED(RPM_TO_SPEED(CONFIG_MOTOR_START_RPM));
	motor->start_torque = NOR_AMP(CONFIG_MOTOR_START_CURRENT);
	
	stm = MOTOR_IDLE;
	pid_speed = pid_Init();
	pid_torque = pid_Init();
	pid_flux = pid_Init();
	pid_SetRef(pid_speed, 0);
	pid_SetRef(pid_torque, 0);
	pid_SetRef(pid_flux, 0);
	smo_Init();
}

void motor_Update(void)
{
	short speed, torque_ref, speed_pid;

	smo_Update();
	speed = smo_GetSpeed();
	speed_pid = pid_GetRef(pid_speed);

	switch (stm) {
		case MOTOR_IDLE:
			if(speed_pid > 0)
				stm = MOTOR_START_OP;
			break;

		case MOTOR_START_OP:
			smo_Start();
			vsm_Start();
			stm = MOTOR_START;

			//soft start???
			torque_ref = motor->start_torque;
			pid_SetRef(pid_flux, 0); /*Id = 0 control method*/
			pid_SetRef(pid_torque, torque_ref);
			break;
			
		case MOTOR_START:
			if(smo_IsLocked()) {
				stm = MOTOR_RUN;
			}
			if(speed_pid == 0) {
				stm = MOTOR_STOP_OP;
			}
			break;

		case MOTOR_RUN:
			if((speed_pid == 0) && (speed < motor->start_speed)) {
				stm = MOTOR_STOP_OP;
				break;
			}

			/*speed pid*/
			torque_ref = pid_Calcu(pid_speed, speed);
			pid_SetRef(pid_flux, 0); /*Id = 0 control method*/
			pid_SetRef(pid_torque, torque_ref);
			break;

		case MOTOR_STOP_OP:
			vsm_Stop(); /*note: It's not a brake!!!*/
			stop_timer = time_get(MOTOR_STOP_PERIOD);
			stm = MOTOR_STOP;
			break;
			
		case MOTOR_STOP:
			if(time_left(stop_timer) < 0)	
				stm = MOTOR_IDLE;
			break;

		default:
			printf("SYSTEM ERROR!!!\n");
	}
}

void motor_SetSpeed(short speed)
{
	pid_SetRef(pid_speed, speed); 
}

/*this routine will be called periodly by ADC isr*/
void motor_isr(void)
{
	short angle;
	
	/*1, smo get cur speed&angle*/
	angle = smo_GetAngle();
	/*2, get current*/
	vsm_GetCurrent(&Iab.a, &Iab.b);
	/*3, clarke*/
	clarke(&Iab, &I);
	/*4, park*/
	park(&I, &Idq, angle);
	/*5, update smo*/
	smo_isr(&V, &I);
	/*6, pid*/
	Vdq.d = pid_Calcu(pid_flux, Idq.d);
	Vdq.q = pid_Calcu(pid_torque, Idq.q);
	/*7, ipark circle lim*/
	/*8, ipark*/
	ipark(&Vdq, &V, angle);
	/*9, set voltage*/
	vsm_SetVoltage(V.alpha, V.beta);
}

