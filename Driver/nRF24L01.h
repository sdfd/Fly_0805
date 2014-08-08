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
���������������Щ�������������    24L01����ͼ
�� GND  �� VDD  ��    GND:��Դ��    			  VDD:��Դ1.9V-3.6V
����������������      �� 
�� CE     CSN  ��    CE:оƬƬѡ�˿�A6    CSN:SPIƬѡʹ��A3
��             ��
�� SCK    MOSI ��    SCK:SPIʱ��A2        MOSI:SPI�����ӽ�A5
��             ��
�� MISO   IRQ  ��    MISO:SPI�����ӳ�A4   ISQ:�ж�����
������������������������������
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
//=========================NRF24L01�Ĵ���ָ��===================================
#define RF_READ_REG        0x00  	// ���Ĵ���ָ��
#define RF_WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP1            0xFF  	// ����
//========================SPI(nRF24L01)�Ĵ�����ַ===============================
#define NRF_READ_REG    0x00  // Define read command to register
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define NRFRegSTATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
//��״̬
#define RX_DR           0x40  //���ݽ���RX FIFO���վ����ж�
#define TX_DS           0x20  //���ݷ���TX FIFO���ͳɹ��ж�      
#define MAX_RT          0x10  //����ʹ����ж�      
#define RX_P_NO         0x0e  //���յ������ݵ�ͨ��
#define TX_FULL         0x01  //����TX FIFO��
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
