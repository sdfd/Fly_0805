#include "User_MPU6050.h"
volatile bool g_bMPU6050Done; 
tI2CMInstance sI2CInst;
tMPU6050 sMPU6050;
uint8_t deviceID;//设备号
uint8_t PowerInfo;//电源信息

T_int16_xyz		GYRO_OFFSET,ACC_OFFSET;			//零漂
static uint8_t	GYRO_OFFSET_OK = 0;
static uint8_t	ACC_OFFSET_OK = 0;
static uint8_t  *mpu6050_buffer;					//iic读取后存放数据
//
// The function that is provided by this example as a callback when MPU6050
// transactions have completed.
//
void MPU6050_Callback(void *pvCallbackData, uint_fast8_t ui8Status)
{
		//
		// See if an error occurred.
		//
		if(ui8Status != I2CM_STATUS_SUCCESS)
		{
		//
		// An error occurred, so handle it here if required.
		//
		}
		//
		// Indicate that the MPU6050 transaction has completed.
		//
		g_bMPU6050Done = true;
}


//*****************************************************************************
//
// Called by the NVIC as a result of I2C3 Interrupt. I2C3 is the I2C connection
// to the MPU6050.
//
//*****************************************************************************
void I2C0IRQ(void)
{
    //
    // Pass through to the I2CM interrupt handler provided by sensor library.
    // This is required to be at application level so that I2CMIntHandler can
    // receive the instance structure pointer as an argument.
    //
    I2CMIntHandler(&sI2CInst);
}
void MPU6050_Init(void){
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		ROM_SysCtlDelay(3);
		ROM_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
    ROM_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	
		ROM_GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
		ROM_GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
	
		//ROM_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
    //GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0);
    //ROM_GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
    //ROM_IntEnable(INT_GPIOB);
		ROM_I2CMasterEnable(I2C0_BASE);
		I2CMInit(&sI2CInst, I2C0_BASE, INT_I2C0, 0xff, 0xff, ROM_SysCtlClockGet());
		
		g_bMPU6050Done = false;
		MPU6050Init(&sMPU6050, &sI2CInst, 0x68, MPU6050_Callback, 0);
		while(!g_bMPU6050Done);
		
		g_bMPU6050Done = false;
		MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_PWR_MGMT_1,
		~MPU6050_PWR_MGMT_1_SLEEP,
		0, MPU6050_Callback,
		0);//从Sleep模式中激活设备，否则无法写入寄存器
		while(!g_bMPU6050Done);
		
		// Configure the MPU6050 for +/- 4 g accelerometer range.
		//
		g_bMPU6050Done = false;
		//先读取寄存器的值，再与上Mask，然后再或上Value，将得到的值写入寄存器
		MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_PWR_MGMT_1,
		~MPU6050_PWR_MGMT_1_CLKSEL_M,
		MPU6050_PWR_MGMT_1_CLKSEL_XG, MPU6050_Callback,
		0);
		while(!g_bMPU6050Done);
		
		g_bMPU6050Done = false;
		MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_ACCEL_CONFIG,
		~MPU6050_ACCEL_CONFIG_AFS_SEL_M,
		MPU6050_ACCEL_CONFIG_AFS_SEL_4G, MPU6050_Callback,
		0);
		while(!g_bMPU6050Done);
		
		g_bMPU6050Done = false;
		MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_CONFIG,
		~MPU6050_CONFIG_DLPF_CFG_M,
		MPU6050_CONFIG_DLPF_CFG_44_42, MPU6050_Callback,
		0);
		while(!g_bMPU6050Done);
		
		g_bMPU6050Done = false;
		MPU6050ReadModifyWrite(&sMPU6050, MPU6050_O_GYRO_CONFIG,
		~MPU6050_GYRO_CONFIG_FS_SEL_M,
		MPU6050_GYRO_CONFIG_FS_SEL_2000, MPU6050_Callback,
		0);
		while(!g_bMPU6050Done);
		
		MPU6050Read(&sMPU6050,MPU6050_O_WHO_AM_I,&deviceID,1,MPU6050_Callback,0);//MPU-60X0设备号默认0x68
		ROM_SysCtlDelay(ROM_SysCtlClockGet()/(3*1000));
		MPU6050Read(&sMPU6050,MPU6050_O_PWR_MGMT_1,&PowerInfo,1,MPU6050_Callback,0);//若读到的值是0x00，则设备处于激活状态。0x40则为休眠状态

}

