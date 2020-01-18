/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ������������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       VCAN_LCD_ST7735R.c
 * @brief      LCD ������
 * @author     ɽ��Ƽ�
 * @version    v6.0
 * @date       2017-11-04
 */


#include "common.h"
#include "MK66_gpio.h"
#include "VCAN_LCD_ST7735R.h"






//�������õ��� IO�ܽ�
#define LCD_RST    PTC13                //��λ

#define ST7735R_H       128
#define ST7735R_W       128

uint16  st7735r_h   = ST7735R_H;
uint16  st7735r_w   = ST7735R_W;
uint8   st7735r_dir = ST7735R_DIR_DEFAULT;


extern void    LCD_ST7735R_init(Size_t *size);
extern void    LCD_ST7735R_dir(uint8 option);
extern void    LCD_ST7735R_ptlon(Site_t site, Size_t size);
extern void    LCD_ST7735R_ramwr(void);
extern void    LCD_ST7735R_wr_data_t(uint16_t data);
extern uint16   ST7735R_get_h();
extern uint16   ST7735R_get_w();
extern uint8    ST7735R_get_dir();


lcd_t   lcd_st7735r ={
    .lcd_w  =  ST7735R_W,
    .lcd_h  =  ST7735R_H,
    .init   =  LCD_ST7735R_init,
    .ptlon  =  LCD_ST7735R_ptlon,
    .ramwr  =  LCD_ST7735R_ramwr,
    .wr_data=  LCD_ST7735R_wr_data_t
};

#define     LCD_ST7735R_WR_OUT      PTXn_T(LCD_ST7735R_WR,OUT)
#define     LCD_ST7735R_RD_OUT      PTXn_T(LCD_ST7735R_RD,OUT)
#define     LCD_ST7735R_CS_OUT      PTXn_T(LCD_ST7735R_CS,OUT)
#define     LCD_ST7735R_RS_OUT      PTXn_T(LCD_ST7735R_RS,OUT)
#define     LCD_ST7735R_RST_OUT     PTXn_T(LCD_ST7735R_RST,OUT)

