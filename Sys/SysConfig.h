#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include <stdbool.h>
#include <stdint.h>
#include "rom.h"
#include "sysctl.h"
#include "pin_map.h"
#include "gpio.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_mpu6050.h"
#include "i2cm_drv.h"
#include "mpu6050.h"
#include "hw_ints.h"
#include "hw_i2c.h"
#include "i2c.h"
#include "uart.h"
//#include "uartstdio.h"
#include "fpu.h"
#include "rom_map.h"
#include "time_systick.h"
#include "LED.h"
#include "User_UART.h"
#include "PWM_Out.h"
//#include "User_MPU6050.h"
#include "nRF24L01.h"
#include "Int_0_5ms.h"

typedef struct{
				float rol;
				float pit;
				float yaw;}T_float_angle;
typedef struct{
				float X;
				float Y;
				float Z;}T_float_xyz;
typedef struct{
				int16_t X;
				int16_t Y;
				int16_t Z;}T_int16_xyz;
typedef struct int16_rcget{
				int16_t ROLL;
				int16_t PITCH;
				int16_t THROTTLE;
				int16_t YAW;
				int16_t AUX1;
				int16_t AUX2;
				int16_t AUX3;
				int16_t AUX4;
				int16_t AUX5;
				int16_t AUX6;}T_RC_Data;
typedef struct{
				double	UTC;		//时间
				vs32	Lat_32;
				double	Lat;		//纬度
				//char	NSi;
				vs32	Lng_32;
				double	Lng;		//经度
				//char	EWi;
				float	Alt;		//高度
				float	SOG;		//速度
				float	Hac;		//高度精度
				float	Vac;		//水平精度
				float	COG;		//
				u8		FixSta;	//GPS状态
				float	VD;			//?????
				u8		SVN;		//卫星数量
				}T_GPS_Data;
typedef struct{
				u8		ARMED;	//=0,锁定
				}T_Control;
#include "Tim_pwm_in.h"
#include "rc.h"
extern char thread_6050_stack[1024];
extern struct rt_thread thd_6050;
void rt_thread_entry_6050(void* parameter);

extern char thread_ATT_stack[2048];
extern struct rt_thread thd_ATT;
void rt_thread_entry_ATT(void* parameter);

extern char thread_POS_stack[1024];
extern struct rt_thread thd_POS;
void rt_thread_entry_POS(void* parameter);

extern char thread_RC_stack[1024];
extern struct rt_thread thd_RC;
void rt_thread_entry_RC(void* parameter);

extern char thread_RT_Timer_stack[1024];
extern struct rt_thread thd_RT_Timer;
void rt_thread_entry_RT_Timer(void* parameter);

extern struct rt_event Evt_Sys;
extern u8 FLAG_ATT;
extern T_int16_xyz 		Acc,Gyr;	//两次综合后的传感器数据
extern T_int16_xyz		Mag;
extern vs32				Alt;
extern int16_t 		Alt_CSB;		//超声波高度
extern T_float_angle 	Att_Angle, Att_Angle_External;	//ATT函数计算出的姿态角
extern float			Mag_Heading;
extern T_RC_Data 		Rc_D;
extern T_Control		RC_Control;
extern T_GPS_Data		Gps_Data;
extern u8				LED_STA;
extern u32				Debug_cnt;
extern u32			Sys_Reg;			//不能等于ffffffff,用于表示该数据存储page是否是使用过的,另外用于表示是否停留在boot,默认=0,=1时表示停留在boot
extern u8 			IIC1_OK, IIC2_OK, NRF_OK, RC_OK, GPS_OK, USB_Enable;

#endif
