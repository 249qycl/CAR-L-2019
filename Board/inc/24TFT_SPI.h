#ifndef _24TFT_SPI_H_
#define _24TFT_SPI_H_

/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换
#define USE_HORIZONTAL  		0	//定义是否使用横屏 		0,不使用.1,使用.

//-----------------------------SPI 总线配置--------------------------------------//
#define USE_HARDWARE_SPI     0  //1:Enable Hardware SPI;0:USE Soft SPI

//-------------------------屏幕物理像素设置--------------------------------------//
#define LCD_X_SIZE	        240
#define LCD_Y_SIZE	        320

#if USE_HORIZONTAL//如果定义了横屏 
#define X_MAX_PIXEL	        LCD_Y_SIZE
#define Y_MAX_PIXEL	        LCD_X_SIZE
#else
#define X_MAX_PIXEL	        LCD_X_SIZE
#define Y_MAX_PIXEL	        LCD_Y_SIZE

//////////////////////////////////////////////////////////////////////////////////
#define _RED  	 0xf800
#define _GREEN	 0x07e0
#define _BLUE 	 0x001f
#define _WHITE	 0xffff
#define _BLACK	 0x0000
#define _YELLOW  0xFFE0
#define _GRAY0   0xEF7D   	//灰色0 3165 00110 001011 00101
#define _GRAY1   0x8410      	//灰色1      00000 000000 00000
#define _GRAY2   0x4208      	//灰色2  1111111111011111


//本测试程序使用的是模拟SPI接口驱动
//可自由更改接口IO配置，使用任意最少4 IO即可完成本款液晶驱动显示

//液晶控制口置1操作语句宏定义
//#define	LCD_CS_SET  	gpio_set(F3,1)  
#define	LCD_RST_SET  	gpio_set(PTD0,1)  
#define	LCD_RS_SET  	gpio_set(PTD1,1)  
#define	LCD_SDA_SET  	gpio_set(PTD3,1)  //连接至TFT - SDI  
#define	LCD_SCL_SET  	gpio_set(PTD5,1)  //连接至TFT -- CLK    
//#define	LCD_LED_SET  	gpio_set(F0,1)  //此线必须连接，否则屏幕不能被使能，无法工作

//液晶控制口置0操作语句宏定义
//#define	LCD_CS_CLR  	gpio_set(F3,0) 
#define	LCD_RST_CLR  	gpio_set(PTD0,0) 
#define	LCD_RS_CLR  	gpio_set(PTD1,0)    
#define	LCD_SDA_CLR  	gpio_set(PTD3,0)     
#define	LCD_SCL_CLR  	gpio_set(PTD5,0)     
//#define	LCD_LED_CLR  	gpio_set(F0,0)
/*
#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //数据输出
#define LCD_DATAIN     LCD_DATA->IDR;   //数据输入
*/
/*
#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 
*/
void  SPIv_WriteData(uint8 Data);
void LCD24_GPIO_Init(void);
void Lcd24_WriteIndex(uint8 Index);
void Lcd24_WriteData(uint8 Data);
void Lcd24_WriteReg(uint8 Index,uint8 Data);
uint16 Lcd24_ReadReg(uint8 LCD_Reg);
void Lcd24_Reset(void);
void Lcd24_Init(void);
void Lcd24_Clear(uint16 Color);
void Lcd24_SetXY(uint16 x,uint16 y);
void Gui_DrawPoint(uint16 x,uint16 y,uint16 Data);
void DrawRectangle(uint16 x,uint16 y,uint16 W,uint16 H,uint16 Data);
unsigned int Lcd24_ReadPoint(uint16 x,uint16 y);
void Lcd24_SetRegion(uint16 xStar, uint16 yStar,uint16 xEnd,uint16 yEnd);
void Lcd24_WriteData_16Bit(uint16 Data);

uint16 LCD_BGR2RGB(uint16 c);
void Gui_Circle(uint16 X,uint16 Y,uint16 R,uint16 fc); 
void Gui_DrawLine(uint16 x0, uint16 y0,uint16 x1, uint16 y1,uint16 Color);  
void Gui_box(uint16 x, uint16 y, uint16 w, uint16 h,uint16 bc);
void Gui_box2(uint16 x,uint16 y,uint16 w,uint16 h, uint8 mode);
void DisplayButtonDown(uint16 x1,uint16 y1,uint16 x2,uint16 y2);
void DisplayButtonUp(uint16 x1,uint16 y1,uint16 x2,uint16 y2);
void Gui_DrawFont_GBK16(uint16 x, uint16 y, uint16 fc, uint16 bc, uint8 *s);
void Gui_DrawFont_GBK24(uint16 x, uint16 y, uint16 fc, uint16 bc, uint8 *s);
void Gui_DrawFont_Num32(uint16 x, uint16 y, uint16 fc, uint16 bc, uint16 num) ;


#endif
#endif
