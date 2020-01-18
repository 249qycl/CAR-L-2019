/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2016,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       VCAN_LCD_ILI9325.c
 * @brief      LCD ILI9325函数库
 * @author     山外科技
 * @version    v6.0
 * @date       2017-11-04
 */

#include "common.h"
#include "MK66_gpio.h"
#include "VCAN_LCD_ILI9325.h"



//定义所用到的 IO管脚
#define LCD_RST    PTC13                //复位
#define LCD_BL     PTB8                 //背光


#define ILI9325_DELAY()          DELAY_MS(100)
#define ILI9325_DELAYMS(ms)      DELAY_MS(ms)


#define ILI9325_H       240
#define ILI9325_W       320

#if ((ILI9325_DIR_DEFAULT&1 )== 0)      //横屏
uint16  ili9325_h   = ILI9325_H;
uint16  ili9325_w   = ILI9325_W;
#else
uint16  ili9325_h   = ILI9325_W;
uint16  ili9325_w   = ILI9325_H;
#endif
uint8   ili9325_dir = ILI9325_DIR_DEFAULT;

extern void    LCD_ILI9325_init(Size_t *size) ;
extern void     LCD_ILI9325_dir(uint8 option);
extern void     LCD_ILI9325_ptlon(Site_t site, Size_t size);
extern uint16   ILI9325_get_h();
extern uint16   ILI9325_get_w();
extern uint8    ILI9325_get_dir();

/********************* ILI9325内部实现 ***********************/

#define LCD_ILI9325_CMD_ADDR                    FB_8080_CMD
#define LCD_ILI9325_DATA_ADDR                   FB_8080_DATA

#define LCD_SET_DIR(opt)        LCD_ILI9325_dir(opt)            //方向

#define LCD_ILI9325_WR_CMD(cmd)         do{LCD_ILI9325_CMD_ADDR  = cmd ;}while(0)
#define LCD_ILI9325_WR_DATA(data)       do{LCD_ILI9325_DATA_ADDR = data;}while(0)

void LCD_ILI9325_ramwr(void)
{
    LCD_ILI9325_WR_CMD(0x22) ;       //写模式
}

void LCD_ILI9325_wr_data(uint16_t data)
{
    LCD_ILI9325_WR_DATA(data) ;
}


lcd_t   lcd_ili9325 ={
    .lcd_w  =  0,
    .lcd_h  =  0,
    .init   =  LCD_ILI9325_init,
    .ptlon  =  LCD_ILI9325_ptlon,
    .ramwr  =  LCD_ILI9325_ramwr,
    .wr_data=  LCD_ILI9325_wr_data
};
extern void    LCD_ILI9325_org_init();



void    LCD_ILI9325_init(Size_t *size)
{
    LCD_ILI9325_org_init() ;

    //配置分辨率
    if((ili9325_dir &1 )== 0)        //横屏
    {
        size->W = ILI9325_W;
        size->H = ILI9325_H;
    }
    else
    {
        size->W = ILI9325_H;
        size->H = ILI9325_W;
    }

}
/*!
 *  @brief      LCD_ILI9325初始化
 *  @since      v5.0
 */
