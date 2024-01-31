#include "stm32f4xx.h"                  // Device header
#include "DHT11.h"
#include "gpio.h"
#include "tim.h"

/*
 *TIM3��ʱ��ʵ��us����ʱ
 */
void Delay_us(uint16_t delay)
{
	__HAL_TIM_DISABLE(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3,0);
	__HAL_TIM_ENABLE(&htim3);
	uint16_t curCnt=0;
	while(1)
	{
		curCnt=__HAL_TIM_GET_COUNTER(&htim3);
		if(curCnt>=delay)
			break;
	}
	__HAL_TIM_DISABLE(&htim3);
}


void DHT11_IO_OUT(void) { //��ʪ��ģ���������

    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.Pin = DHT11_IO; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //�������       
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW; //����IO�ӿ��ٶȣ�2/10/50MHz��    
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

void DHT11_IO_IN(void) { //��ʪ��ģ�����뺯��
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.Pin = DHT11_IO; //ѡ��˿ںţ�0~15��all��                        
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //��������      
   HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructure);
}

void DHT11_RST(void) { 						//DHT11�˿ڸ�λ��������ʼ�źţ�IO���ͣ�
    DHT11_IO_OUT();							//�˿�Ϊ���
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_RESET);	//ʹ����Ϊ�͵�ƽ
   HAL_Delay(30);						//��������18ms						
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_3,GPIO_PIN_SET); 		//ʹ����Ϊ�ߵ�ƽ							
    Delay_us(30); 							//��������20~40us
}

uint8_t DHT11_Check(void) { 	//�ȴ�DHT11��Ӧ������1:δ��⵽DHT11������0:�ɹ���IO���գ�	   
    uint8_t retry = 0;			//������ʱ����
    DHT11_IO_IN();		//IO������״̬	 
//GPIO�˿�����ʱ������Ϊ����������߸������룬��Ϊ����������裬����Ĭ��Ϊ�ߵ�ƽ
//���DHT11������������Ϊ�ߵ�ƽ���� retry С��100���� retry ��1������ʱ1΢�룬�ظ��������ֱ�� retry ���ڵ���100 ����DHT11�������������ɵ͵�ƽ����� retry ���ڵ���100����ʾ���ʧ�ܣ�����1�����򣬽� retry ����Ϊ0��
    while ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_IO) == SET) && retry < 100)	//DHT11������40~80us
    {
        retry++;
        Delay_us(1);
    }
    if (retry >= 100)return 1;
    else retry = 0;
    //���DHT11������������Ϊ�͵�ƽ���� retry С��100���� retry ��1������ʱ1΢�룬�ظ��������ֱ�� retry ���ڵ���100 ����DHT11�������������ɸߵ�ƽ����� retry ���ڵ���100����ʾ���ʧ�ܣ�����1�����򣬷���0����ʾ���ɹ���
    while ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_IO) == RESET) && retry < 100)  //DHT11���ͺ���ٴ�����40~80us
    {
        retry++;
        Delay_us(1);
    }
    if (retry >= 100)return 1;
    return 0;
}

uint8_t DHT11_Init(void) {	//DHT11��ʼ��
   __HAL_RCC_GPIOA_CLK_ENABLE();	//��ʼDHT11��ʱ��
    DHT11_RST();								//DHT11�˿ڸ�λ��������ʼ�ź�
    return DHT11_Check(); 						//�ȴ�DHT11��Ӧ
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;
    while ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_IO) == SET) && retry < 100) //�ȴ���Ϊ�͵�ƽ
    {
        retry++;
        Delay_us(1);
    }
    retry = 0;
    while ((HAL_GPIO_ReadPin(DHT11_PORT, DHT11_IO) == RESET) && retry < 100) //�ȴ���ߵ�ƽ
    {
        retry++;
        Delay_us(1);
    }
    Delay_us(40);//�ȴ�40us
    if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_IO) == SET)       //�����жϸߵ͵�ƽ��������1��0
        return 1;
    else
        return 0;
}

//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        dat <<= 1;//���������,dat����1λ
        dat |= DHT11_Read_Bit();	//"|"��ʾ��λ�����
			Delay_us(5);
    }
    return dat;
}

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8_t DHT11_Read_Data(uint8_t* temp, uint8_t* humi)
{
    uint8_t buf[5]={1,1,1,1,1};
    uint8_t i;
    DHT11_RST();						//DHT11�˿ڸ�λ��������ʼ�ź�
    if (DHT11_Check() == 0)				//�ȴ�DHT11��Ӧ��0Ϊ�ɹ���Ӧ
    {
        for (i = 0; i < 5; i++) 			//��ȡ40λ����
        {
            buf[i] = DHT11_Read_Byte();	//��������
        }
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])	//����У��
        {
            *humi = buf[0];				//��ʪ��ֵ����ָ��humi
            *temp = buf[2];				//���¶�ֵ����ָ��temp
        }
    }
    else return 1;
    return 0;
}



