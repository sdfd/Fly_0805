#include "nRF24L01.h"
//=============================RF24l01״̬=====================================
uint8_t  TX_ADDRESS[TX_ADR_WIDTH]  = {0xa0,0xa1,0xa2,0xa3,0xa4};//0x34,0x43,0x10,0x10,0x01};  // ·�ɽڵ��ַ
uint8_t  RX_ADDRESS1[TX_ADR_WIDTH] = {0xe0,0xe1,0xe2,0xe3,0xe4};//д��ַ�Ĵ�������д���ֽڣ���˺��漸���ֽ���ͬ
uint8_t  RX_ADDRESS2[TX_ADR_WIDTH] = {0xe1,0xe1,0xe2,0xe3,0xe4};
//uint8_t  RX_ADDRESS3[TX_ADR_WIDTH] = {0xe2,0xe1,0xe2,0xe3,0xe4};
//uint8_t  RX_ADDRESS4[TX_ADR_WIDTH] = {0xe3,0xe1,0xe2,0xe3,0xe4};
//uint8_t  RX_ADDRESS5[TX_ADR_WIDTH] = {0xe4,0xe1,0xe2,0xe3,0xe4};

uint8_t RX_Number;
uint8_t status_in,status_out;
bool RX_Data_Ready,TX_Data_Success,TX_Data_Failed=1;

void SSI0SetToSPIInit(void){
		//SPI���ö˿�����
		ROM_GPIOPinConfigure(GPIO_PA2_SSI0CLK);//SCK
    //ROM_GPIOPinConfigure(GPIO_PA3_SSI0FSS);//ʹ��SPIƬѡ
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);//ʹ�����ģ��SPIƬѡCSN
    ROM_GPIOPinConfigure(GPIO_PA4_SSI0RX);//MISO
    ROM_GPIOPinConfigure(GPIO_PA5_SSI0TX);//MOSI
		ROM_GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 /*| GPIO_PIN_3*/ |GPIO_PIN_2);
		ROM_SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,SSI_MODE_MASTER, 1000000, 8);
		ROM_SSIEnable(SSI0_BASE);
}
//==============================================================================
//������uint SPI_RW(uint uuint8_t)
//���ܣ�NRF24L01��SPIдʱ��
//******************************************************************************
uint8_t SPI_RW(uint8_t data){
		uint32_t temp;
		SSIDataPut(SSI0_BASE,data);
		SSIDataGet(SSI0_BASE, &temp);
		return (uint8_t)temp;
}
//****************************************************************************************************
//������uuint8_t SPI_Read(uuint8_t reg)
//���ܣ�NRF24L01��SPIʱ��
//****************************************************************************************************
uint8_t SPI_Read(uint8_t reg){
		uint8_t reg_val;
		RF24L01_CSN_0;           // CSN low, initialize SPI communication...
		//��д�Ĵ�����ַ���������ǼĴ����ĵ�ַ�������������Ĳ��ǼĴ��������ֵ
		SPI_RW(reg);            // Select register to read from..
		reg_val = SPI_RW(0);    // ..then read registervalue
		RF24L01_CSN_1;         // CSN high, terminate SPI communication
		return(reg_val);       // return register value
}
//****************************************************************************************************/
//���ܣ�NRF24L01��д�Ĵ�������
//****************************************************************************************************/
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value){
		uint8_t status1;
		RF24L01_CSN_0;                   // CSN low, init SPI transaction
		status1 = SPI_RW(reg);      // select register
		SPI_RW(value);             // ..and write value to it..
		RF24L01_CSN_1;                   // CSN high again
		return(status1);            // return nRF24L01 status uuint8_t
}
//****************************************************************************************************/
//������uint SPI_Read_Buf(uuint8_t reg, uuint8_t *pBuf, uuint8_t uuint8_ts)
//����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uuint8_ts���������ݵĸ���
//****************************************************************************************************/
uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts){
		uint8_t status2,uuint8_t_ctr;
		RF24L01_CSN_0;                    		// Set CSN low, init SPI tranaction
		status2 = SPI_RW(reg);       		// Select register to write to and read status uuint8_t
		for(uuint8_t_ctr=0;uuint8_t_ctr<uint8_ts;uuint8_t_ctr++)
				pBuf[uuint8_t_ctr] = SPI_RW(0);    // 
		RF24L01_CSN_1;                           
		return(status2);                    // return nRF24L01 status uuint8_t
}
//*********************************************************************************************************
//������uint SPI_Write_Buf(uuint8_t reg, uuint8_t *pBuf, uuint8_t uuint8_ts)
//����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uuint8_ts��д�����ݵĸ���
//*********************************************************************************************************/
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uint8_ts){
		uint8_t status1,uuint8_t_ctr;
		RF24L01_CSN_0;             //SPIʹ��       
		status1 = SPI_RW(reg);   
		for(uuint8_t_ctr=0; uuint8_t_ctr<uint8_ts; uuint8_t_ctr++) //
				SPI_RW(*pBuf++);
		RF24L01_CSN_1;           //�ر�SPI
		return(status1);    		  // 
}
void RF24L01_Init(void){
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);//����ʹ��
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);//CE����Ϊ���ģʽ
	SSI0SetToSPIInit();//��ʼ��SSI0ΪSPI����
}
void RF24L01_ON(void){
  RF24L01_CE_0 ;    // Chip Enable
  RF24L01_CSN_1;   // SPI disable 
  ROM_SysCtlDelay(5*(SysCtlClockGet()/ 3000));
  RF24L01_CE_0;
  SPI_RW_Reg(RF_WRITE_REG + EN_AA, 0x3f);       // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	//status_out = SPI_Read(EN_AA);
  SPI_RW_Reg(RF_WRITE_REG + EN_RXADDR, 0x3f);   // ʹ�ܽ���ͨ��0
  SPI_RW_Reg(RF_WRITE_REG + SETUP_RETR, 0x05);  // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
  SPI_RW_Reg(RF_WRITE_REG + RF_CH, 0);         // ѡ����Ƶͨ��0x40
  SPI_RW_Reg(RF_WRITE_REG + RF_SETUP, 0x07);    // ���ݴ�����1Mbps�����书��0dBm���������Ŵ�������
  RF24L01_CE_1;
  ROM_SysCtlDelay(5*(SysCtlClockGet()/ 3000));
  RF24L01_CE_0;
}
void RF24L01_OFF(void){
  RF24L01_CE_0;
  SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0);              // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
  RF24L01_CE_1;
  ROM_SysCtlDelay(5*(SysCtlClockGet()/ 3000));
  RF24L01_CE_0;
}

