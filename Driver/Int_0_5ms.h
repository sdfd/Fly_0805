#ifndef __INT_0_5MS_H__
#define __INT_0_5MS_H__

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/rom.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
void Int_0_5msInit(void);
void Timer0_IRQ(void);
#endif
