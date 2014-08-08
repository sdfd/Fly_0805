#include "time_systick.h"
static struct 
{
    volatile uint32_t msPeriod;   //�����ڵ�ʱ�䣬ms
    uint32_t ticksPerUs;  // ÿus���ڵ�tick��
    uint32_t ticksPerMs;  // ÿms���ڵ�tick��
    uint32_t msPerPeriod; // ÿ���ڵ�ms��
}time;

//*****************************************************************************
//
// The interrupt handler for the for Systick interrupt.
//
//*****************************************************************************
void SysTickIRQ(void)
{
    time.msPeriod += time.msPerPeriod;
}
void time_init(void){
			time.msPeriod = 0;
			time.ticksPerUs = ROM_SysCtlClockGet() / 1e6;
			time.ticksPerMs = ROM_SysCtlClockGet() / 1e3;
			time.msPerPeriod = 10;
	    ROM_SysTickPeriodSet(ROM_SysCtlClockGet()/(1000/time.msPerPeriod));
	    ROM_SysTickIntEnable();
			ROM_SysTickEnable();
}
// ��ȡ��ǰʱ��,us?
uint64_t time_nowUs(void)
{
    return time.msPeriod * (uint64_t)1000 + (ROM_SysTickPeriodGet() - ROM_SysTickValueGet()) / time.ticksPerUs;
}
// ��ȡ��ǰʱ��,ms?
uint32_t time_nowMs(void)
{
    return time.msPeriod + (ROM_SysTickPeriodGet() - ROM_SysTickValueGet()) / time.ticksPerMs;
}
// ��ʱdelay us,delay>=4��׼ȷ
void time_waitUs(uint32_t delay)
{
    uint64_t target = time_nowUs() + delay - 2;
    while(time_nowUs() <= target); 
}
// ��ʱdelay ms
void time_waitMs(uint32_t delay)
{
    time_waitUs(delay * 1000);
}
