#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include "SysConfig.h"
#include "IMU.h"
#include "Control.h"
#include "User_MPU6050.h"

extern u8 Data_Check,Send_Status,Send_Senser,Send_RCData,Send_GpsData,Send_Offset,Send_MotoPwm;

void Data_GetOneByte(u8 data);
void Data_Receive_Anl(u8 *data_buf,u8 num);
void Data_Exchange(void);

void Data_Send_Status(void);	
void Data_Send_Senser(void);	
void Data_Send_RCData(void);	
void Data_Send_GpsData(void);
void Data_Send_OFFSET(void);	
void Data_Send_PID1(void);
void Data_Send_PID2(void);
void Data_Send_PID3(void);
void Data_Send_PID4(void);
void Data_Send_PID5(void);
void Data_Send_PID6(void);
void Data_Send_MotoPWM(void);

void Data_Send_Check(u16 check);

void NRF_Send_Test(void);

#endif
