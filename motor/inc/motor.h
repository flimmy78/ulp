/*
 * 	miaofng@2009 initial version
 */
#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "pid.h"
#include "smo.h"
#include "vector.h"
#include "math.h"

/*shared with command shell*/
extern vector_t Iab, I, Idq;
extern vector_t V, Vdq; 
extern pid_t *pid_speed;
extern pid_t *pid_torque;
extern pid_t *pid_flux;

#define MOTOR_UPDATE_PERIOD	(1000) /*unit: mS*/
#define MOTOR_STOP_PERIOD	(5000) /*unit: mS*/

#define RPM_TO_SPEED(rpm) ((int)rpm * motor->pn / (2 * 60))
#define SPEED_TO_RPM(speed) ((int)speed * (60 * 2) / motor->pn)

typedef enum {
	MOTOR_IDLE,
	MOTOR_START_OP,
	MOTOR_START,
	MOTOR_RUN,
	MOTOR_STOP_OP,
	MOTOR_STOP,
	MOTOR_ERROR
} motor_status_t;

void motor_Init(void);
void motor_SetSpeed(short speed);
void motor_Update(void);
void motor_isr(void); /*this routine will be called periodly by ADC isr*/
#endif /*__MOTOR_H_*/

