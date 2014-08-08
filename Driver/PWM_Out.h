#ifndef PWM_OUT_H
#define PWM_OUT_H

#include "SysConfig.h"
#include "PWM.h"
#define Moto_PwmMax 5000
#define Moto_PwmMin 2500
void Tim_Pwm_Out_Init(void);
void Moto_PwmRflash(int32_t MOTO1_PWM,int32_t MOTO2_PWM,int32_t MOTO3_PWM,int32_t MOTO4_PWM);

#endif
