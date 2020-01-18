#include "common.h"
#include "include.h"
#include "24TFT_Font.h"
/***************************************************************************************
STM32测试平台介绍:
开发板：正点原子MiniSTM32开发板
MCU ：STM32_F103_RBT6
晶振 ：12MHZ
主频 ：72MHZ
接线说明:
//-------------------------------------------------------------------------------------
#define LCD_CTRL   	  	GPIOB		//定义TFT数据端口
#define LCD_LED        	GPIO_Pin_9  //PB9 连接至TFT -LED
#define LCD_RS         	GPIO_Pin_10	//PB10连接至TFT --RS
#define LCD_CS        	GPIO_Pin_11 //PB11 连接至TFT --CS
#define LCD_RST     	  GPIO_Pin_12	//PB12连接至TFT --RST
#define LCD_SCL        	GPIO_Pin_13	//PB13连接至TFT -- CLK
#define LCD_SDA        	GPIO_Pin_15	//PB15连接至TFT - SDI
//VCC:可以接5V也可以接3.3V
//LED:可以接5V也可以接3.3V或者使用任意空闲IO控制(高电平使能)
//GND：接电源地
//说明：如需要尽可能少占用IO，可以将LCD_CS接地，LCD_LED接3.3V，LCD_RST接至单片机复位端，
//将可以释放3个可用IO
//接口定义在Lcd_Driver.h内定义，
//如需变更IO接法，请根据您的实际接线修改相应IO初始化LCD_GPIO_Init()
//-----------------------------------------------------------------------------------------
例程功能说明：
1.	简单刷屏测试
2.	英文显示测试示例
3.	中文显示测试示例
4.	数码管字体显示示例
5.	图片显示示例
6.	2D按键菜单示例
7.	本例程支持横屏/竖屏切换(开启宏USE_HORIZONTAL,详见Lcd_Driver.h)
8.	本例程支持软件模拟SPI/硬件SPI切换(开启宏USE_HARDWARE_SPI,详见Lcd_Driver.h)


ILI9341屏幕大小：320x240

**********************************************************************************************/


//---------------------------------function----------------------------------------------------//

/****************************************************************************
* 名    称：void LCD_GPIO_Init(void)
* 功    能：STM32_模拟SPI所用到的GPIO初始化
* 入口参数：无
* 出口参数：无
* 说    明：初始化模拟SPI所用的GPIO
****************************************************************************/
void LCD24_GPIO_Init(void)
{
  
gpio_init(PTD0, GPO, 0);
gpio_init(PTD1, GPO, 0);
gpio_init(PTD3, GPO, 0);
gpio_init(PTD5, GPO, 0);  

//  spi_init(SPI0,SPIn_PCS0, MASTER,10*1000*1000);
}
/****************************************************************************
* 名    称：void  SPIv_WriteData(u8 Data)
* 功    能：STM32_模拟SPI写一个字节数据底层函数
* 入口参数：Data
* 出口参数：无
* 说    明：STM32_模拟SPI读写一个字节数据底层函数
****************************************************************************/
void  SPIv_WriteData(uint8 Data)
{
  
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data&0x80)	
                    gpio_set(PTD3,1); //输出数据
                else 
                    gpio_set(PTD3,0);	   
      gpio_set(PTD5,0);       
      gpio_set(PTD5,1);
      Data<<=1; 
	}

//  spi_init(SPI0,SPIn_PCS0, MASTER,10*1000*1000);
}

/****************************************************************************
* 名    称：Lcd_WriteIndex(u8 Index)
* 功    能：向液晶屏写一个8位指令
* 入口参数：Index   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
****************************************************************************/
void Lcd24_WriteIndex(uint8 Index)
{
   //LCD_CS_CLR;
   gpio_set(PTD1,0);
   SPIv_WriteData(Index);
   //LCD_CS_SET;
}

/****************************************************************************
* 名    称：Lcd_WriteData(u8 Data)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void Lcd24_WriteData(uint8 Data)
{
   //LCD_CS_CLR;
   gpio_set(PTD1,1);
   SPIv_WriteData(Data);
   //LCD_CS_SET;
}

/****************************************************************************
* 名    称：void LCD_WriteReg(u8 Index,u16 Data)
* 功    能：写寄存器数据
* 入口参数：Index,Data
* 出口参数：无
* 说    明：本函数为组合函数，向Index地址的寄存器写入Data值
****************************************************************************/
void LCD24_WriteReg(uint8 Index,uint16 Data)
{
	Lcd24_WriteIndex(Index);
  	Lcd24_WriteData_16Bit(Data);
}

