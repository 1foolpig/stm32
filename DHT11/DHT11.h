#include "stm32f4xx.h"
#include "gpio.h"

#ifndef _DHT11_H
#define _DHT11_H
#define  DHT11_RCC RCC_APB2Periph_GPIOA //开启引脚RCC
#define DHT11_PORT GPIOA //定义端口
#define DHT11_IO GPIO_PIN_3

void Delay_us(uint16_t delay);
void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);
void DHT11_RST(void);
uint8_t DHT11_Check(void);
uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Bit(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_Read_Data(uint8_t* temp, uint8_t* humi);

#endif 

