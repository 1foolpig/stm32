#include "lcd.h"
#include "spi.h"
#include "main.h"
/****************************************************************************
* 名    称：void  SPIv_WriteData(u8 Data)
* 功    能：STM32_模拟SPI写一个字节数据底层函数
* 入口参数：Data
* 出口参数：无
* 说    明：STM32_模拟SPI读写一个字节数据底层函数
****************************************************************************/
//void  SPIv_WriteData(u8 Data)
//{
//	unsigned char i=0;
//	for(i=8;i>0;i--)
//	{
//	  if(Data&0x80)	
//		{
//			LCD_SDA_SET; //????
//		}
//    else 
//		{
//			LCD_SDA_CLR;
//		}	   
//      LCD_SCL_CLR;       
//      LCD_SCL_SET;
//      Data<<=1; 
//	}
//}
uint8_t SPI_WriteByte(uint8_t *TxData,uint16_t size)
{
	return HAL_SPI_Transmit(&hspi2,TxData,size,1000);
}
void  SPIv_WriteData(u8 Data)
{
	SPI_WriteByte(&Data, 1);
}


/****************************************************************************
* 名    称：Lcd_WriteIndex(u8 Index)
* 功    能：向液晶屏写一个8位指令
* 入口参数：Index   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
****************************************************************************/
void Lcd_WriteIndex(u8 Index)
{
   LCD_CS_CLR;
   LCD_RS_CLR;

   SPIv_WriteData(Index);

   LCD_CS_SET;
}

/****************************************************************************
* 名    称：Lcd_WriteData(u8 Data)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
   LCD_CS_CLR;
   LCD_RS_SET;

   SPIv_WriteData(Data);

   LCD_CS_SET;
}

/****************************************************************************
* 名    称：void LCD_WriteReg(u8 Index,u16 Data)
* 功    能：写寄存器数据
* 入口参数：Index,Data
* 出口参数：无
* 说    明：本函数为组合函数，向Index地址的寄存器写入Data值
****************************************************************************/
void LCD_WriteReg(u8 Index,u16 Data)
{
	Lcd_WriteIndex(Index);
  	Lcd_WriteData_16Bit(Data);
}

/****************************************************************************
* 名    称：void Lcd_WriteData_16Bit(u16 Data)
* 功    能：向液晶屏写一个16位数据
* 入口参数：Data
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void Lcd_WriteData_16Bit(u16 Data)
{	
	LCD_CS_CLR;
  LCD_RS_SET;
	Lcd_WriteData(Data>>8);
	Lcd_WriteData(Data);
	LCD_CS_SET;
}

/****************************************************************************
* 名    称：void Lcd_Reset(void)
* 功    能：液晶硬复位函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化前需执行一次复位操作
****************************************************************************/
void Lcd_Reset(void)
{
	LCD_RST_CLR;
	HAL_Delay(100);
	LCD_RST_SET;
	HAL_Delay(50);
}