void    LCD_ILI9325_org_init()
{
    gpio_init (LCD_BL, GPO, 0); //LCD背光管脚输出1，表示关闭LCD背光

    //复位LCD
    gpio_init (LCD_RST, GPO, 0);

    ILI9325_DELAYMS(1);
    GPIO_SET   (LCD_RST, 1);

    //初始化总线
    flexbus_8080_init();

    /* Start Initial Sequence ----------------------------------------------------*/
    LCD_ILI9325_WR_CMD(0xE7);
    LCD_ILI9325_WR_DATA(0x0010); /* Set the internal vcore voltage */
    LCD_ILI9325_WR_CMD(0);
    LCD_ILI9325_WR_DATA(  0x0001); /* Start internal OSC. */
    LCD_ILI9325_WR_CMD(1);
    LCD_ILI9325_WR_DATA(  0x0100); /* set SS and SM bit */
    LCD_ILI9325_WR_CMD(2);
    LCD_ILI9325_WR_DATA(  0x0700); /* set 1 line inversion */
    LCD_ILI9325_WR_CMD(3);
    LCD_ILI9325_WR_DATA(  (1 << 12) | (1 << 5) | (1 << 4) | (1 << 0) ); /* set GRAM write direction and BGR=1. */


    LCD_ILI9325_WR_CMD(4);
    LCD_ILI9325_WR_DATA(  0x0000); /* Resize register */
    LCD_ILI9325_WR_CMD(8);
    LCD_ILI9325_WR_DATA(  0x0207); /* set the back porch and front porch */
    LCD_ILI9325_WR_CMD(9);
    LCD_ILI9325_WR_DATA(  0x0000); /* set non-display area refresh cycle ISC[3:0] */
    LCD_ILI9325_WR_CMD(0xa);
    LCD_ILI9325_WR_DATA( 0x0000); /* FMARK function */
    LCD_ILI9325_WR_CMD(0xc);
    LCD_ILI9325_WR_DATA( 0x0001); /* RGB interface setting */
    LCD_ILI9325_WR_CMD(13);
    LCD_ILI9325_WR_DATA( 0x0000); /* Frame marker Position */
    LCD_ILI9325_WR_CMD(15);
    LCD_ILI9325_WR_DATA( 0x0000); /* RGB interface polarity */
    /* Power On sequence ---------------------------------------------------------*/
    LCD_ILI9325_WR_CMD(16);
    LCD_ILI9325_WR_DATA( 0x0000); /* SAP,BT[3:0],AP,DSTB,SLP, STB */
    LCD_ILI9325_WR_CMD(17);
    LCD_ILI9325_WR_DATA( 0x0007); /* DC1[2:0], DC0[2:0], VC[2:0] */
    LCD_ILI9325_WR_CMD(18);
    LCD_ILI9325_WR_DATA( 0x0000); /* VREG1OUT voltage */
    LCD_ILI9325_WR_CMD(19);
    LCD_ILI9325_WR_DATA( 0x0000); /* VDV[4:0] for VCOM amplitude */
    ILI9325_DELAY() ;                 /* Dis-charge capacitor power voltage  */
    LCD_ILI9325_WR_CMD(16);
    LCD_ILI9325_WR_DATA( 0x1590); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
    LCD_ILI9325_WR_CMD(17);
    LCD_ILI9325_WR_DATA( 0x0227); /* DC1[2:0], DC0[2:0], VC[2:0] */
    ILI9325_DELAY() ;                  /* Delay 50 ms */
    LCD_ILI9325_WR_CMD(18);
    LCD_ILI9325_WR_DATA( 0x009c); /* VREG1OUT voltage */
    ILI9325_DELAY() ;                  /* Delay 50 ms */
    LCD_ILI9325_WR_CMD(19);
    LCD_ILI9325_WR_DATA( 0x1900); /* VDV[4:0] for VCOM amplitude */

    LCD_ILI9325_WR_CMD(41);
    LCD_ILI9325_WR_DATA( 0x0023); /* VCM[4:0] for VCOMH */
    ILI9325_DELAY() ;                  /* Delay 50 ms */
    LCD_ILI9325_WR_CMD(32);
    LCD_ILI9325_WR_DATA( 0x0000); /* GRAM horizontal Address */
    LCD_ILI9325_WR_CMD(33);
    LCD_ILI9325_WR_DATA( 0x0000); /* GRAM Vertical Address */
    /* Adjust the Gamma Curve ----------------------------------------------------*/
    LCD_ILI9325_WR_CMD(48);
    LCD_ILI9325_WR_DATA( 0x0007);
    LCD_ILI9325_WR_CMD(49);
    LCD_ILI9325_WR_DATA( 0x0707);
    LCD_ILI9325_WR_CMD(50);
    LCD_ILI9325_WR_DATA( 0x0006);
    LCD_ILI9325_WR_CMD(53);
    LCD_ILI9325_WR_DATA( 0x0704);
    LCD_ILI9325_WR_CMD(54);
    LCD_ILI9325_WR_DATA( 0x1f04);
    LCD_ILI9325_WR_CMD(55);
    LCD_ILI9325_WR_DATA( 0x0004);
    LCD_ILI9325_WR_CMD(56);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(57);
    LCD_ILI9325_WR_DATA( 0x0706);
    LCD_ILI9325_WR_CMD(60);
    LCD_ILI9325_WR_DATA( 0x0701);
    LCD_ILI9325_WR_CMD(61);
    LCD_ILI9325_WR_DATA( 0x000f);
    ILI9325_DELAY() ;

    LCD_ILI9325_WR_CMD(0x0030);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(0x0031);
    LCD_ILI9325_WR_DATA( 0x0603);
    LCD_ILI9325_WR_CMD(0x0032);
    LCD_ILI9325_WR_DATA( 0x0206);
    LCD_ILI9325_WR_CMD(0x0035);
    LCD_ILI9325_WR_DATA( 0x0206);
    LCD_ILI9325_WR_CMD(0x0036);
    LCD_ILI9325_WR_DATA( 0x0004);
    LCD_ILI9325_WR_CMD(0x0037);
    LCD_ILI9325_WR_DATA( 0x0105);
    LCD_ILI9325_WR_CMD(0x0038);
    LCD_ILI9325_WR_DATA( 0x0401);
    LCD_ILI9325_WR_CMD(0x0039);
    LCD_ILI9325_WR_DATA( 0x0707);
    LCD_ILI9325_WR_CMD(0x003C);
    LCD_ILI9325_WR_DATA( 0x0602);
    LCD_ILI9325_WR_CMD(0x003D);
    LCD_ILI9325_WR_DATA( 0x0004);

    /* Set GRAM area -------------------------------------------------------------*/
    LCD_ILI9325_WR_CMD(80);
    LCD_ILI9325_WR_DATA( 0x0000); /* Horizontal GRAM Start Address */
    LCD_ILI9325_WR_CMD(81);
    LCD_ILI9325_WR_DATA( 0x00EF); /* Horizontal GRAM End Address */
    LCD_ILI9325_WR_CMD(82);
    LCD_ILI9325_WR_DATA( 0x0000); /* Vertical GRAM Start Address */
    LCD_ILI9325_WR_CMD(83);
    LCD_ILI9325_WR_DATA( 0x013F); /* Vertical GRAM End Address */
    LCD_ILI9325_WR_CMD(96);
    LCD_ILI9325_WR_DATA(  0xA700); /* Gate Scan Line */
    LCD_ILI9325_WR_CMD(97);
    LCD_ILI9325_WR_DATA(  0x0001); /* NDL,VLE,REV */
    LCD_ILI9325_WR_CMD(106);
    LCD_ILI9325_WR_DATA( 0x0000); /* set scrolling line */
    /* Partial Display Control ---------------------------------------------------*/
    LCD_ILI9325_WR_CMD(128);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(129);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(130);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(131);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(132);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(133);
    LCD_ILI9325_WR_DATA( 0x0000);
    /* Panel Control -------------------------------------------------------------*/
    LCD_ILI9325_WR_CMD(144);
    LCD_ILI9325_WR_DATA( 0x0010);
    LCD_ILI9325_WR_CMD(146);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(147);
    LCD_ILI9325_WR_DATA( 0x0003);
    LCD_ILI9325_WR_CMD(149);
    LCD_ILI9325_WR_DATA( 0x0110);
    LCD_ILI9325_WR_CMD(151);
    LCD_ILI9325_WR_DATA( 0x0000);
    LCD_ILI9325_WR_CMD(152);
    LCD_ILI9325_WR_DATA( 0x0000);
    /* Set GRAM write direction and BGR = 1 */
    /* I/D=01 (Horizontal : increment,Vertical : decrement) */
    /* AM=1 (address is updated in vertical writing direction) */
    LCD_ILI9325_WR_CMD(43);
    LCD_ILI9325_WR_DATA( 0x000D);
    LCD_ILI9325_WR_CMD(7);
    LCD_ILI9325_WR_DATA( 0x0173); /* 262K color and display ON */

    LCD_ILI9325_WR_CMD(7);
    LCD_ILI9325_WR_DATA( 0x0133);


    LCD_ILI9325_WR_CMD(0x20);
    LCD_ILI9325_WR_DATA( 0);

    LCD_ILI9325_WR_CMD(0x21);
    LCD_ILI9325_WR_DATA( 319);

    LCD_ILI9325_dir(ili9325_dir) ;

    PTXn_T(LCD_BL, OUT) = 1;    //开LCD背光
}