#define LCD_ST7735R_WR_8CMD(cmd)    do\
                            {\
                                LCD_ST7735R_RD_OUT=1;\
                                LCD_ST7735R_RS_OUT=0;\
                                LCD_ST7735R_CS_OUT=0;\
                                LCD_ST7735R_P0=(uint8)(cmd);\
                                LCD_ST7735R_WR_OUT=0;\
                                LCD_ST7735R_WR_OUT=1;\
                                LCD_ST7735R_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;LCD_WR=1;����һ��������

#define LCD_ST7735R_WR_8DATA(data)  do\
                            {\
                                LCD_ST7735R_RD_OUT=1;\
                                LCD_ST7735R_RS_OUT=1;\
                                LCD_ST7735R_CS_OUT=0;\
                                LCD_ST7735R_P0=(uint8)(data);\
                                LCD_ST7735R_WR_OUT=0;\
                                LCD_ST7735R_WR_OUT=1;\
                                LCD_ST7735R_CS_OUT=1;\
                            }while(0)   //LCD_WR=0;������д�����ݵ�RAM


#define LCD_ST7735R_WR_DATA(data)       do{LCD_ST7735R_WR_8DATA((uint8)((data)>>8) );LCD_ST7735R_WR_8DATA( (uint8)(data) );}while(0)       //д����




/*!
 *  @brief      LCD_ST7735R��ʼ��
 *  @since      v5.0
 */
extern void    LCD_ST7735R_org_init() ;

void    LCD_ST7735R_init(Size_t *size)
{
    LCD_ST7735R_org_init() ;

    //���÷ֱ���
    size->W = ST7735R_W;
    size->H = ST7735R_H;

}

void    LCD_ST7735R_org_init()
{
    uint8 n ;

    for(n = 0; n < 8; n++)
    {
        gpio_init  ((PTXn_e)(PTC0 + n), GPO, 0);
    }


    gpio_init  (LCD_ST7735R_WR, GPO, 0);
    gpio_init  (LCD_ST7735R_RD, GPO, 1);
    gpio_init  (LCD_ST7735R_CS, GPO, 1);
    gpio_init  (LCD_ST7735R_RS, GPO, 0);
    gpio_init  (LCD_ST7735R_RST, GPO, 0);

    port_init  (LCD_ST7735R_WR , ALT1 | HDS);
    port_init  (LCD_ST7735R_RD , ALT1 | HDS);
    port_init  (LCD_ST7735R_CS , ALT1 | HDS);
    port_init  (LCD_ST7735R_RS , ALT1 | HDS);

    //��ʼ������
    LCD_ST7735R_RST_OUT = 0;
    ST7735R_DELAY();
    LCD_ST7735R_RST_OUT = 1;
    ST7735R_DELAY_MS(500);      //�ϵ���㹻ʱ��

    LCD_ST7735R_WR_8CMD(0x11);           //Sleep out   �˳�˯��ģʽ
    ST7735R_DELAY_MS(120);      //Delay 120ms
    //------------------------------------ST7735R Frame Rate-----------------------------------------//
    /* Set the frame frequency of the full colors normal mode. */
    LCD_ST7735R_WR_8CMD(0xB1);     //In Normal Mode (Full Colors)    ȫ��ģʽ
    LCD_ST7735R_WR_8DATA(0x05);    //���� RTNA Set 1-line  Period  һ������
    LCD_ST7735R_WR_8DATA(0x3A);    //���� FPA: Front Porch
    LCD_ST7735R_WR_8DATA(0x3A);    //���� BPA: Back Porch
    //Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
    //���� fosc = 850kHz

    /* Set the frame frequency of the Idle mode.  */
    LCD_ST7735R_WR_8CMD(0xB2);     //In Idle Mode (8-colors)
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x3A);

    /* Set the frame frequency of the Partial mode/ full colors. */
    LCD_ST7735R_WR_8CMD(0xB3);
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x3A);

    //------------------------------------End ST7735R Frame Rate-----------------------------------------//
    LCD_ST7735R_WR_8CMD(0xB4);   //Display Inversion Control  ��ʾ��ת����

    LCD_ST7735R_WR_8DATA(0x07);  //LCD_ST7735R_WR_8DATA(0x03); ԭ��Ϊ3����Ϊ7
    // ����λ�Ӹߵ��ͣ��ֱ�Ϊ  full colors normal mode ��Idle mode ��
    // full Colors partial mode  �� �㷴ת �� �з�ת ��1Ϊ�з�ת

    LCD_ST7735R_WR_8CMD(0xC0);   //Power Control Setting  ��Դ��������
    LCD_ST7735R_WR_8DATA(0x28);
    LCD_ST7735R_WR_8DATA(0x08);
    LCD_ST7735R_WR_8DATA(0x84);
    LCD_ST7735R_WR_8CMD(0xC1);
    LCD_ST7735R_WR_8DATA(0XC0);
    LCD_ST7735R_WR_8CMD(0xC2);
    LCD_ST7735R_WR_8DATA(0x0C);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8CMD(0xC3);
    LCD_ST7735R_WR_8DATA(0x8C);
    LCD_ST7735R_WR_8DATA(0x2A);
    LCD_ST7735R_WR_8CMD(0xC4);
    LCD_ST7735R_WR_8DATA(0x8A);
    LCD_ST7735R_WR_8DATA(0xEE);
    //---------------------------------End ST7735R Power Sequence-------------------------------------//
    LCD_ST7735R_WR_8CMD(0xC5);   //  VCOM ��ѹ����
    LCD_ST7735R_WR_8DATA(0x0C); //  -0.725
    //------------------------------------ST7735R Gamma Sequence-----------------------------------------//
    LCD_ST7735R_WR_8CMD(0xE0);
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8DATA(0x1A);
    LCD_ST7735R_WR_8DATA(0x0C);
    LCD_ST7735R_WR_8DATA(0x0E);
    LCD_ST7735R_WR_8DATA(0x3A);
    LCD_ST7735R_WR_8DATA(0x34);
    LCD_ST7735R_WR_8DATA(0x2D);
    LCD_ST7735R_WR_8DATA(0x2F);
    LCD_ST7735R_WR_8DATA(0x2D);
    LCD_ST7735R_WR_8DATA(0x2A);
    LCD_ST7735R_WR_8DATA(0x2F);
    LCD_ST7735R_WR_8DATA(0x3C);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8DATA(0x01);
    LCD_ST7735R_WR_8DATA(0x02);
    LCD_ST7735R_WR_8DATA(0x10);
    LCD_ST7735R_WR_8CMD(0xE1);
    LCD_ST7735R_WR_8DATA(0x04);
    LCD_ST7735R_WR_8DATA(0x1B);
    LCD_ST7735R_WR_8DATA(0x0D);
    LCD_ST7735R_WR_8DATA(0x0E);
    LCD_ST7735R_WR_8DATA(0x2D);
    LCD_ST7735R_WR_8DATA(0x29);
    LCD_ST7735R_WR_8DATA(0x24);
    LCD_ST7735R_WR_8DATA(0x29);
    LCD_ST7735R_WR_8DATA(0x28);
    LCD_ST7735R_WR_8DATA(0x26);
    LCD_ST7735R_WR_8DATA(0x31);
    LCD_ST7735R_WR_8DATA(0x3B);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8DATA(0x00);
    LCD_ST7735R_WR_8DATA(0x03);
    LCD_ST7735R_WR_8DATA(0x12);
    //------------------------------------End ST7735R Gamma Sequence-----------------------------------------//

    LCD_ST7735R_WR_8CMD(0x3A); //65k mode
    LCD_ST7735R_WR_8DATA(0x05);
    LCD_ST7735R_WR_8CMD(0x29); //����ʾ Display on
    LCD_ST7735R_WR_8CMD(0x2c); //

    LCD_ST7735R_dir(st7735r_dir);

}

