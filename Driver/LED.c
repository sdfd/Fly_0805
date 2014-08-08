#include "LED.h"
void LED_Init()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	ROM_SysCtlDelay(5);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2);	
}
void LED_Loop()
{
	LED_B_ON;
	ROM_SysCtlDelay((ROM_SysCtlClockGet())/3/2);
	LED_R_ON;
	ROM_SysCtlDelay((ROM_SysCtlClockGet())/3/2);
	LED_G_ON;
	ROM_SysCtlDelay((ROM_SysCtlClockGet())/3/2);
	LED_B_OFF;
	ROM_SysCtlDelay((ROM_SysCtlClockGet())/3/2);
	LED_R_OFF;
	ROM_SysCtlDelay((ROM_SysCtlClockGet())/3/2);
	LED_G_OFF;
	ROM_SysCtlDelay((ROM_SysCtlClockGet())/3/2);
}
void LEDx_ONOFF(u8 numx)
{
	static uint32_t Ledx_temp;
	static u8 Ledx_sta=1,busy=0;
	static u8 num;
	num = numx;
	if(Ledx_sta)
	{
		if(!busy)
		{
			Ledx_temp = time_nowMs();
			busy = 1;
		}
		else if((Ledx_temp+200) < time_nowMs())
			{
				switch (num)
				{
					case 1 : LED_B_ON; break;
					case 2 : LED_R_ON; break;
					case 3 : LED_G_ON; break;
					default : ;
				}
				busy = 0;
				Ledx_sta = 0;
			}
	}
	else 
	{
		if(!busy)
		{
			Ledx_temp = time_nowMs();
			busy = 1;
		}
		else if((Ledx_temp + 200) < time_nowMs())
		{
				switch (num)
				{
					case 1 : LED_B_OFF; break;
					case 2 : LED_R_OFF; break;
					case 3 : LED_G_OFF; break;
					default : ;
				}
			busy = 0;
			Ledx_sta = 1;
		}
	}
}
