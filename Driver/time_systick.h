#ifndef __TIME_H__
#define __TIME_H__
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

void SysTickIRQ(void);
void time_init(void);
uint64_t time_nowUs(void);
uint32_t time_nowMs(void);
void time_waitUs(uint32_t delay);
void time_waitMs(uint32_t delay);
#endif
