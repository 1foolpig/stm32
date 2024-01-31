
#define __MLX90614_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SMBus_StartBit(void);
void SMBus_StopBit(void);
void SMBus_SendBit(uint8_t);
void SMBus_Init(void);
uint8_t SMBus_SendByte(uint8_t);
uint8_t SMBus_ReceiveBit(void);
uint8_t SMBus_ReceiveByte(uint8_t);
void SMBus_Delay(uint16_t);
uint16_t SMBus_ReadMemory(uint8_t, uint8_t);
uint8_t PEC_Calculation(uint8_t*);
float SMBus_ReadTemp(void); 


