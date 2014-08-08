#include "User_UART.h"
#include "Data_Transfer.h"
//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************
static u8 RxBuffer[50];
static u8 RxState = 0;
void UARTIRQ(void)
{
    uint32_t ui32Status;
		u8 com_data;
    //
    // Get the interrrupt status.
    //
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
//        ROM_UARTCharPutNonBlocking(UART0_BASE,ROM_UARTCharGetNonBlocking(UART0_BASE));
			com_data = ROM_UARTCharGetNonBlocking(UART0_BASE);
			static u8 _data_len = 0,_data_cnt = 0;
			if(RxState==0&&com_data==0xAA)
			{
				RxState=1;
				RxBuffer[0]=com_data;
			}
			else if(RxState==1&&com_data==0xAF)
			{
				RxState=2;
				RxBuffer[1]=com_data;
			}
			else if(RxState==2&&com_data>0&&com_data<0XF1)
			{
				RxState=3;
				RxBuffer[2]=com_data;
			}
			else if(RxState==3&&com_data<50)
			{
				RxState = 4;
				RxBuffer[3]=com_data;
				_data_len = com_data;
				_data_cnt = 0;
			}
			else if(RxState==4&&_data_len>0)
			{
				_data_len--;
				RxBuffer[4+_data_cnt++]=com_data;
				if(_data_len==0)
					RxState = 5;
			}
			else if(RxState==5)
			{
				RxState = 0;
				RxBuffer[4+_data_cnt]=com_data;
				Data_Receive_Anl(RxBuffer,_data_cnt+5);
			}
			else
				RxState = 0;                                   
    }
}

//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
        ROM_UARTCharPutNonBlocking(UART0_BASE, *pui8Buffer++);
    }
}
void UARTInit(uint32_t bps){
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
		ROM_SysCtlDelay(3);
		ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), bps,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));
//		ROM_UARTFIFOEnable(UART0_BASE);
//		ROM_UARTFIFOLevelSet(UART0_BASE,UART_FIFO_TX7_8,UART_FIFO_RX7_8);
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}