/****************************************************************************
* 名    称：void Lcd_WriteData_16Bit(u16 Data)
* 功    能：向液晶屏写一个16位数据
* 入口参数：Data
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void Lcd24_WriteData_16Bit(uint16 Data)
{	
	Lcd24_WriteData(Data>>8);
	Lcd24_WriteData(Data);	
}

/****************************************************************************
* 名    称：void Lcd_Reset(void)
* 功    能：液晶硬复位函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化前需执行一次复位操作
****************************************************************************/
void Lcd24_Reset(void)
{
	LCD_RST_CLR;
	DELAY_MS(100);//延时100ms
	LCD_RST_SET;
	DELAY_MS(50);
}
/****************************************************************************
* 名    称：void Lcd_Init(void)
* 功    能：液晶初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化_ILI9225_176X220
****************************************************************************/
void Lcd24_Init(void)
{	

	LCD24_GPIO_Init();
	Lcd24_Reset(); //Reset before LCD Init.

	//2.2inch TM2.2-G2.2 Init 20171020 
	Lcd24_WriteIndex(0x11);  
	Lcd24_WriteData(0x00); 

	Lcd24_WriteIndex(0xCF);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0XC1); 
	Lcd24_WriteData(0X30);

	Lcd24_WriteIndex(0xED);  
	Lcd24_WriteData(0X64); 
	Lcd24_WriteData(0X03); 
	Lcd24_WriteData(0X12);
	Lcd24_WriteData(0X81);

	Lcd24_WriteIndex(0xE8);  
	Lcd24_WriteData(0X85); 
	Lcd24_WriteData(0X11); 
	Lcd24_WriteData(0X78);

	Lcd24_WriteIndex(0xF6);  
	Lcd24_WriteData(0X01); 
	Lcd24_WriteData(0X30); 
	Lcd24_WriteData(0X00);

	Lcd24_WriteIndex(0xCB);  
	Lcd24_WriteData(0X39); 
	Lcd24_WriteData(0X2C); 
	Lcd24_WriteData(0X00);
	Lcd24_WriteData(0X34);
	Lcd24_WriteData(0X05);
           
	Lcd24_WriteIndex(0xF7);  
	Lcd24_WriteData(0X20); 
           
	Lcd24_WriteIndex(0xEA);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X00); 
           
	Lcd24_WriteIndex(0xC0);  
	Lcd24_WriteData(0X20); 
           
	Lcd24_WriteIndex(0xC1);  
	Lcd24_WriteData(0X11); 
           
	Lcd24_WriteIndex(0xC5);  
	Lcd24_WriteData(0X31); 
	Lcd24_WriteData(0X3C); 
           
	Lcd24_WriteIndex(0xC7);  
	Lcd24_WriteData(0XA9); 
           
	Lcd24_WriteIndex(0x3A);  
	Lcd24_WriteData(0X55); 
	   
        Lcd24_WriteIndex(0x36);  
	#if USE_HORIZONTAL
		 Lcd24_WriteData(0xE8);//横屏参数
	#else
		 Lcd24_WriteData(0x48);//竖屏参数 
	#endif

	Lcd24_WriteIndex(0xB1);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X18); 

	Lcd24_WriteIndex(0xB4);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X00); 

	Lcd24_WriteIndex(0xF2);  
	Lcd24_WriteData(0X00); 

	Lcd24_WriteIndex(0x26);  
	Lcd24_WriteData(0X01); 

	Lcd24_WriteIndex(0xE0);  
	Lcd24_WriteData(0X0F); 
	Lcd24_WriteData(0X17); 
	Lcd24_WriteData(0X14); 
	Lcd24_WriteData(0X09); 
	Lcd24_WriteData(0X0C); 
	Lcd24_WriteData(0X06); 
	Lcd24_WriteData(0X43); 
	Lcd24_WriteData(0X75); 
	Lcd24_WriteData(0X36); 
	Lcd24_WriteData(0X08); 
	Lcd24_WriteData(0X13); 
	Lcd24_WriteData(0X05); 
	Lcd24_WriteData(0X10); 
	Lcd24_WriteData(0X0B); 
	Lcd24_WriteData(0X08); 


	Lcd24_WriteIndex(0xE1);  
	Lcd24_WriteData(0X00); 
	Lcd24_WriteData(0X1F); 
	Lcd24_WriteData(0X23); 
	Lcd24_WriteData(0X03); 
	Lcd24_WriteData(0X0E); 
	Lcd24_WriteData(0X04); 
	Lcd24_WriteData(0X39); 
	Lcd24_WriteData(0X25); 
	Lcd24_WriteData(0X4D); 
	Lcd24_WriteData(0X06); 
	Lcd24_WriteData(0X0D); 
	Lcd24_WriteData(0X0B); 
	Lcd24_WriteData(0X33); 
	Lcd24_WriteData(0X37); 
	Lcd24_WriteData(0X0F); 

	Lcd24_WriteIndex(0x29); 
        Lcd24_Clear(_WHITE);
}



