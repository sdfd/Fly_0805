#ifndef LED_H
#define LED_H

#include "SysConfig.h"

#define LED_B_ON		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_0, GPIO_PIN_0)		
#define LED_B_OFF		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_0, 0)
#define LED_R_ON		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_1, GPIO_PIN_1)		
#define LED_R_OFF		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_1, 0)
#define LED_G_ON		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_2, GPIO_PIN_2)		
#define LED_G_OFF		GPIOPinWrite(GPIO_PORTM_BASE, GPIO_PIN_2, 0)

void LED_Init(void);
void LED_Loop(void);
void LEDx_ONOFF(u8 num);
#endif
