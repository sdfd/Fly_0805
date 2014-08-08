#include "tim_pwm_in.h"
static T_RC_Data 	Rc_Data;//1000~2000

void Rc_GetValue(T_RC_Data *temp)
{
	temp->THROTTLE	= Rc_Data.THROTTLE;
	temp->YAW		= Rc_Data.YAW;
	temp->ROLL		= Rc_Data.ROLL;
	temp->PITCH		= Rc_Data.PITCH;	
}
void Tim_Pwm_In_Init(void)
{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);//外设使能
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG); 
		ROM_SysCtlDelay(5);
  
		GPIOPinConfigure(GPIO_PJ4_T3CCP0);//引脚功能选择为输入捕获比较模式
	  GPIOPinConfigure(GPIO_PJ5_T3CCP1);
	  GPIOPinConfigure(GPIO_PG0_T4CCP0);
	  GPIOPinConfigure(GPIO_PG1_T4CCP1);
	
	  GPIOPinTypeTimer(GPIO_PORTJ_BASE, GPIO_PIN_4);//配置引脚用作时钟外设
	  GPIOPinTypeTimer(GPIO_PORTJ_BASE, GPIO_PIN_5);     
	  GPIOPinTypeTimer(GPIO_PORTG_BASE, GPIO_PIN_0); 
	  GPIOPinTypeTimer(GPIO_PORTG_BASE, GPIO_PIN_1);

    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);//引脚若上拉
    GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);   
	
    TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP);//定时器配置为独立模式|上升计数模式
    TimerConfigure(TIMER4_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP);
 
    TimerControlEvent(TIMER3_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES); //双边触发
    TimerControlEvent(TIMER3_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    TimerControlEvent(TIMER4_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES); 
    TimerControlEvent(TIMER4_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

    TimerPrescaleSet(TIMER3_BASE, TIMER_A, 0xff);//定时器扩展为0xffffff
    TimerPrescaleSet(TIMER3_BASE, TIMER_B, 0xff);
    TimerPrescaleSet(TIMER4_BASE, TIMER_A, 0xff);
    TimerPrescaleSet(TIMER4_BASE, TIMER_B, 0xff);

    TimerIntEnable(TIMER3_BASE, TIMER_CAPA_EVENT);//定时器中断选择:捕获事件模式
		TimerIntEnable(TIMER3_BASE, TIMER_CAPB_EVENT);
    TimerIntEnable(TIMER4_BASE, TIMER_CAPA_EVENT);
		TimerIntEnable(TIMER4_BASE, TIMER_CAPB_EVENT);
		  
    IntEnable(INT_TIMER3A);//定时器中断使能
		IntEnable(INT_TIMER3B);
		IntEnable(INT_TIMER4A);
		IntEnable(INT_TIMER4B);
   
    TimerEnable(TIMER3_BASE, TIMER_A);//定时器使能
		TimerEnable(TIMER3_BASE, TIMER_B);
		TimerEnable(TIMER4_BASE, TIMER_A);
		TimerEnable(TIMER4_BASE, TIMER_B);
}
void Timer3A_IRQ(void)//PJ4
{
	static uint32_t start3A,end3A;
	TimerIntClear(TIMER3_BASE,TIMER_CAPA_EVENT);
	if(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_4) == GPIO_PIN_4){
		start3A = TimerValueGet(TIMER3_BASE, TIMER_A);
	}
	else{
		end3A = TimerValueGet(TIMER3_BASE, TIMER_A);
		if(end3A>start3A){
			Rc_Data.THROTTLE = (end3A-start3A)/80;
//			UARTprintf("THROTTLE Value: %dus\n",Rc_Data.THROTTLE);
		}
		else if(end3A<start3A)
			Rc_Data.THROTTLE = (end3A+0xffffff-start3A)/80;
//			UARTprintf("THROTTLE Value: %dus\n",Rc_Data.THROTTLE);
	}
}
void Timer3B_IRQ(void)//PJ5
{
	static uint32_t start3B,end3B;
	TimerIntClear(TIMER3_BASE,TIMER_CAPB_EVENT);
	if(GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_5) == GPIO_PIN_5){
		start3B = TimerValueGet(TIMER3_BASE, TIMER_B);
	}
	else{
		end3B = TimerValueGet(TIMER3_BASE, TIMER_B);
		if(end3B>start3B){
			Rc_Data.YAW = (end3B-start3B)/80;
//			UARTprintf("YAW Value: %dus\n",Rc_Data.YAW);
		}
		else if(end3B<start3B)
			Rc_Data.YAW = (end3B+0xffffff-start3B)/80;
//			UARTprintf("YAW Value: %dus\n",Rc_Data.YAW);
	}
}
void Timer4A_IRQ(void)//PG0
{
	static uint32_t start4A,end4A;
	TimerIntClear(TIMER4_BASE,TIMER_CAPA_EVENT);
	if(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_0) == GPIO_PIN_0){
		start4A = TimerValueGet(TIMER4_BASE, TIMER_A);
	}
	else{
		end4A = TimerValueGet(TIMER4_BASE, TIMER_A);
		if(end4A>start4A){
			Rc_Data.ROLL = (end4A-start4A)/80;
//			UARTprintf("ROLL Value: %dus\n",Rc_Data.ROLL);
		}
		else if(end4A<start4A)
			Rc_Data.ROLL = (end4A+0xffffff-start4A)/80;
//			UARTprintf("ROLL Value: %dus\n",Rc_Data.ROLL);
	}
}
void Timer4B_IRQ(void)//PG1
{
	static uint32_t start4B,end4B;
	TimerIntClear(TIMER4_BASE,TIMER_CAPB_EVENT);
	if(GPIOPinRead(GPIO_PORTG_BASE, GPIO_PIN_1) == GPIO_PIN_1){
		start4B = TimerValueGet(TIMER4_BASE, TIMER_B);
	}
	else{
		end4B = TimerValueGet(TIMER4_BASE, TIMER_B);
		if(end4B>start4B){
			Rc_Data.PITCH = (end4B-start4B)/80;
//			UARTprintf("PITCH Value: %dus\n",Rc_Data.PITCH);
		}
		else if(end4B<start4B)
			Rc_Data.PITCH = (end4B+0xffffff-start4B)/80;
//			UARTprintf("PITCH Value: %dus\n",Rc_Data.PITCH);
	}
}
