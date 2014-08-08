#ifndef __USER_UART_T__
#define __USER_UART_T__
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count);
void UARTInit(uint32_t bps);
void UARTIRQ(void);
#endif