/*!
 *  @brief      设置ILI9325GRAM指针扫描方向
 *  @param      option    方向选择（0~3）
 *  @since      v5.0
 */
void LCD_ILI9325_dir(uint8 option)
{
    option = option % 4;


    ili9325_dir = option;


    switch(option)
    {
        case 0:
        {
            /*横屏*/

            ili9325_h   = ILI9325_H;
            ili9325_w   = ILI9325_W;
            LCD_ILI9325_WR_CMD(3);
            LCD_ILI9325_WR_DATA(  (1 << 12) | (0 << 9) | (1 << 7) | (0 << 5) | (1 << 4) | (1 << 3));
        }
        break;

        case 1:
        {
            ili9325_h   = ILI9325_W;
            ili9325_w   = ILI9325_H;

            /*竖屏*/
            LCD_ILI9325_WR_CMD(3);
            LCD_ILI9325_WR_DATA(  (1 << 12) | (0 << 9) | (1 << 7) | (1 << 5) | (1 << 4) | (0 << 3)); /* set GRAM write direction and BGR=1. */


        }
        break;

        case 2:
        {
            /*横屏*/
            ili9325_h   = ILI9325_H;
            ili9325_w   = ILI9325_W;
            LCD_ILI9325_WR_CMD(3);
            LCD_ILI9325_WR_DATA(  (1 << 12) | (0 << 9) | (1 << 7) | (1 << 5) | (0 << 4) | (1 << 3)); /* set GRAM write direction and BGR=1. */

        }
        break;

        case 3:
        {
            /*竖屏*/
            ili9325_h   = ILI9325_W;
            ili9325_w   = ILI9325_H;
            LCD_ILI9325_WR_CMD(3);
            LCD_ILI9325_WR_DATA(  (1 << 12) | (0 << 9) | (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3)); /* set GRAM write direction and BGR=1. */

        }
        break;

        default:
            //由于开头有校验，因而不会执行到这里
            //LCD_ILI9325_dir(ILI9325_DIR_DEFAULT);
            break;

    }


    LCD_ILI9325_WR_CMD(0X50);
    LCD_ILI9325_WR_DATA(0); //start

    LCD_ILI9325_WR_CMD(0X51);
    LCD_ILI9325_WR_DATA(ili9325_w - 1); //end


    LCD_ILI9325_WR_CMD(0X52);
    LCD_ILI9325_WR_DATA(0 ); //start

    LCD_ILI9325_WR_CMD(0X53);
    LCD_ILI9325_WR_DATA(ili9325_h - 1); //end
}