/**************************************************
������RX_Mode()

������
    �����������nRF24L01Ϊ����ģʽ���ȴ����շ����豸�����ݰ�

����ģʽ����ѡ�������ͨ������ѡ��ͨ��0~1ʱ����ֱд��ַ
��ѡ��ͨ��2~5ʱ������дͨ��1�ĵ�ַ����Ϊ2~5ͨ����ַֻ��һ���ֽ�
*************************************************/
void RX_Mode(void){
      RF24L01_CE_0;
      SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
      SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P1, RX_ADDRESS1, TX_ADR_WIDTH); 
      SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P2, RX_ADDRESS2, 1);
      
      SPI_RW_Reg(RF_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);  // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ�ȣ�32�ֽڳ���
      SPI_RW_Reg(RF_WRITE_REG + RX_PW_P1, TX_PLOAD_WIDTH);//32�ֽڳ���
      SPI_RW_Reg(RF_WRITE_REG + RX_PW_P2, TX_PLOAD_WIDTH);//32�ֽڳ��� 
      
      SPI_RW_Reg(FLUSH_RX,0);
      SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0f);              // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
      SPI_RW_Reg(RF_WRITE_REG+NRFRegSTATUS,0x70);   //��״̬�Ĵ���
      RF24L01_CE_1;                                            // ����CE���������豸

}
/**************************************************/

/**************************************************
������TX_Mode()

������
    �����������nRF24L01Ϊ����ģʽ����CE=1��������10us����
	130us���������䣬���ݷ��ͽ����󣬷���ģ���Զ�ת�����
	ģʽ�ȴ�Ӧ���źš�

����ģʽ��Ӧ�����ͨ������ѡ��0ͨ����ѡ������ͨ�����޷����ܵ�Ӧ��λ
**************************************************/
void TX_Mode(void){
	RF24L01_CE_0;
	SPI_Write_Buf(RF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);     // д�뷢�͵�ַ
	SPI_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);  // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ,��������Ϊ����ͨ��
			
			SPI_RW_Reg(FLUSH_TX,0);
	SPI_RW_Reg(RF_WRITE_REG + CONFIG, 0x0e);      // CRCʹ�ܣ�16λCRCУ�飬�ϵ�
			SPI_RW_Reg(RF_WRITE_REG+NRFRegSTATUS,0x70);   //��״̬�Ĵ���
	RF24L01_CE_1;
	ROM_SysCtlDelay(5*(SysCtlClockGet()/ 3000));
	RF24L01_CE_0;
}
//******************************************************************************************************/
//������unsigned uint8_t nRF24L01_RxPacket(unsigned uint8_t* rx_buf)
//���ܣ����ݶ�ȡ�����rx_buf���ջ�������
//******************************************************************************************************/
uint8_t nRF24L01_RxPacket(uint8_t* rx_buf)
{
	uint8_t status;
	status=SPI_Read(NRFRegSTATUS);
	if(status&RX_DR){
		RF24L01_CE_0;
    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
    RF24L01_CE_1;		 //�ø�CE���������ݷ���
		SPI_RW_Reg(RF_WRITE_REG+NRFRegSTATUS,0x70);   //��״̬�Ĵ���
		return (RX_Number+1);     //���յ��򷵻�����ͨ�����
	}
	else{
		SPI_RW_Reg(RF_WRITE_REG+NRFRegSTATUS,0x70);   //��״̬�Ĵ���
		return 0;//���򷵻�0����ʾδ���ܵ��µ�����
	}
}
//***********************************************************************************************************
//������void nRF24L01_TxPacket(uint8_t * tx_buf)
//���ܣ����� tx_buf������
//**********************************************************************************************************/
bool NRF_TxPacket(uint8_t * tx_buf,uint8_t length)
{
	  uint8_t status;
		status=SPI_Read(NRFRegSTATUS);
	  if(status&MAX_RT){//��������ط�����
			SPI_RW_Reg(RF_WRITE_REG+NRFRegSTATUS,MAX_RT);   //��״̬�Ĵ���
			return false;
		}
    RF24L01_CE_0;			//StandBy Iģʽ	
    SPI_Write_Buf(WR_TX_PLOAD, tx_buf, length); 			 // װ������	
    RF24L01_CE_1;		 //�ø�CE���������ݷ���
		status=SPI_Read(NRFRegSTATUS);
		if(status&TX_DS){//���ͳɹ�
			SPI_RW_Reg(RF_WRITE_REG+NRFRegSTATUS,TX_DS);   //��״̬�Ĵ���
			return true;
		}
		else{
			return false;
		}
}
void NRF24L01_Init()
{
	RF24L01_Init();
	RF24L01_ON();
	TX_Mode();
}
	
