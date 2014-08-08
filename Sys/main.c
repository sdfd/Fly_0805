#include "SysConfig.h"
#include "data_transfer.h"

void SYS_INIT()
{
//	ROM_FPUEnable();
  ROM_FPULazyStackingEnable();
	ROM_IntMasterEnable();
	time_init();
	LED_Init();
	LED_Loop();
	Tim_Pwm_Out_Init();
	UARTInit(115200);
	MPU6050_Init();//包含I2C
	NRF24L01_Init();//包含SPI
	Int_0_5msInit();	
	Tim_Pwm_In_Init();
}
u8 FLAG_ATT=0;
T_int16_xyz 		Acc,Gyr;	//两次综合后的传感器数据
T_int16_xyz			Acc_AVG;
T_float_angle 		Att_Angle;	//ATT函数计算出的姿态角
vs32				Alt;
T_RC_Data 			Rc_D;		//遥控通道数据
T_Control		RC_Control;		//遥控功能数据
int16_t 		Alt_CSB;		//超声波高度
T_GPS_Data		Gps_Data;
T_int16_xyz		Mag;
uint32_t time_start,time_dlt;
int main(void)
{
	static u8 att_cnt=0;
	static u8 rc_cnt=0;
	static T_int16_xyz mpu6050_dataacc1,mpu6050_dataacc2,mpu6050_datagyr1,mpu6050_datagyr2;
	static u8 senser_cnt=0,status_cnt=0,dt_rc_cnt=0,dt_moto_cnt=0;
	
	SYS_INIT();
	PID_PIT.P = 3;
	PID_PIT.D = 0.05;
	while (1)
	{			  
	time_start = time_nowUs();		
		if(FLAG_ATT)
		{
			FLAG_ATT = 0;
			att_cnt++;
			rc_cnt++;
			
			if(rc_cnt==20)
			{
				rc_cnt = 0;
//				#ifdef CONTROL_USE_RC
				Rc_GetValue(&Rc_D);
//				#endif
				RC_Analyse(&Rc_D,&RC_Control);
//				LED_B_OFF;
			}
			
			MPU6050_Read();
			
			if(att_cnt==1)
				MPU6050_Dataanl(&mpu6050_dataacc1,&mpu6050_datagyr1);
			else
			{
				att_cnt = 0;
				MPU6050_Dataanl(&mpu6050_dataacc2,&mpu6050_datagyr2);
				Acc.X = (mpu6050_dataacc1.X+mpu6050_dataacc2.X)/2;
				Acc.Y = (mpu6050_dataacc1.Y+mpu6050_dataacc2.Y)/2;
				Acc.Z = (mpu6050_dataacc1.Z+mpu6050_dataacc2.Z)/2;
				Gyr.X = (mpu6050_datagyr1.X+mpu6050_datagyr2.X)/2;
				Gyr.Y = (mpu6050_datagyr1.Y+mpu6050_datagyr2.Y)/2;
				Gyr.Z = (mpu6050_datagyr1.Z+mpu6050_datagyr2.Z)/2;
				Prepare_Data(&Acc,&Acc_AVG);
				IMUupdate(&Gyr,&Acc_AVG,&Att_Angle);
				Control(&Att_Angle,&Gyr,&Rc_D,RC_Control.ARMED);
				
				if(RC_Control.ARMED)
					LEDx_ONOFF(1);
				else
					LED_B_ON;
//					LED_G_OFF;
				senser_cnt++;
				status_cnt++;
				dt_rc_cnt++;
				dt_moto_cnt++;
				if(senser_cnt==5)
				{
					senser_cnt = 0;
	
//					Send_Senser = 1;
				}
				if(status_cnt==5)
				{
					status_cnt = 0;
		    	Send_Status = 1;
				}
				if(dt_rc_cnt==10)
				{
					dt_rc_cnt=0;
					Send_RCData = 1;
				}
				if(dt_moto_cnt==10)
				{
					dt_moto_cnt=0;
					Send_MotoPwm = 1;
				}
				time_dlt =  time_nowUs() - time_start;
			}
		}
		
	}
}