void MPU6050_Read(void){
//		int time;
		g_bMPU6050Done = false;
		MPU6050DataRead(&sMPU6050, MPU6050_Callback, 0);
//		time = time_nowUs();
		while(!g_bMPU6050Done)
		{
//			if (time+200>time_nowUs())
//				return;
		}
		mpu6050_buffer = sMPU6050.pui8Data;
}
#define 	MPU6050_MAX		32767
#define		MPU6050_MIN		-32768
void MPU6050_Dataanl(T_int16_xyz *data_tempacc,T_int16_xyz *data_tempgyr)
{
	vs32 acc_x,acc_y,acc_z,gyr_x,gyr_y,gyr_z;
	acc_x=((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) - ACC_OFFSET.X;
	acc_y=((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) - ACC_OFFSET.Y;
	acc_z=((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) - ACC_OFFSET.Z;
	//????ADC
	gyr_x=((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]) - GYRO_OFFSET.X;
	gyr_y=((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11]) - GYRO_OFFSET.Y;
	gyr_z=((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13]) - GYRO_OFFSET.Z;
	acc_x>MPU6050_MAX ? MPU6050_MAX:acc_x;
	acc_x<MPU6050_MIN ? MPU6050_MIN:acc_x;
	acc_y>MPU6050_MAX ? MPU6050_MAX:acc_y;
	acc_y<MPU6050_MIN ? MPU6050_MIN:acc_y;
	acc_z>MPU6050_MAX ? MPU6050_MAX:acc_z;
	acc_z<MPU6050_MIN ? MPU6050_MIN:acc_z;
	gyr_x>MPU6050_MAX ? MPU6050_MAX:gyr_x;
	gyr_x<MPU6050_MIN ? MPU6050_MIN:gyr_x;
	gyr_y>MPU6050_MAX ? MPU6050_MAX:gyr_y;
	gyr_y<MPU6050_MIN ? MPU6050_MIN:gyr_y;
	gyr_z>MPU6050_MAX ? MPU6050_MAX:gyr_z;
	gyr_z<MPU6050_MIN ? MPU6050_MIN:gyr_z;
	data_tempacc->X = acc_x;
	data_tempacc->Y = acc_y;
	data_tempacc->Z = acc_z;
	data_tempgyr->X = gyr_x;
	data_tempgyr->Y = gyr_y;
	data_tempgyr->Z = gyr_z;
	
	if(!GYRO_OFFSET_OK)
	{
		static int32_t	tempgx=0,tempgy=0,tempgz=0;
		static uint8_t cnt_g=0,cnt_pass;
		if(cnt_pass>=200)
		{cnt_g++;
// 		LED1_ON;
		if(cnt_g==0)
		{
			GYRO_OFFSET.X=0;
			GYRO_OFFSET.Y=0;
			GYRO_OFFSET.Z=0;
			tempgx = 0;
			tempgy = 0;
			tempgz = 0;
			cnt_g = 1;
			return;
		}
		tempgx+= data_tempgyr->X;
		tempgy+= data_tempgyr->Y;
		tempgz+= data_tempgyr->Z;
		if(cnt_g==200)
		{
			GYRO_OFFSET.X=tempgx/cnt_g;
			GYRO_OFFSET.Y=tempgy/cnt_g;
			GYRO_OFFSET.Z=tempgz/cnt_g;
			cnt_g = 0;
			GYRO_OFFSET_OK = 1;
			//EE_SAVE_GYRO_OFFSET();//保存数据
			return;
		}}else cnt_pass++;
		
		
	}
	if(!ACC_OFFSET_OK)
	{
		static int32_t	tempax=0,tempay=0,tempaz=0;
		static uint8_t cnt_a=0;
// 		LED1_ON;
		if(cnt_a==0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return;
		}
		tempax+= data_tempacc->X;
		tempay+= data_tempacc->Y;
		//tempaz+= MPU6050_ACC_LAST.Z;
		if(cnt_a==200)
		{
			ACC_OFFSET.X=tempax/cnt_a;
			ACC_OFFSET.Y=tempay/cnt_a;
			ACC_OFFSET.Z=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
			//EE_SAVE_ACC_OFFSET();//保存数据
			return;
		}
		cnt_a++;		
	}
}
void MPU6050_CalOff_Acc(void)
{
	ACC_OFFSET_OK = 0;
}
void MPU6050_CalOff_Gyr(void)
{
	GYRO_OFFSET_OK = 0;
}
