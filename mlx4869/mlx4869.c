#include "main.h"
#include "mlx4869.h"
#define ACK	 0 //Ӧ��
#define	NACK 1 //��Ӧ��
#define SA				0x00 //Slave address ����MLX90614ʱ��ַΪ0x00,���ʱ��ַĬ��Ϊ0x5a
#define RAM_ACCESS		0x00 //RAM aess mand RAM��ȡ����
#define EEPROM_ACCESS	0x20 //EEPROM aess mand EEPROM��ȡ����
#define RAM_TOBJ1		0x07 //To1 address in the eeprom Ŀ��1�¶�,��⵽�ĺ����¶� -70.01 ~ 382.19��//
#define SMBUS_PORT	GPIOA      //PB�˿�(�˿ں������������ſ��Զ���)
#define SMBUS_SCK		GPIO_Pin_6 //PB6��SCL
#define SMBUS_SDA		GPIO_Pin_7 //PB7��SDA//
#define RCC_APB2Periph_SMBUS_PORT		RCC_APB2Periph_GPIOB

#define SMBUS_SCK_H()	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)//SMBUS_PORT->BSRR = SMBUS_SCK //�øߵ�ƽ
#define SMBUS_SCK_L()	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)//SMBUS_PORT->BRR = SMBUS_SCK  //�õ͵�ƽ
#define SMBUS_SDA_H()	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)//SMBUS_PORT->BSRR = SMBUS_SDA
#define SMBUS_SDA_L()	    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)
#define SMBUS_SDA_PIN()	    HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7)//SMBUS_PORT->IDR & SMBUS_SDA //��ȡ���ŵ�ƽ/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*//*******************************************************************************
* Function Name  : SMBus_StartBit
* Description    : Generate START condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StartBit(void)
{
	SMBUS_SDA_H();		// Set SDA line
	SMBus_Delay(5);	    // Wait a few microseconds
	SMBUS_SCK_H();		// Set SCL line
	SMBus_Delay(5);	    // Generate bus free time between Stop
	SMBUS_SDA_L();		// Clear SDA line
	SMBus_Delay(5);	    // Hold time after (Repeated) Start// Condition. After this period, the first clock is generated.//(Thd:sta=4.0us min)��SCK=1ʱ����⵽SDA��1��0��ʾͨ�ſ�ʼ���½��أ�
	SMBUS_SCK_L();	    // Clear SCL line
	SMBus_Delay(5);	    // Wait a few microseconds
}/*******************************************************************************
* Function Name  : SMBus_StopBit
* Description    : Generate STOP condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StopBit(void)
{
	SMBUS_SCK_L();		// Clear SCL line
	SMBus_Delay(5);	    // Wait a few microseconds
	SMBUS_SDA_L();		// Clear SDA line
	SMBus_Delay(5);	    // Wait a few microseconds
	SMBUS_SCK_H();		// Set SCL line
	SMBus_Delay(5);	    // Stop condition setup time(Tsu:sto=4.0us min)
	SMBUS_SDA_H();
	SMBus_Delay(5);// Set SDA line��SCK=1ʱ����⵽SDA��0��1��ʾͨ�Ž����������أ�
}/*******************************************************************************
* Function Name  : SMBus_SendByte
* Description    : Send a byte on SMBus
* Input          : Tx_buffer
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t SMBus_SendByte(uint8_t Tx_buffer)
{
	uint8_t	Bit_counter;
	uint8_t	Ack_bit;
	uint8_t	bit_out;
	for(Bit_counter=0; Bit_counter<8; Bit_counter++)
	{
		if (Tx_buffer&0x80)
			{
				bit_out=1;
			}
	   // If the current bit of Tx_buffer is 1 set bit_out
		else
			{
				bit_out=0;  // else clear bit_out
			}
				if(bit_out==1){
				SMBUS_SDA_H();
				}else{
				SMBUS_SDA_L();
				}// Send the current bit on SDA
			SMBus_Delay(5);
				SMBUS_SCK_H();
				SMBus_Delay(5);
				SMBUS_SCK_L();
		Tx_buffer<<=1;				// Get next bit for checking
	if(Bit_counter==7){
		SMBUS_SDA_H();//�ͷ�SDA��
	Ack_bit=SMBus_ReceiveBit();}
} 	// Get acknowledgment bit
	return	Ack_bit;
}/*******************************************************************************
* Function Name  : SMBus_SendBit
* Description    : Send a bit on SMBus 82.5kHz
* Input          : bit_out
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_SendBit(uint8_t bit_out)
{
	if(bit_out==0)
	{
		SMBUS_SDA_L();
	}
	else
	{
		SMBUS_SDA_H();
	}
	SMBus_Delay(2);					// Tsu:dat = 250ns minimum
	SMBUS_SCK_H();					// Set SCL line
	SMBus_Delay(6);					// High Level of Clock Pulse
	SMBUS_SCK_L();					// Clear SCL line
	SMBus_Delay(3);					// Low Level of Clock Pulse
	SMBUS_SDA_H();				    // Master release SDA line ,return;
	}
/*******************************************************************************
* Function Name  : SMBus_ReceiveBit
* Description    : Receive a bit on SMBus
* Input          : None
* Output         : None
* Return         : Ack_bit
*******************************************************************************/
uint8_t SMBus_ReceiveBit(void)
{
	uint8_t Ack_bit;
	SMBUS_SDA_H();          //���ſ��ⲿ������������������
	SMBus_Delay(2);			// High Level of Clock Pulse
	SMBUS_SCK_H();			// Set SCL line
	SMBus_Delay(5);			// High Level of Clock Pulse
	if(SMBUS_SDA_PIN())
	{Ack_bit=1;}
	else{Ack_bit=0;}
	SMBUS_SCK_L();			// Clear SCL line
	SMBus_Delay(3);			// Low Level of Clock Pulse
	return	Ack_bit;
}/*******************************************************************************
* Function Name  : SMBus_ReceiveByte
* Description    : Receive a byte on SMBus
* Input          : ack_nack
* Output         : None
* Return         : RX_buffer
*******************************************************************************/
uint8_t SMBus_ReceiveByte(uint8_t ack_nack)
{
	uint8_t 	RX_buffer=0;
	uint8_t	Bit_Counter=0;
	for(Bit_Counter=0; Bit_Counter<8; Bit_Counter++)
	{
		
		if(SMBus_ReceiveBit())			// Get a bit from the SDA line
		{
			RX_buffer <<= 1;			// If the bit is HIGH save 1  in RX_buffer
			RX_buffer |=0x01;
		}
		else
		{
			RX_buffer <<= 1;			// If the bit is LOW save 0 in RX_buffer
			RX_buffer &=0xfe;
		}
	}
	SMBus_SendBit(ack_nack);			// Sends acknowledgment bit
	return RX_buffer;
}/*******************************************************************************
* Function Name  : SMBus_Delay
* Description    : ��ʱ  һ��ѭ��Լ1us
* Input          : time
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_Delay(uint16_t time)
{
	uint16_t i, j;
	for (i=0; i<5; i++)
{
for (j=0; j<time; j++);
}
}/*******************************************************************************
* Function Name  : SMBus_Init
* Description    : SMBus��ʼ��
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/******************************************************************************** Function Name  : SMBus_ReadMemory* Description    : READ DATA FROM RAM/EEPROM* Input          : slaveAddress, mand* Output         : None* Return         : Data
*******************************************************************************/
uint16_t SMBus_ReadMemory(uint8_t slaveAddress, uint8_t mand)
{
	uint16_t data=1;			// Data storage (DataH:DataL)
	uint8_t Pec;				// PEC byte storage
	uint8_t DataL=0;			// Low data byte storage
	uint8_t DataH=0;			// High data byte storage
	uint8_t arr[6];			// Buffer for the sent bytes
	uint8_t PecReg;			// Calculated PEC byte storage
	uint8_t ErrorCounter;	// Defines the number of the attempts for munication with MLX90614
	ErrorCounter=0x00;				// Initialising of ErrorCounter
	slaveAddress <<= 1;	//2-7λ��ʾ�ӻ���ַ
	do{
repeat:SMBus_StopBit();			    //If slave send NACK stop unication
		--ErrorCounter;				    //Pre-decrement ErrorCounter
		if(!ErrorCounter) 			    //ErrorCounter=0?
			{break;					    //Yes,go out from do-while{}
			}
			SMBus_StartBit();				//Start condition
		if(SMBus_SendByte(slaveAddress))//Send SlaveAddress ���λWr=0��ʾ������д����
			{goto	repeat;			    //Repeat unication again
			}
		if(SMBus_SendByte(mand))	    //Send mand
			{goto	repeat;		    	//Repeat unication again
			}
		SMBus_StartBit();					//Repeated Start condition
		if(SMBus_SendByte(slaveAddress+1))	//Send SlaveAddress ���λRd=1��ʾ������������
		{goto	repeat;             	//Repeat unication again
		}
		DataL = SMBus_ReceiveByte(ACK);	//Read low data,master must send ACK
		DataH = SMBus_ReceiveByte(ACK); //Read high data,master must send ACK
		Pec = SMBus_ReceiveByte(NACK);	//Read PEC byte, master must send NACK
		SMBus_StopBit();				//Stop condition
		arr[5] = slaveAddress;		
		arr[4] = mand;			//
		arr[3] = slaveAddress+1;	//Load array arr
		arr[2] = DataL;				//
		arr[1] = DataH;				//
		arr[0] = 0;					//
		PecReg=PEC_Calculation(arr);//Calculate CRC
		}while(PecReg != Pec);		//If received and calculated CRC are equal go out from do-while{}
		data = (DataH<<8) | DataL;	//data=DataH:DataL
		return data;
}/*******************************************************************************
* Function Name  : PEC_calculation
* Description    : Calculates the PEC of received bytes
* Input          : pec[]
* Output         : None
* Return         : pec[0]-this byte contains calculated crc value
*******************************************************************************/
uint8_t PEC_Calculation(uint8_t pec[])
{
	uint8_t 	crc[6];
	uint8_t	BitPosition=47;
	uint8_t	shift;
	uint8_t	i;
	uint8_t	j;
	uint8_t	temp;
	do
	{
		/*Load pattern value 0x000000000107*/
		crc[5]=0;
		crc[4]=0;
		crc[3]=0;
		crc[2]=0;
		crc[1]=0x01;
		crc[0]=0x07;
		/*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
		BitPosition=47;
		/*Set shift position at 0*/
		shift=0;
		/*Find first "1" in the transmited message beginning from the MSByte byte5*/
		i=5;j=0;
		while((pec[i]&(0x80>>j))==0 && i>0)
			{
			BitPosition--;
			if(j<7){
				j++;
			}else{
			j=0x00;i--;
			}
			}
		/*End of while *//*Get shift value for pattern value*/
		shift=BitPosition-8;/*Shift pattern value */
		while(shift)
		{
			for(i=5; i<0xFF; i--)
			{
				if((crc[i-1]&0x80) && (i>0)){
				temp=1;
				}
				else{
				temp=0;
				}
				crc[i]<<=1;
				crc[i]+=temp;
			}/*End of for*/
			shift--;
		}/*End of while*//*Exclusive OR between pec and crc*/
		for(i=0; i<=5; i++){
		pec[i] ^=crc[i];
		}/*End of for*/
	}while(BitPosition>8); /*End of do-while*/
	return pec[0];
}/******************************************************************************** Function Name  : SMBus_ReadTemp* Description    : Calculate and return the temperature* Input          : None* Output         : None* Return         : SMBus_ReadMemory(0x00, 0x07)*0.02-273.15
*******************************************************************************/
float SMBus_ReadTemp(void)
{   float temp;
	//temp = SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1);
	temp = SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
	return temp;
}
void SMBus_Init(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
 
		__HAL_RCC_GPIOD_CLK_ENABLE();
	
		GPIO_InitStruct.Pin 		= 6|7;				   // SCL����
		GPIO_InitStruct.Mode 		= GPIO_MODE_OUTPUT_OD;			// ��©���
		GPIO_InitStruct.Pull 		= GPIO_NOPULL;						// ����������
		GPIO_InitStruct.Speed 		= GPIO_SPEED_FREQ_LOW;			// �ٶȵȼ� 
		HAL_GPIO_Init(SMBUS_PORT, &GPIO_InitStruct);
 
//		GPIO_InitStruct.Pin 			= SMBUS_SDA;				// SDA����
//		HAL_GPIO_Init(SMBUS_PORT, &GPIO_InitStruct);		
 
    SMBUS_SCK_H();
    SMBUS_SDA_H();
}