/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd24_SetXY(uint16 Xpos, uint16 Ypos)
{	
	Lcd24_WriteIndex(0x2A);
	Lcd24_WriteData_16Bit(Xpos);
	Lcd24_WriteIndex(0x2B);
	Lcd24_WriteData_16Bit(Ypos);
	Lcd24_WriteIndex(0x2c);	
} 
/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
//设置显示窗口
void Lcd24_SetRegion(uint16 xStar, uint16 yStar,uint16 xEnd,uint16 yEnd)
{
	Lcd24_WriteIndex(0x2A);
	Lcd24_WriteData_16Bit(xStar);
	Lcd24_WriteData_16Bit(xEnd);
	Lcd24_WriteIndex(0x2B);
	Lcd24_WriteData_16Bit(yStar);
	Lcd24_WriteData_16Bit(yEnd);
	Lcd24_WriteIndex(0x2c);
}

	
/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：xy坐标和颜色数据
返回值：无
*************************************************/
void Gui_DrawPoint(uint16 x,uint16 y,uint16 Data)
{
	Lcd24_SetXY(x,y);
	Lcd24_WriteData_16Bit(Data);

}    

/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：xy坐标和颜色数据
返回值：无
*************************************************/
void DrawRectangle(uint16 x,uint16 y,uint16 W,uint16 H,uint16 Data)
{
  uint16 i,j;
  for(i=0;i<H;i++)
  {
    for(j=0;j<W;j++)
    {
      Gui_DrawPoint(x+j,y+i,Data);
    }
  }  
}    

/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd24_Clear(uint16 Color)               
{	
   unsigned int i;
   Lcd24_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
	 //LCD_CS_CLR;
   LCD_RS_SET;	
   for(i=0;i<X_MAX_PIXEL*Y_MAX_PIXEL;i++)
   {	
	  	//Lcd_WriteData_16Bit(Color);
		SPIv_WriteData(Color>>8);
		SPIv_WriteData(Color); 
   }   
		//LCD_CS_SET;
}



//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16 LCD_BGR2RGB(uint16 c)
{
  uint16  r,g,b,rgb;   
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;	 
  rgb=(b<<11)+(g<<5)+(r<<0);		 
  return(rgb);

}




void Gui_Circle(uint16 X,uint16 Y,uint16 R,uint16 fc) //绘制一个圆  fc为颜色数据
{//Bresenham算法 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc);     //        7 
        Gui_DrawPoint(X-a,Y+b,fc);     //        6 
        Gui_DrawPoint(X+a,Y-b,fc);     //        2 
        Gui_DrawPoint(X-a,Y-b,fc);     //        3 
        Gui_DrawPoint(X+b,Y+a,fc);     //        8 
        Gui_DrawPoint(X-b,Y+a,fc);     //        5 
        Gui_DrawPoint(X+b,Y-a,fc);     //        1 
        Gui_DrawPoint(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y+b,fc); 
        Gui_DrawPoint(X+a,Y-b,fc); 
        Gui_DrawPoint(X-a,Y-b,fc); 
        Gui_DrawPoint(X+b,Y+a,fc); 
        Gui_DrawPoint(X-b,Y+a,fc); 
        Gui_DrawPoint(X+b,Y-a,fc); 
        Gui_DrawPoint(X-b,Y-a,fc); 
    } 
	
} 
//画线函数，使用Bresenham 画线算法
void Gui_DrawLine(uint16 x0, uint16 y0,uint16 x1, uint16 y1,uint16 Color) //画斜线  
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	Lcd24_SetXY(x0,y0);
	dx = x1-x0;//计算x距离
	dy = y1-y0;//计算y距离

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			Gui_DrawPoint(x0,y0,Color);
			
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//增加y坐标值
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} // end for
	} // end if |slope| <= 1
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			Gui_DrawPoint(x0,y0,Color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}



void Gui_box(uint16 x, uint16 y, uint16 w, uint16 h,uint16 bc)//bc背景颜色
{
	Gui_DrawLine(x,y,x+w,y,0xEF7D);
	Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
	Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
	Gui_DrawLine(x,y,x,y+h,0xEF7D);
        Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}
void Gui_box2(uint16 x,uint16 y,uint16 w,uint16 h, uint8 mode)
{
	if (mode==0)	{
		Gui_DrawLine(x,y,x+w,y,0xEF7D);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
		Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
		Gui_DrawLine(x,y,x,y+h,0xEF7D);
		}
	if (mode==1)	{
		Gui_DrawLine(x,y,x+w,y,0x2965);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
		Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
		Gui_DrawLine(x,y,x,y+h,0x2965);
	}
	if (mode==2)	{
		Gui_DrawLine(x,y,x+w,y,0xffff);
		Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
		Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
		Gui_DrawLine(x,y,x,y+h,0xffff);
	}
}


