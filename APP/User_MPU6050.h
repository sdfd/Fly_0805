#ifndef __USER_MPU6050_H__
#define __USER_MPU6050_H__

#include "SysConfig.h"


void MPU6050_Init(void);
void MPU6050_Read(void);
void MPU6050_Dataanl(T_int16_xyz *data_tempacc,T_int16_xyz *data_tempgyr);
void MPU6050_CalOff_Acc(void);
void MPU6050_CalOff_Gyr(void);
void I2C0IRQ(void);
#endif