void Lcd_Init(void)
{	
	
	Lcd_Reset(); //Reset before LCD Init.

	//2.2inch TM2.2-G2.2 Init 20171020 
Lcd_WriteIndex(0xEF);
	Lcd_WriteIndex(0xEB);
	Lcd_WriteData(0x14); 
	
  Lcd_WriteIndex(0xFE);			 
	Lcd_WriteIndex(0xEF); 

	Lcd_WriteIndex(0xEB);	
	Lcd_WriteData(0x14); 

	Lcd_WriteIndex(0x84);			
	Lcd_WriteData(0x40); 

	Lcd_WriteIndex(0x85);			
	Lcd_WriteData(0xFF); 

	Lcd_WriteIndex(0x86);			
	Lcd_WriteData(0xFF); 

	Lcd_WriteIndex(0x87);			
	Lcd_WriteData(0xFF);

	Lcd_WriteIndex(0x88);			
	Lcd_WriteData(0x0A);

	Lcd_WriteIndex(0x89);			
	Lcd_WriteData(0x21); 

	Lcd_WriteIndex(0x8A);			
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0x8B);			
	Lcd_WriteData(0x80); 

	Lcd_WriteIndex(0x8C);			
	Lcd_WriteData(0x01); 

	Lcd_WriteIndex(0x8D);			
	Lcd_WriteData(0x01); 

	Lcd_WriteIndex(0x8E);			
	Lcd_WriteData(0xFF); 

	Lcd_WriteIndex(0x8F);			
	Lcd_WriteData(0xFF); 


	Lcd_WriteIndex(0xB6);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x20);

	Lcd_WriteIndex(0x36);
	Lcd_WriteData(0x08);
	
	Lcd_WriteIndex(0x3A);			
	Lcd_WriteData(0x05); 


	Lcd_WriteIndex(0x90);			
	Lcd_WriteData(0x08);
	Lcd_WriteData(0x08);
	Lcd_WriteData(0x08);
	Lcd_WriteData(0x08); 

	Lcd_WriteIndex(0xBD);			
	Lcd_WriteData(0x06);
	
	Lcd_WriteIndex(0xBC);			
	Lcd_WriteData(0x00);	

	Lcd_WriteIndex(0xFF);			
	Lcd_WriteData(0x60);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x04);

	Lcd_WriteIndex(0xC3);			
	Lcd_WriteData(0x13);
	Lcd_WriteIndex(0xC4);			
	Lcd_WriteData(0x13);

	Lcd_WriteIndex(0xC9);			
	Lcd_WriteData(0x22);

	Lcd_WriteIndex(0xBE);			
	Lcd_WriteData(0x11); 

	Lcd_WriteIndex(0xE1);			
	Lcd_WriteData(0x10);
	Lcd_WriteData(0x0E);

	Lcd_WriteIndex(0xDF);			
	Lcd_WriteData(0x21);
	Lcd_WriteData(0x0c);
	Lcd_WriteData(0x02);

	Lcd_WriteIndex(0xF0);   
	Lcd_WriteData(0x45);
	Lcd_WriteData(0x09);
	Lcd_WriteData(0x08);
	Lcd_WriteData(0x08);
	Lcd_WriteData(0x26);
 	Lcd_WriteData(0x2A);

 	Lcd_WriteIndex(0xF1);    
 	Lcd_WriteData(0x43);
 	Lcd_WriteData(0x70);
 	Lcd_WriteData(0x72);
 	Lcd_WriteData(0x36);
 	Lcd_WriteData(0x37);  
 	Lcd_WriteData(0x6F);


 	Lcd_WriteIndex(0xF2);   
 	Lcd_WriteData(0x45);
 	Lcd_WriteData(0x09);
 	Lcd_WriteData(0x08);
 	Lcd_WriteData(0x08);
 	Lcd_WriteData(0x26);
 	Lcd_WriteData(0x2A);

 	Lcd_WriteIndex(0xF3);   
 	Lcd_WriteData(0x43);
 	Lcd_WriteData(0x70);
 	Lcd_WriteData(0x72);
 	Lcd_WriteData(0x36);
 	Lcd_WriteData(0x37); 
 	Lcd_WriteData(0x6F);

	Lcd_WriteIndex(0xED);	
	Lcd_WriteData(0x1B); 
	Lcd_WriteData(0x0B); 

	Lcd_WriteIndex(0xAE);			
	Lcd_WriteData(0x77);
	
	Lcd_WriteIndex(0xCD);			
	Lcd_WriteData(0x63);		


	Lcd_WriteIndex(0x70);			
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x04);
	Lcd_WriteData(0x0E); 
	Lcd_WriteData(0x0F); 
	Lcd_WriteData(0x09);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x08);
	Lcd_WriteData(0x03);

	Lcd_WriteIndex(0xE8);			
	Lcd_WriteData(0x34);

	Lcd_WriteIndex(0x62);			
	Lcd_WriteData(0x18);
	Lcd_WriteData(0x0D);
	Lcd_WriteData(0x71);
	Lcd_WriteData(0xED);
	Lcd_WriteData(0x70); 
	Lcd_WriteData(0x70);
	Lcd_WriteData(0x18);
	Lcd_WriteData(0x0F);
	Lcd_WriteData(0x71);
	Lcd_WriteData(0xEF);
	Lcd_WriteData(0x70); 
	Lcd_WriteData(0x70);

	Lcd_WriteIndex(0x63);			
	Lcd_WriteData(0x18);
	Lcd_WriteData(0x11);
	Lcd_WriteData(0x71);
	Lcd_WriteData(0xF1);
	Lcd_WriteData(0x70); 
	Lcd_WriteData(0x70);
	Lcd_WriteData(0x18);
	Lcd_WriteData(0x13);
	Lcd_WriteData(0x71);
	Lcd_WriteData(0xF3);
	Lcd_WriteData(0x70); 
	Lcd_WriteData(0x70);

	Lcd_WriteIndex(0x64);			
	Lcd_WriteData(0x28);
	Lcd_WriteData(0x29);
	Lcd_WriteData(0xF1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0xF1);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x07);

	Lcd_WriteIndex(0x66);			
	Lcd_WriteData(0x3C);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0xCD);
	Lcd_WriteData(0x67);
	Lcd_WriteData(0x45);
	Lcd_WriteData(0x45);
	Lcd_WriteData(0x10);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);

	Lcd_WriteIndex(0x67);			
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x3C);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x54);
	Lcd_WriteData(0x10);
	Lcd_WriteData(0x32);
	Lcd_WriteData(0x98);

	Lcd_WriteIndex(0x74);			
	Lcd_WriteData(0x10);	
	Lcd_WriteData(0x85);	
	Lcd_WriteData(0x80);
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x4E);
	Lcd_WriteData(0x00);					
	
    Lcd_WriteIndex(0x98);			
	Lcd_WriteData(0x3e);
	Lcd_WriteData(0x07);

	Lcd_WriteIndex(0x35);	
	Lcd_WriteIndex(0x21);

	Lcd_WriteIndex(0x11); 
	HAL_Delay(120); 

	Lcd_WriteIndex(0x29); 	  	
}



/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(u16 Xpos, u16 Ypos)
{	
	Lcd_WriteIndex(0x2A);
	Lcd_WriteData_16Bit(Xpos);
	Lcd_WriteIndex(0x2B);
	Lcd_WriteData_16Bit(Ypos);
	Lcd_WriteIndex(0x2c);	
} 
/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
//设置显示窗口
void Lcd_SetRegion(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
	Lcd_WriteIndex(0x2A);
	Lcd_WriteData_16Bit(xStar);
	Lcd_WriteData_16Bit(xEnd);
	Lcd_WriteIndex(0x2B);
	Lcd_WriteData_16Bit(yStar);
	Lcd_WriteData_16Bit(yEnd);
	Lcd_WriteIndex(0x2c);
}

	
/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：xy坐标和颜色数据
返回值：无
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
	Lcd_SetXY(x,y);
	Lcd_WriteData_16Bit(Data);

}    

/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(u16 Color)               
{	
   unsigned int i;
   Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
	 LCD_CS_CLR;
   LCD_RS_SET;	
   for(i=0;i<X_MAX_PIXEL*Y_MAX_PIXEL;i++)
   {	
//	  	Lcd_WriteData_16Bit(Color);

		SPIv_WriteData(Color>>8);
		SPIv_WriteData(Color);

   }   
		LCD_CS_SET;
}

