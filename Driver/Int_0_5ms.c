#include "Int_0_5ms.h"
#include "SysConfig.h"
#include "User_MPU6050.h"
#include "data_transfer.h"

void Int_0_5msInit(void){
	    //
    // Enable the peripherals used by this example.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	  ROM_SysCtlDelay(3);
	    //
    // Configure the two 32-bit periodic timers.
    //
    ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet()/2000);
    //
    // Setup the interrupts for the timer timeouts.
    //
    ROM_IntEnable(INT_TIMER0A);
    ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //
    // Enable the timers.
    //
    ROM_TimerEnable(TIMER0_BASE, TIMER_A);
}
//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void Timer0_IRQ(void)
{
    //
    // Clear the timer interrupt.
    //
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		
		static u8 ms1_cnt=0;
//	static u16 s1_cnt=0;
	
//	if(TIM3->SR & TIM_IT_Update)		//if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
//	{     
//		TIM3->SR = ~TIM_FLAG_Update;//TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);   //清除中断标志
//		TIM3_IRQCNT ++;
		
		ms1_cnt++;
//		s1_cnt++;
		if(ms1_cnt==2)
		{
			ms1_cnt = 0;
			
			FLAG_ATT = 1;
		}
//		else
//		{
//			MS5611_loop();
//		}
//		if(s1_cnt==2000)
//		{
//			s1_cnt = 0;
//			debug_cntshow = Debug_cnt;
//			Debug_cnt = 0;
//		}
		Data_Exchange();
//	}
    //
    // Update the interrupt status on the display.
    //
    //ROM_IntMasterDisable();

    //ROM_IntMasterEnable();
}
