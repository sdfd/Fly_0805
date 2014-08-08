#include "imu.h"
#include "math.h"


#define RtA 		57.324841f				
#define AtR    		0.0174533f				
#define Acc_G 		0.0011963f				
#define Gyro_G 		0.0610351f				
#define Gyro_Gr		0.0010653f			
#define FILTER_NUM 	20

float 	AngleOffset_Rol=0,AngleOffset_Pit=0;

void Prepare_Data(T_int16_xyz *acc_in,T_int16_xyz *acc_out)
{
	static uint8_t 	filter_cnt=0;
	static int16_t	ACC_X_BUF[FILTER_NUM],ACC_Y_BUF[FILTER_NUM],ACC_Z_BUF[FILTER_NUM];
	int32_t temp1=0,temp2=0,temp3=0;
	uint8_t i;

	ACC_X_BUF[filter_cnt] = acc_in->X;
	ACC_Y_BUF[filter_cnt] = acc_in->Y;
	ACC_Z_BUF[filter_cnt] = acc_in->Z;
	for(i=0;i<FILTER_NUM;i++)
	{
		temp1 += ACC_X_BUF[i];
		temp2 += ACC_Y_BUF[i];
		temp3 += ACC_Z_BUF[i];
	}
	acc_out->X = temp1 / FILTER_NUM;
	acc_out->Y = temp2 / FILTER_NUM;
	acc_out->Z = temp3 / FILTER_NUM;
	filter_cnt++;
	if(filter_cnt==FILTER_NUM)	filter_cnt=0;
}
////////////////////////////////////////////////////////////////////////////////
#define Kp 1.3f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.0006f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.001f                   // half the sample period???????

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
void IMUupdate(T_int16_xyz *gyr, T_int16_xyz *acc, T_float_angle *angle)
{
	float ax = acc->X,ay = acc->Y,az = acc->Z;
	float gx = gyr->X,gy = gyr->Y,gz = gyr->Z;
  float norm;
//  float hx, hy, hz, bx, bz;
  float vx, vy, vz;// wx, wy, wz;
  float ex, ey, ez;

  // ???????????
  float q0q0 = q0*q0;
  float q0q1 = q0*q1;
  float q0q2 = q0*q2;
//  float q0q3 = q0*q3;
  float q1q1 = q1*q1;
//  float q1q2 = q1*q2;
  float q1q3 = q1*q3;
  float q2q2 = q2*q2;
  float q2q3 = q2*q3;
  float q3q3 = q3*q3;
	
	if(ax==0||ay==0||az==0)
 		return;
		
	gx *= Gyro_Gr;
	gy *= Gyro_Gr;
	gz *= Gyro_Gr;
		
  norm = sqrt(ax*ax + ay*ay + az*az);       //acc?????
  ax = ax /norm;
  ay = ay / norm;
  az = az / norm;

  // estimated direction of gravity and flux (v and w)              ?????????/??
  vx = 2*(q1q3 - q0q2);												//????xyz???
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;                           					 //???????????????
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki;								  //???????
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt;					   							//???PI???????,???????
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;				   							//???gz????????????????,??????????????

  // integrate quaternion rate and normalise						   //????????
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

  // normalise quaternion
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

	angle->yaw += gyr->Z*Gyro_G*0.002f;
	
  angle->rol = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3 - AngleOffset_Pit; // pitch
  angle->pit = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3 - AngleOffset_Rol; // roll
}

////////////////////////////////////////////////////////////////
#define HEAD_LPF_FACTOR 20
vs16	head_temp[HEAD_LPF_FACTOR]={0};
u8		head_temp_cnt = 0;
u16		cal_cnt=0;
vs16	mx_min=0,mx_max=0,my_min=0,my_max=0,mz_min=0,mz_max=0;
vs16	Mx_Offset,My_Offset,Mz_Offset;
float	My_Gain,Mz_Gain;

void Cal_Compass(void)
{
	cal_cnt = 1000;
	mx_min=0;
	mx_max=0;
	my_min=0;
	my_max=0;
	mz_min=0;
	mz_max=0;
}
void Get_CompassAngle(T_int16_xyz *mag, T_int16_xyz *acc, float *heading)
{
	static float mx=0,my=0,mz=0;
	if(cal_cnt)
	{
		cal_cnt--;
		if(mx_min>mag->X)	mx_min = mag->X;
		if(mx_max<mag->X)	mx_max = mag->X;
		if(my_min>mag->Y)	my_min = mag->Y;
		if(my_max<mag->Y)	my_max = mag->Y;
		if(mz_min>mag->Z)	mz_min = mag->Z;
		if(mz_max<mag->Z)	mz_max = mag->Z;
		if(cal_cnt==0)
		{
			Mx_Offset = (mx_min+mx_max)/2;
			My_Offset = (my_min+my_max)/2;
			Mz_Offset = (mz_min+mz_max)/2;
			float mx_sum = mx_max - mx_min;
			float my_sum = my_max - my_min;
			float mz_sum = mz_max - mz_min;
			My_Gain = mx_sum/my_sum;
			Mz_Gain = mx_sum/mz_sum;
//			Data_Save();
		}
		
//		if(LED_STA==0)	LED_STA = 2;
	}
	else
	{
		mx = mag->X-Mx_Offset;
		my = (mag->Y-My_Offset)*My_Gain;
		mz = (mag->Z-Mz_Offset)*Mz_Gain;
		
		head_temp[head_temp_cnt++] = atan2(acc->X*mz - acc->Z*mx, acc->Z*my + acc->Y*mz)* 57.3 * 100;
		
		if(head_temp_cnt==HEAD_LPF_FACTOR)	
			head_temp_cnt = 0;
		vs32 sum = 0;
		for(u8 i=0;i<HEAD_LPF_FACTOR;i++)
			sum += head_temp[i];
		*heading = (float)(sum / HEAD_LPF_FACTOR)/100 + 180;
		if(*heading>180)
			*heading -= 360;
		if(*heading<-180)
			*heading += 360;
		
//		if(LED_STA==2)	LED_STA = 0;
	}
}