void LCD_ST7735R_ramwr(void)
{
    LCD_ST7735R_WR_8CMD(0x2C);
}
void LCD_ST7735R_wr_data_t(uint16_t data)
{

    LCD_ST7735R_WR_8DATA((uint8)((data)>>8) );
    LCD_ST7735R_WR_8DATA( (uint8)(data)) ;
}

/*!
 *  @brief      ����ST7735R GRAMָ��ɨ�跽��
 *  @param      option    ����ѡ���������������������棩 Ϊ 0 ��90��Ϊ1��180��Ϊ2��270��Ϊ2��
 *  @since      v5.0
 */
void LCD_ST7735R_dir(uint8 option)
{
    ASSERT(option < 4);

    st7735r_dir = option;
    switch(option)
    {
    case 0:
        LCD_ST7735R_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735R_WR_8DATA(0xc8);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(128);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(50);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(128+50);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 1:
        LCD_ST7735R_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735R_WR_8DATA(0xA8);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x03);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x82);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x02);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x81);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    case 2:
        LCD_ST7735R_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735R_WR_8DATA(0x08);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x02);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x81);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x03);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x82);

        st7735r_h   = ST7735R_H;
        st7735r_w   = ST7735R_W;
        break;
    case 3:
        LCD_ST7735R_WR_8CMD(0x36); // �ڴ����ݷ��ʿ���  MX �е�ַ˳��, MY �е�ַ˳�� ��MV ���н��� ��ML ��ֱˢ��˳�� ,RGB  RGB-BGA˳��
        LCD_ST7735R_WR_8DATA(0x68);//0xc8  0xA8 0x08 0x68

        LCD_ST7735R_WR_8CMD(0x2a);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x03);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x82);

        LCD_ST7735R_WR_8CMD(0x2B);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(0x00);
        LCD_ST7735R_WR_8DATA(128);

        st7735r_h   = ST7735R_W;
        st7735r_w   = ST7735R_H;
        break;
    default:
        break;
    }

}

/*!
 *  @brief      ���� ST7735R ����
 *  @param      site        ���Ͻ�����λ��
 *  @param      size        ������С
 *  @since      v5.0
 */
void LCD_ST7735R_ptlon(Site_t site, Size_t size)
{

    if(st7735r_dir&0x01)
    {
        site.x += 32;     //Һ����Ҫƫ��һ�£��������ܿ���������
        //site.y += 2;
    }
    else
    {
        //site.x += 2;     //Һ����Ҫƫ��һ�£��������ܿ���������
        site.y += 32;
    }

    LCD_ST7735R_WR_8CMD(0x2a);   //Partial Mode On  �ֲ�ģʽ
    LCD_ST7735R_WR_8DATA((uint8)(site.x >> 8)); //��8λ Sx
    LCD_ST7735R_WR_8DATA((uint8)site.x);   //��8λ Sx
    LCD_ST7735R_WR_8DATA((uint8)((site.x + size.W - 1) >> 8));
    LCD_ST7735R_WR_8DATA((uint8)(site.x + size.W - 1));

    LCD_ST7735R_WR_8CMD(0x2B);//Row Address Set  �е�ַ����
    LCD_ST7735R_WR_8DATA((uint8)(site.y >> 8));
    LCD_ST7735R_WR_8DATA((uint8)site.y);
    LCD_ST7735R_WR_8DATA((uint8)((site.y + size.H - 1) >> 8));
    LCD_ST7735R_WR_8DATA((uint8)(site.y + size.H - 1));

}

/*!
 *  @brief      ��ȡ ST7735R �߶�
 *  @return     ILI9341 �߶�
 *  @since      v5.0
 */
uint16 ST7735R_get_h()
{
    return st7735r_h;
}

/*!
 *  @brief      ��ȡ ST7735R ����
 *  @return     ILI9341 ����
 *  @since      v5.0
 */
uint16 ST7735R_get_w()
{
    return st7735r_w;
}

/*!
 *  @brief      ��ȡ ST7735R ��ʾ����
 *  @return     ST7735R ����
 *  @since      v5.0
 */
uint8 ST7735R_get_dir()
{
    return st7735r_dir;
}
