#ifndef __NRF24L01_H__
#define __NRF24L01_H__
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/ssi.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/rom.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
/*******************************************************************************
┌──────┬──────┐    24L01引脚图
│ GND  │ VDD  │    GND:电源地    			  VDD:电源1.9V-3.6V
├──────┘      │ 
│ CE     CSN  │    CE:芯片片选端口A6    CSN:SPI片选使能A3
│             │
│ SCK    MOSI │    SCK:SPI时钟A2        MOSI:SPI主出从进A5
│             │
│ MISO   IRQ  │    MISO:SPI主进从出A4   ISQ:中断引脚
└─────────────┘
*******************************************************************************/


//==============================RF24L01_CE======================================
#define  RF24L01_CSN_0        ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0)         
#define  RF24L01_CSN_1        ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3)        
//=============================RF24L01_CSN======================================
#define  RF24L01_CE_0       ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0)       
#define  RF24L01_CE_1       ROM_GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6)     
//=============================RF24L01_SCK======================================
//#define  RF24L01_SCK_0       GPIO_ResetBits(GPIOF,GPIO_Pin_2)      
//#define  RF24L01_SCK_1       GPIO_SetBits(GPIOF,GPIO_Pin_2)   
//=============================RF24L01_MISO=====================================
//#define  RF24L01_MISO        GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)

//============================= RF24L01_MOSI====================================
//#define  RF24L01_MOSI_0      GPIO_ResetBits(GPIOF,GPIO_Pin_3)
//#define  RF24L01_MOSI_1      GPIO_SetBits(GPIOF,GPIO_Pin_3)
//==========================RF24L01_IRQ=========================================
//#define  RF24L01_IRQ       C13 


//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  32  	// 32 TX payload
#define RX_PLOAD_WIDTH  32  	// 32 uints TX payload
//=========================NRF24L01寄存器指令===================================
#define RF_READ_REG        0x00  	// 读寄存器指令
#define RF_WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP1            0xFF  	// 保留
//========================SPI(nRF24L01)寄存器地址===============================
#define NRF_READ_REG    0x00  // Define read command to register
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define NRFRegSTATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置
//读状态
#define RX_DR           0x40  //数据接收RX FIFO接收就绪中断
#define TX_DS           0x20  //数据发送TX FIFO发送成功中断      
#define MAX_RT          0x10  //最大发送次数中断      
#define RX_P_NO         0x0e  //接收到的数据的通道
#define TX_FULL         0x01  //发送TX FIFO满
#define R_RX_PL_WID   	0x60

uint8_t SPI_RW(uint8_t data);
uint8_t SPI_Read(uint8_t reg);
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value);
uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts);
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts);

void RF24L01_Init(void);
void RF24L01_ON(void);
void RF24L01_OFF(void);

void RX_Mode(void);
void TX_Mode(void);
void TX_Mode1(void);
void TX_Mode2(void);

uint8_t nRF24L01_RxPacket(uint8_t* rx_buf);
bool NRF_TxPacket(uint8_t * tx_buf,uint8_t length);

void EXTI_PE5_Config(void);

void NRF24L01_Init(void);


#endif
