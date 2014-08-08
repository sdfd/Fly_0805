#include "time_systick.h"
static struct 
{
    volatile uint32_t msPeriod;   //正周期的时间，ms
    uint32_t ticksPerUs;  // 每us等于的tick数
    uint32_t ticksPerMs;  // 每ms等于的tick数
    uint32_t msPerPeriod; // 每周期的ms数
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
// 获取当前时间,us?
uint64_t time_nowUs(void)
{
    return time.msPeriod * (uint64_t)1000 + (ROM_SysTickPeriodGet() - ROM_SysTickValueGet()) / time.ticksPerUs;
}
// 获取当前时间,ms?
uint32_t time_nowMs(void)
{
    return time.msPeriod + (ROM_SysTickPeriodGet() - ROM_SysTickValueGet()) / time.ticksPerMs;
}
// 延时delay us,delay>=4才准确
void time_waitUs(uint32_t delay)
{
    uint64_t target = time_nowUs() + delay - 2;
    while(time_nowUs() <= target); 
}
// 延时delay ms
void time_waitMs(uint32_t delay)
{
    time_waitUs(delay * 1000);
}
