#include "SysConfig.h"
#include "User_mpu6050.h"
void SysTick_Handler()
{
	SysTickIRQ();
}

void UART0_Handler()
{
	UARTIRQ();
}
void I2C0_Handler()
{
	I2C0IRQ();
}
//void GPIOA_Handler()
//{
//	IntGPIOAIRQ();
//}
void TIMER0A_Handler()
{
	Timer0_IRQ();
}
void TIMER3A_Handler()
{
	Timer3A_IRQ();
}
void TIMER3B_Handler()
{
	Timer3B_IRQ();
}
void TIMER4A_Handler()
{
	Timer4A_IRQ();
}
void TIMER4B_Handler()
{
	Timer4B_IRQ();
}

