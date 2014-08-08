#ifndef _TIM_PWM_IN_H_
#define _TIM_PWM_IN_H_

#include "SysConfig.h"

//THROTTLE   PJ4
//YAW	       PJ5
//ROLL		   PG0
//PITCH		   PG1

void Rc_GetValue(T_RC_Data *temp);

void Tim_Pwm_In_Init(void);
//void Tim4_Pwm_In_Irq(void);
void Timer3A_IRQ(void);//PJ4
void Timer3B_IRQ(void);//PJ5
void Timer4A_IRQ(void);//PG0
void Timer4B_IRQ(void);//PG1

#endif