/*!
 *  @brief      设置ILI9325开窗
 *  @param      site        左上角坐标位置
 *  @param      size        开窗大小
 *  @since      v5.0
 */
void LCD_ILI9325_ptlon(Site_t site, Size_t size)
{


    if(ili9325_dir == 0)
    {



        LCD_ILI9325_WR_CMD(0X50);
        LCD_ILI9325_WR_DATA(site.y ); //start

        LCD_ILI9325_WR_CMD(0X51);
        LCD_ILI9325_WR_DATA(site.y + size.H - 1); //end


        LCD_ILI9325_WR_CMD(0X52);
        LCD_ILI9325_WR_DATA(ili9325_w - site.x - size.W); //end

        LCD_ILI9325_WR_CMD(0X53);
        LCD_ILI9325_WR_DATA(ili9325_w - site.x - 1); //start
    }
    else  if(ili9325_dir == 1)
    {
        LCD_ILI9325_WR_CMD(0X50);
        LCD_ILI9325_WR_DATA(site.x); //start

        LCD_ILI9325_WR_CMD(0X51);
        LCD_ILI9325_WR_DATA(site.x + size.W - 1); //end


        LCD_ILI9325_WR_CMD(0X52);
        LCD_ILI9325_WR_DATA(site.y ); //start

        LCD_ILI9325_WR_CMD(0X53);
        LCD_ILI9325_WR_DATA(site.y + size.H - 1); //end
    }
    else  if(ili9325_dir == 2)
    {



        LCD_ILI9325_WR_CMD(0X50);
        LCD_ILI9325_WR_DATA(ili9325_h - site.y - size.H); //start

        LCD_ILI9325_WR_CMD(0X51);
        LCD_ILI9325_WR_DATA(ili9325_h - site.y - 1  ); //end


        LCD_ILI9325_WR_CMD(0X52);
        LCD_ILI9325_WR_DATA(site.x);

        LCD_ILI9325_WR_CMD(0X53);
        LCD_ILI9325_WR_DATA(site.x + size.W - 1);
    }
    else  if(ili9325_dir == 3)
    {
        LCD_ILI9325_WR_CMD(0X50);
        LCD_ILI9325_WR_DATA(ili9325_w - site.x - size.W ); //start

        LCD_ILI9325_WR_CMD(0X51);
        LCD_ILI9325_WR_DATA(ili9325_w - site.x - 1); //end


        LCD_ILI9325_WR_CMD(0X52);
        LCD_ILI9325_WR_DATA(ili9325_h - site.y - size.H); //start

        LCD_ILI9325_WR_CMD(0X53);
        LCD_ILI9325_WR_DATA(ili9325_h - site.y - 1 ); //end
    }

    // GRAM Horizontal/Vertical Address Set
    LCD_ILI9325_WR_CMD(0X20);
    LCD_ILI9325_WR_DATA(0 );

    LCD_ILI9325_WR_CMD(0X21);
    LCD_ILI9325_WR_DATA(0);


}

/*!
 *  @brief      获取 ILI9325 高度
 *  @return     ILI9325 高度
 *  @since      v5.0
 */
uint16 ILI9325_get_h()
{
    return ili9325_h;
}

/*!
 *  @brief      获取 ILI9325 宽度
 *  @return     ILI9325 宽度
 *  @since      v5.0
 */
uint16 ILI9325_get_w()
{
    return ili9325_w;
}

/*!
 *  @brief      获取 ILI9325 显示方向
 *  @return     ILI9325 方向
 *  @since      v5.0
 */
uint8 ILI9325_get_dir()
{
    return ili9325_dir;
}