/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: uint16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonDown(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, _GRAY2);  //H
	Gui_DrawLine(x1+1,y1+1,x2,y1+1, _GRAY1);  //H
	Gui_DrawLine(x1,  y1,  x1,y2, _GRAY2);  //V
	Gui_DrawLine(x1+1,y1+1,x1+1,y2, _GRAY1);  //V
	Gui_DrawLine(x1,  y2,  x2,y2, _WHITE);  //H
	Gui_DrawLine(x2,  y1,  x2,y2, _WHITE);  //V
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: uint16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(uint16 x1,uint16 y1,uint16 x2,uint16 y2)
{
	Gui_DrawLine(x1,  y1,  x2,y1, _WHITE); //H
	Gui_DrawLine(x1,  y1,  x1,y2, _WHITE); //V
	
	Gui_DrawLine(x1+1,y2-1,x2,y2-1, _GRAY1);  //H
	Gui_DrawLine(x1,  y2,  x2,y2, _GRAY2);  //H
	Gui_DrawLine(x2-1,y1+1,x2-1,y2, _GRAY1);  //V
    Gui_DrawLine(x2  ,y1  ,x2,y2, _GRAY2); //V
}

/*显示字体为  16  的中文 英文 数字 字符*/
void Gui_DrawFont_GBK16(uint16 x, uint16 y, uint16 fc, uint16 bc, uint8 *s)  //fc字体颜色  bc背景颜色
{
	unsigned char i,j;
	unsigned short k,x0;
	x0=x;

	while(*s) 
	{	
		if((*s) < 128) 
		{
			k=*s;
			if (k==13) 
			{
				x=x0;
				y+=16;
			}
			else 
			{
				if (k>32) k-=32; else k=0;
	
			    for(i=0;i<16;i++)
				for(j=0;j<8;j++) 
					{
				    	if(asc16[k*16+i]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				x+=8;
			}
			s++;
		}
			
		else 
		{
		

			for (k=0;k<hz16_num;k++) 
			{
			  if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
			  { 
				    for(i=0;i<16;i++)
				    {
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2]&(0x80>>j))	Gui_DrawPoint(x+j,y+i,fc);
								else {
									if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
								}
							}
						for(j=0;j<8;j++) 
							{
						    	if(hz16[k].Msk[i*2+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
								else 
								{
									if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
								}
							}
				    }
				}
			  }
			s+=2;x+=16;
		} 
		
	}
}

void Gui_DrawFont_GBK24(uint16 x, uint16 y, uint16 fc, uint16 bc, uint8 *s)//fc字体颜色  bc背景颜色
{
unsigned char i,j;
unsigned short k;

while(*s) 
{
if( *s < 0x80 ) 
{
	k=*s;
	if (k>32) k-=32; else k=0;

        for(i=0;i<16;i++)
	for(j=0;j<8;j++) 
		{
	    	if(asc16[k*16+i]&(0x80>>j))	
			Gui_DrawPoint(x+j,y+i,fc);
			else 
			{
				if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
			}
		}
	s++;x+=8;
}
else 
{

	for (k=0;k<hz24_num;k++) 
	{
	  if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
	  { 
		    for(i=0;i<24;i++)
		    {
				for(j=0;j<8;j++) 
					{
				    	if(hz24[k].Msk[i*3]&(0x80>>j))
						Gui_DrawPoint(x+j,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
						}
					}
				for(j=0;j<8;j++) 
					{
				    	if(hz24[k].Msk[i*3+1]&(0x80>>j))	Gui_DrawPoint(x+j+8,y+i,fc);
						else {
							if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
						}
					}
				for(j=0;j<8;j++) 
					{
				    	if(hz24[k].Msk[i*3+2]&(0x80>>j))	
						Gui_DrawPoint(x+j+16,y+i,fc);
						else 
						{
							if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
						}
					}
		    }
	  }
	}
	s+=2;x+=24;
}
}
}
void Gui_DrawFont_Num32(uint16 x, uint16 y, uint16 fc, uint16 bc, uint16 num)//fc字体颜色  bc背景颜色
{
	unsigned char i,j,k,c;
	//lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//	w=w/8;

    for(i=0;i<32;i++)
	{
		for(j=0;j<4;j++) 
		{
			c=*(sz32+num*32*4+i*4+j);
			for (k=0;k<8;k++)	
			{
	
		    	if(c&(0x80>>k))	Gui_DrawPoint(x+j*8+k,y+i,fc);
				else {
					if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
				}
			}
		}
	}
}


