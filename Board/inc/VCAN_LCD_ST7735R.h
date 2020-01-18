/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_LCD_ST7735R.h
 * @brief      LCD 函数库
 * @author     山外科技
 * @version    v6.0
 * @date       2017-11-04
 */

#ifndef _VCAN_LCD_ST7735R_H_
#define _VCAN_LCD_ST7735R_H_

#include "MK66_gpio.h"
#include "enter.h"

//用户配置
#define ST7735R_DIR_DEFAULT     0       //定义默认方向 (0~3)


/********************* ST7735R内部实现 ***********************/
//LCD 的管脚定义
#define     LCD_ST7735R_WR      PTC9
#define     LCD_ST7735R_RD      PTC10
#define     LCD_ST7735R_CS      PTC11
#define     LCD_ST7735R_RS      PTC12
#define     LCD_ST7735R_RST     PTC13

//此部分需要修改初始化里的代码
#define     LCD_ST7735R_P0              PTC_B0_OUT
#define     LCD_ST7735R_PIN             PTC_B0_IN
#define     LCD_ST7735R_PDDR_OUT()      (DDRC_B0 = 0xff)    //DDR[X]_B[N]_OUT   ,X取 A~E ,N取0~3 。例如  DDRD_B3_OUT  表示 PTD 的Byte 3 ，即 PTD24~31
#define     LCD_ST7735R_PDDR_IN()       (DDRC_B0 = 0)


#define ST7735R_DELAY()                         //do{asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");asm("nop");}while(0)
#define ST7735R_DELAY_MS(ms)    DELAY_MS(ms)


extern lcd_t   lcd_st7735r;





#endif  //_VCAN_LCD_ST7735R_H_
