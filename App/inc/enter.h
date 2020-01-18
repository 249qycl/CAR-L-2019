#ifndef _ENTER_H_
#define _ENTER_H_
#include "stdint.h"

/*
    FWD 文件为山外UD版核心板专用的固件文件，提供常用字库和函数功能，加快开发速度和调试速度。
    FWD是可执行的文件。直接烧录到指定的位置，即可在 APP中调用相关的 API 函数。非UD版，请勿使用FWD固件。

    烧录方法：

        插好usb线，按着 独立按键(默认是 PTD7，即核心板上的独立按键)，然后复位，即可进入UD模式。

        电脑会识别到插入UD下载盘，把FWD文件复制到U盘的根目录即可。


    若在使用中发现有bug，请发邮件到此邮箱：minimcu@foxmail.com。我们会尽快更新。

*/


//用户的API 函数接口，可看下面的结构体的注释，有注明函数原型
//一般看注释里的函数原型即可使用的。
//APP需要初始化时执行 ASSERT(enter_init());判断是否已经烧录了固件

#define BOOT_VER    102      // 固件版本号 （请勿修改，仅作给上层识别）
#define FWD_VER     602      // 固件版本号 （请勿修改，仅作给上层识别）

#define USE_FOUNT_SIZE      16 //12 或 16号字体 （请勿修改，仅作给上层识别）


#define FOUNT12_ADDR        //定义12号字体的汉字字库地址 （如果 USE_FOUNT_SIZE 为12，不需要写，留空即可）
#define FOUNT16_ADDR        //定义16号字体的汉字字库地址 （如果 USE_FOUNT_SIZE 为16，不需要写，留空即可。其他的，则需要写，否则不能用 16号的字体函数）


//printf / tpm_pwm_init / tpm_pwm_freq /tpm_input_init /  ftm_quad_init   LCD 6X12    特殊处理



typedef struct
{
    uint16_t    flag;                   //标记
    uint16_t    ver;                    //版本

    //字库
    uint8_t *   ascii12;                //英文字库，char *  ascii_6x12 ;
    uint8_t *   ascii16;                //英文字库，char *  ascii_8x16 ;


    //字符串 变量 转换
    void *      sprintf;                //int       sprintf         (char *buf, const char *fmt, ...) ;          //字符串格式化到缓冲区
    void *      printf;                 //void      printf          (fp,fmt, ...);                               //字符串格式化输出到指定端口 fp （常用为串口）
    void *      atoi;                   //int       atoi            (char *str);                                //字符串转成整型
    void *      itoa;                   //char *    itoa            (int n, char * chBuffer);                   //整型转换为字符串
    void *      ftoa;                   //char *    ftoa            (double dValue, char * chBuffer);           //浮点转换为字符串



    //常用小功能
    void *      memcpy;                 //void *    memcpy  ( uint8_t *dst, uint8_t *src, uint32_t count);      //内存复制
    void *      memset;                 //void *    memset  (void *src, int c, int count);                      //内存设置数值
    void *      strlen;                 //int       strlen(const char * str);                                   //求字符串的长度
    void *      img_extract;            //void      img_extract(void *dst, void *src, uint32_t srclen);         //摄像头解压

    void *      get_isr_info;           //int       get_isr_info    (char * str);                               //获取中断号所对应的具体意义


    //MCG 超频函数
    void *      pll_init;               //uint16_t    pll_init        (PLL_e pll,uint16_t max_bus_mhz,uint16_t max_flexbus_mhz,uint16_t max_flash_mhz);         //超频配置

    //flash API
    void *      flash_init;             //void      flash_init          ();                                     //初始化Flash
    void *      flash_erase_sector;     //uint8_t   flash_erase_sector  (uint16_t sectorNum);                     //擦除指定flash扇区
    void *      flash_write;            //uint8_t   flash_write         (uint16_t sectorNum, uint16_t offset, FLASH_WRITE_TYPE data);       //写入flash操作
    void *      flash_write_buf;        //uint8_t   flash_write_buf     (uint16_t sectorNum, uint16_t offset, uint16_t cnt, uint8 buf[]);     //从缓存区写入flash操作
    //还有一个flash API接口：                        flash_read          (sectorNum,offset,type)                  //读取指定地址的数据


    //flash 存储数据
    void *      flash_data_init;        //void      flash_data_init     (flash_data_t *d);                      //对结构体数据进行预处理，若flash的数据还没初始化，则重置flash
    void *      flash_data_reset;       //void      flash_data_reset    (flash_data_t *d);                      //重置flash，就是对存储数据进行清空
    void *      flash_data_load;        //uint8_t   flash_data_load     (flash_data_t *d);                      //从 flash 加载数据
    void *      flash_data_save;        //uint8_t   flash_data_save     (flash_data_t *d);                      //保存数据到flash


    //dwt 延时函数
    void *      dwt_delay_us;           //void      dwt_delay_us        (uint32_t core_clk,uint32_t us);          //dwt延时函数，最终用 DWT_DELAY_US(us) 来代替
    void *      dwt_delay_ms;           //void      dwt_delay_ms        (uint32_t core_clk,uint32_t ms);          //dwt延时函数，最终用 DWT_DELAY_MS(ms) 来代替

    //SYSTICK 延时函数
    void *      systick_delay;          //void      systick_delay       (uint32_t time);;                         //延时 time 个core周期，24位变量
    void *      systick_delay_ms;       //void      systick_delay_ms    (uint32_t ms,uint32_t core_khz);          //延时 time 毫米。最终会以宏定义 SYSTICK_DELAY_MS(ms) 替换

    //SYSTICK 定时函数
    void *      systick_timing;         //void      systick_timing      (uint32_t time);                          //定时中断初始化函数， time个core周期，24bit




    //PIT. 定时函数
    void *      pit_init;               //void      pit_init            (PITn_e, uint32_t cnt);                   //初始化PITn，并设置定时时间(单位为bus时钟周期)

    //PIT. 延时函数
    void *      pit_delay;               //void     pit_delay           (PITn_e, uint32_t cnt);                   //PIT延时（不需要初始化的）

    //PIT. 计时函数
    void *      pit_time_start;         //void      pit_time_start      (PITn_e);                               //PIT开始计时
    void *      pit_time_get;           //uint32_t  pit_time_get        (PITn_e);                               //获取 PITn计时时间(超时时会关闭 定时器)（单位为 bus时钟）(若值为 0xFFFFFFFF，则表示溢出)
    void *      pit_close;              //void      pit_close           (PITn_e);                               //关闭 PIT 计时


    //LPT  延时函数
    void *      lptmr_delay_ms;         //void      lptmr_delay_ms       (uint16_t ms);                           //延时(ms)
    void *      lptmr_delay_us;         //void      lptmr_delay_us       (uint16_t us);                           //延时(us)

    //LPT  定时中断函数
    void *      lptmr_timing_ms;        //void      lptmr_timing_ms      (uint16_t ms);                           //定时(ms)
    void *      lptmr_timing_us;        //void      lptmr_timing_us      (uint16_t us);                           //定时(us)

    //LPT  计时函数
    void *      lptmr_time_start_ms;    //void      lptmr_time_start_ms  (void);                                //开始计时(ms)
    void *      lptmr_time_get_ms;      //uint32_t  lptmr_time_get_ms    (void);                                //获取计时时间
    void *      lptmr_time_start_us;    //void      lptmr_time_start_us  (void);                                //开始计时(ns)
    void *      lptmr_time_get_us;      //uint32_t  lptmr_time_get_us    (void);                                //获取计时时间
    void *      lptmr_time_close;       //void      lptmr_time_close     ();                                    //关闭计时器

    //LPT  脉冲计数函数
    void *      lptmr_pulse_init;       //void      lptmr_pulse_init     (LPT0_ALTn, uint16_t count, LPT_CFG);   //脉冲计数器初始化设置
    void *      lptmr_pulse_get;        //uint16_t  lptmr_pulse_get      (void);                               //获取计数值
    void *      lptmr_pulse_clean;      //void      lptmr_pulse_clean    (void);                               //清空计数值


    //TPM  PWM输出
    void *      tpm_pwm_init;           //int8_t    tpm_pwm_init        (TPMn_e, TPM_CHn_e, uint32_t freq, float duty,PTXn_e pin,uint32_t core_khz);    //初始化TPM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个TPM，各通道的PWM频率是一样的
    void *      tpm_pwm_duty;           //void      tpm_pwm_duty        (TPMn_e, TPM_CHn_e,                float duty);                                 //设置通道占空比,占空比为 duty%
    void *      tpm_pwm_freq;           //int8_t    tpm_pwm_freq        (TPMn_e,            uint32_t freq, uint32_t core_khz);                          //设置TPM的频率（改频率后，需要重新配置占空比）

    //TPM  输入捕捉
    void *      tpm_input_init;         //void      tpm_input_init      (TPMn_e, TPM_CHn_e, TPM_Input_cfg cfg,TPM_PS_e ps,PTXn_e pin);      //输入捕捉初始化
    void *      tpm_input_get;          //uint16_t  tpm_input_get       (TPMn_e, TPM_CHn_e);                                                //输入捕捉模式下，读取捕捉事件发生时的计数器值(根据两次捕捉之间的差值可判断周期频率)
    void *      tpm_input_clean;        //void      tpm_input_clean     (TPMn_e );                                                          //清空计数器的值

    //TPM  脉冲计数
    void *      tpm_pulse_init;         //void      tpm_pulse_init      (TPMn_e tpmn,TPM_CLKIN_e clkin,TPM_PS_e ps,PTXn_e pin);             //脉冲计数初始化
    void *      tpm_pulse_get;          //uint16_t  tpm_pulse_get       (TPMn_e tpmn);                                                      //获取脉冲数
    void *      tpm_pulse_clean;        //void      tpm_pulse_clean     (TPMn_e tpmn);                                                      //清脉冲计数

    //FTM  PWM输出
    void *      ftm_pwm_init;           //int8_t    ftm_pwm_init        (FTMn_e, FTM_CHn_e, uint32_t freq, float duty,PTXn_e pin,uint32_t bus_khz);     //初始化FTM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个FTM，各通道的PWM频率是一样的，共3个FTM(FX有4个)
    void *      ftm_pwm_duty;           //void      ftm_pwm_duty        (FTMn_e, FTM_CHn_e,              float duty);                                   //设置通道占空比,占空比为 （duty * 精度） % ，如果 FTM_PRECISON 定义为 1000 ，duty = 100 ，则占空比 100*0.1%=10%
    void *      ftm_pwm_freq;           //int8_t    ftm_pwm_freq        (FTMn_e,            uint32_t freq,uint32_t bus_khz);                            //设置FTM的频率（改频率后，需要重新配置占空比）

    //FTM  输入捕捉
    void *      ftm_input_init;         //void      ftm_input_init      (FTMn_e, FTM_CHn_e, FTM_Input_cfg,FTM_PS_e ps,PTXn_e pin);      //输入捕捉初始化函数
    void *      ftm_input_get;          //uint16_t  ftm_input_get       (FTMn_e, FTM_CHn_e);                                            //输入捕捉模式下，读取捕捉事件发生时的计数器值(根据两次捕捉之间的差值可判断周期频率)
    void *      ftm_input_clean;        //void      ftm_input_clean     (FTMn_e );                                                      //清空计数器的值

    //FTM  正交解码
    void *      ftm_quad_init;          //void      ftm_quad_init       (FTMn_e, FTM_PS_e ps,PTXn_e Apin,PTXn_e Bpin);                  //初始化FTM 的正交解码 功能
    void *      ftm_quad_get;           //int16_t   ftm_quad_get        (FTMn_e);                                                       //获取FTM 正交解码 的脉冲数(负数表示反方向)
    void *      ftm_quad_clean;         //void      ftm_quad_clean      (FTMn_e);           //清 FTM 正交解码 的脉冲数

    //DAC 模拟输出
    void *      dac_init;               //void      dac_init            (DACn_e);               //DAC一次转换初始化
    void *      dac_out;                //void      dac_out             (DACn_e, uint16_t val);    //DAC一次转换操作


    //LCD 显示
    void *      lcd_init;               //void      lcd_init            (lcd_t * lcd,uint16_t rgb565);

    void *      lcd_point;              //void      lcd_point           (lcd_t * lcd,Site_t,                            uint16_t rgb565);           //画点
    void *      lcd_points;             //void      lcd_points          (lcd_t * lcd,Site_t *site,  uint32_t point_num, uint16_t rgb565);           //画一堆点

    void *      lcd_rectangle;          //void      lcd_rectangle       (lcd_t * lcd,Site_t,Size_t,                     uint16_t rgb565);           //画矩形
    void *      lcd_cross;              //void      lcd_cross           (lcd_t * lcd,Site_t,uint16_t len,               uint16_t rgb565);           //画十字形
    void *      lcd_clear;              //void      lcd_clear           (lcd_t * lcd,uint16_t rgb565);                                              //清屏


    //LCD 英文字符、数字显示
    void *      lcd_char_6x12;          //void      lcd_char_6x12       (lcd_t * lcd,Site_t site, uint8 ascii, uint16_t Color, uint16_t bkColor) ;                          //显示英文字符
    void *      lcd_str_6x12;           //void      lcd_str_6x12        (lcd_t * lcd,Site_t site, uint8 *Str,  uint16_t Color, uint16_t bkColor);                           //显示英文字符串
    void *      lcd_num_6x12;           //uint8_t   lcd_num_6x12        (lcd_t * lcd,Site_t site, uint32_t num,uint16_t Color, uint16_t bkColor);                           //显示无符号数字
    void *      lcd_snum_6x12;          //uint8_t   lcd_snum_6x12       (lcd_t * lcd,Site_t site, int32 num,   uint16_t Color, uint16_t bkColor);                           //显示有符号数字

    void *      lcd_num_bc_6x12;        //void      lcd_num_bc_6x12     (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);            //显示无符号数字(擦除后面数据)
    void *      lcd_snum_bc_6x12;       //void      lcd_snum_bc_6x12    (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);            //显示有符号数字(擦除后面数据)

    //LCD 英文数字显示
    void *      lcd_char_8x16;          //void      lcd_char_8x16       (lcd_t * lcd,Site_t site, uint8 ascii, uint16_t Color, uint16_t bkColor);                               //显示英文字符
    void *      lcd_str_8x16;           //void      lcd_str_8x16        (lcd_t * lcd,Site_t site, uint8 *Str,  uint16_t Color, uint16_t bkColor);                               //显示英文字符串
    void *      lcd_num_8x16;           //uint8_t   lcd_num_8x16        (lcd_t * lcd,Site_t site, uint32_t num,uint16_t Color, uint16_t bkColor) ;                              //显示无符号数字
    void *      lcd_snum_8x16;          //uint8_t   lcd_snum_8x16       (lcd_t * lcd,Site_t site, int32 num,   uint16_t Color, uint16_t bkColor) ;                              //显示有符号数字

    void *      lcd_num_bc_8x16;        //void      lcd_num_bc_8x16     (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);                //显示无符号数字(擦除后面数据)
    void *      lcd_snum_bc_8x16;       //void      lcd_snum_bc_8x16    (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);                //显示有符号数字(擦除后面数据)




    //LCD 图像显示
    void *      lcd_img_binary;         //void      lcd_img_binary      (lcd_t * lcd,Site_t site, Size_t size, uint8 *img,                uint16_t Color, uint16_t bkColor);    //显示二值化图像
    void *      lcd_img_binary_z;       //void      lcd_img_binary_z    (lcd_t * lcd,Site_t site, Size_t size, uint8 *img, Size_t imgsize,uint16_t Color, uint16_t bkColor);    //显示二值化图像(可缩放)
    void *      lcd_img_gray;           //void      lcd_img_gray        (lcd_t * lcd,Site_t site, Size_t size, uint8 *img);                                                     //显示灰度图像
    void *      lcd_img_gray_z;         //void      lcd_img_gray_z      (lcd_t * lcd,Site_t site, Size_t size, uint8 *img, Size_t imgsize);                                     //显示灰度图像(可缩放)
    void *      lcd_img_rgb565;         //void      lcd_img_rgb565      (lcd_t * lcd,Site_t site, Size_t size, uint8 *img) ;                                                    //彩色
    void *      lcd_img_rgb565_z;       //void      lcd_img_rgb565_z    (lcd_t * lcd,Site_t site, Size_t size, uint8 *img, Size_t imgsize);                                     //彩色(可缩放)

    //LCD 波形显示
    void *      lcd_wave;               //void      lcd_wave            (lcd_t * lcd,Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16_t Color ,uint16_t bkColor);         //波形显示，带背景颜色（maxval 最大值表示超过此值的数都当作最大值处理）
    void *      lcd_wave_display;       //void      lcd_wave_display    (lcd_t * lcd,Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16_t Color);                           //波形显示，不带背景颜色


    //执行新APP API接口 （和普通的 函数调用不同，下面的函数都重置栈地址）
    void *      jmp_app;                //void      jmp_app             (uint32_t sp, uint32_t pc);          //跳转到新的APP
    void *      jmp_bin;                //void      jmp_bin             (uint32_t addr);                     //跳转到新的APP BIN文件 （bin文件的第1个32位数据是栈地址，第2个32位数据是启动函数入口）

}mydata_boot_in_t;




typedef struct
{
    uint16_t    flag;                   //标记
    uint16_t    ver;                    //版本

    uint8_t *   chinese;                //中文字库入口  大小由 USE_FOUNT_SIZE 判断


    //void *      get_isr;                //int       get_isr_info    (char * str);                               //获取中断号所对应的具体意义




    //LCD 汉字显示
    void *      lcd_char_ch_12x12;      //void      lcd_char_ch_12x12   (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_12x12);     //显示汉字字符          （若 USE_FOUNT_SIZE 为12，最后一个参数不需要写）
    void *      lcd_str_ch_12x12;       //void      lcd_str_ch_12x12    (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_12x12);     //显示汉字字符串        （若 USE_FOUNT_SIZE 为12，最后一个参数不需要写）
    void *      lcd_str_ench_12x12;     //void      lcd_str_ench_12x12  (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_12x12);     //显示汉字英文混合字符串 （若 USE_FOUNT_SIZE 为12，最后一个参数不需要写）




    void *      lcd_char_ch_16x16;      //void      lcd_char_ch_16x16   (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_16x16);     //显示汉字字符          （若 USE_FOUNT_SIZE 为16，最后一个参数不需要写）
    void *      lcd_str_ch_16x16;       //void      lcd_str_ch_16x16    (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_16x16);     //显示汉字字符串        （若 USE_FOUNT_SIZE 为16，最后一个参数不需要写）
    void *      lcd_str_ench_16x16;     //void      lcd_str_ench_16x16  (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_16x16);     //显示汉字英文混合字符串 （若 USE_FOUNT_SIZE 为16，最后一个参数不需要写）


}mydata_in_t;


#ifndef FWD_BOOT_LIB

#define    FTMN_addr                              0x0000dcec
#define    TPMN_addr                              0x0000e010
#define    ascii_6x12_addr                        0x0000cba0
#define    ascii_8x16_addr                        0x0000c5b0
#define    atoi_addr                              0x00008187
#define    dac_init_addr                          0x0000d9b5
#define    dac_out_addr                           0x0000da09
#define    dwt_delay_ms_addr                      0x0000da85
#define    dwt_delay_us_addr                      0x0000da2d
#define    flash_data_init_addr                   0x0000c339
#define    flash_data_load_addr                   0x0000c495
#define    flash_data_reset_addr                  0x0000c40f
#define    flash_data_save_addr                   0x0000c4c1
#define    flash_erase_sector_addr                0x0000c031
#define    flash_init_addr                        0x0000c003
#define    flash_write_addr                       0x0000c095
#define    flash_write_buf_addr                   0x0000c13b
#define    ftm_input_clean_addr                   0x0000b6ef
#define    ftm_input_get_addr                     0x0000b6db
#define    ftm_input_init_addr                    0x0000b553
#define    ftm_pwm_duty_addr                      0x0000b3bd
#define    ftm_pwm_freq_addr                      0x0000b4cb
#define    ftm_pwm_init_addr                      0x0000b197
#define    ftm_quad_clean_addr                    0x0000b8af
#define    ftm_quad_get_addr                      0x0000b89d
#define    ftm_quad_init_addr                     0x0000b6fd
#define    ftoa_addr                              0x00008809
#define    get_isr_info_addr                      0x00009fc5
#define    img_extract_addr                       0x00009e1d
#define    itoa_addr                              0x000082b1
#define    jmp_app_addr                           0x00009e09
#define    jmp_bin_addr                           0x00009e0f
#define    lcd_char_6x12_addr                     0x0000a551
#define    lcd_char_8x16_addr                     0x0000a1d1
#define    lcd_clear_addr                         0x0000a05f
#define    lcd_cross_addr                         0x0000a089
#define    lcd_img_binary_addr                    0x0000aadf
#define    lcd_img_binary_z_addr                  0x0000ab59
#define    lcd_img_gray_addr                      0x0000a8f3
#define    lcd_img_gray_z_addr                    0x0000a997
#define    lcd_img_rgb565_addr                    0x0000a95b
#define    lcd_img_rgb565_z_addr                  0x0000aa4d
#define    lcd_init_addr                          0x00009ff5
#define    lcd_num_6x12_addr                      0x0000a6e5
#define    lcd_num_8x16_addr                      0x0000a349
#define    lcd_num_bc_6x12_addr                   0x0000a7af
#define    lcd_num_bc_8x16_addr                   0x0000a46d
#define    lcd_point_addr                         0x0000a185
#define    lcd_points_addr                        0x0000a1a9
#define    lcd_rectangle_addr                     0x0000a025
#define    lcd_snum_6x12_addr                     0x0000a823
#define    lcd_snum_8x16_addr                     0x0000a411
#define    lcd_snum_bc_6x12_addr                  0x0000a87f
#define    lcd_snum_bc_8x16_addr                  0x0000a4df
#define    lcd_str_6x12_addr                      0x0000a629
#define    lcd_str_8x16_addr                      0x0000a28d
#define    lcd_wave_addr                          0x0000ac13
#define    lcd_wave_display_addr                  0x0000acb9
#define    lptmr_delay_ms_addr                    0x0000d35b
#define    lptmr_delay_us_addr                    0x0000d3c7
#define    lptmr_pulse_clean_addr                 0x0000d341
#define    lptmr_pulse_get_addr                   0x0000d321
#define    lptmr_pulse_init_addr                  0x0000d2b5
#define    lptmr_time_close_addr                  0x0000d495
#define    lptmr_time_get_ms_addr                 0x0000d477
#define    lptmr_time_get_us_addr                 0x0000d4d1
#define    lptmr_time_start_ms_addr               0x0000d44f
#define    lptmr_time_start_us_addr               0x0000d49d
#define    lptmr_timing_ms_addr                   0x0000d39b
#define    lptmr_timing_us_addr                   0x0000d415
#define    memcpy_addr                            0x00009f1f
#define    memset_addr                            0x00009f9b
#define    pit_close_addr                         0x0000d705
#define    pit_delay_addr                         0x0000d573
#define    pit_init_addr                          0x0000d511
#define    pit_time_get_addr                      0x0000d6a1
#define    pit_time_start_addr                    0x0000d627
#define    pll_init_addr                          0x0000d07f
#define    printf_addr                            0x0000d915
#define    sprintf_addr                           0x000094dd
#define    strlen_addr                            0x00009fb1
#define    systick_delay_addr                     0x0000daa1
#define    systick_delay_ms_addr                  0x0000dac5
#define    systick_timing_addr                    0x0000dadd
#define    tpm_input_clean_addr                   0x0000be29
#define    tpm_input_get_addr                     0x0000be15
#define    tpm_input_init_addr                    0x0000bcb3
#define    tpm_pulse_clean_addr                   0x0000bf4f
#define    tpm_pulse_get_addr                     0x0000bf5d
#define    tpm_pulse_init_addr                    0x0000be37
#define    tpm_pwm_duty_addr                      0x0000bbd3
#define    tpm_pwm_freq_addr                      0x0000bc49
#define    tpm_pwm_init_addr                      0x0000ba43







#define     ascii_6x12      ((const char *)ascii_6x12_addr)
#define     ascii_8x16      ((const char *)ascii_8x16_addr)


#endif

//数据的尺寸 必须是 FLASH_ALIGN_ADDR 对齐
//
typedef struct
{
    uint16_t    sectornum_start;            //开始的扇区
    uint16_t    sectornum_end;              //结束的扇区
    void *     data_addr;                   //数据的地址
    uint16_t    data_size;                  //数据的尺寸
    uint16_t    res1;                       //保留，底层使用 （勿删）
    uint32_t    res2[2];                    // 保留，底层使用 （勿删）


}flash_data_t;

//     固件的起始地址 (用户不可更改，仅作APP调用识别)
#define START_APP_ADDR  (32*1024)               //APP启动地址
#if defined(MK60DZ10)
#define START_FWD_ADDR  (256*1024)              //DN 是烧录到后半部分flash，256k
#elif defined(MK60FN15)
#define START_FWD_ADDR  (512*1024)              //FN 是烧录到后半部分flash，512k
#elif defined(MK60FX15)
#define START_FWD_ADDR  (0x10000000)            //FX 是烧录到 flexNVM
#elif defined(MK66FX18)
#define START_FWD_ADDR  (0x10000000)            //K66FX 是烧录到 flexNVM
#endif

#define BOOT_FWD_ADDR   (0x8000)

//内部用，用户无需关注
#define     OFFSET(type, member)                (uint32)(&(((type *)0)->member))
#define     IN_ADDR(member)                     (*((uint32_t *)(START_FWD_ADDR + OFFSET(mydata_in_t, member)) ))
#define     ENTER_FUN(func,...)                 (*(func##_t)(IN_ADDR(func)))(##__VA_ARGS__)
#define     ENTER_FUN_ADDR(func,...)            (*(func##_t)(func##_addr))(##__VA_ARGS__)
#define     PENTER_FUN_ADDR(func,...)            (*(func##_t)(func##_addr))(##__VA_ARGS__)

#define     BOOT_ADDR(member)                   (*((uint32_t *)(BOOT_FWD_ADDR + OFFSET(mydata_boot_in_t, member)) ))


//以下4个是备用，和ENTER_FUN_ADDR是一样的。如果把enter函数加入到另外enter函数的形参，就会导致提示：
// no definition for "ENTER_FUN_ADDR"
//就是形参里的宏没法正常解析，只需要把形参的宏换成下方的即可
#define     ENTER_FUN_ADDR0(func,...)            (*(func##_t)(func##_addr))(##__VA_ARGS__)
#define     ENTER_FUN_ADDR1(func,...)            (*(func##_t)(func##_addr))(##__VA_ARGS__)
#define     ENTER_FUN_ADDR2(func,...)            (*(func##_t)(func##_addr))(##__VA_ARGS__)
#define     ENTER_FUN_ADDR3(func,...)            (*(func##_t)(func##_addr))(##__VA_ARGS__)


//用户的 API 接口调用
//
#ifndef FWD_LIB         //请勿在工程中定义此宏定义(内部识别用的) by 山外

//#define  enter_init()      (((uint16_t)IN_ADDR(flag)==0x55A5) && (uint16_t)IN_ADDR(ver)==FWD_VER)      //用于判断固件文件是否存在 ,APP需要初始化时执行 ASSERT(enter_init());


//K66FX1M0VLQ18     Flash参数     1M的flash，256K ROM API ,256K RAM
#define     FLASH_SECTOR_SIZE       (4*1024)                //扇区大小 为 4k 字节
#define     FLASH_SECTOR_NUM        (256)                   //扇区数

#define     FLASH_ALIGN_ADDR        8                       //地址对齐整数倍
typedef     uint64                  FLASH_WRITE_TYPE;       //flash_write 函数写入 的数据类型

extern int core_clk_khz;
extern int core_clk_mhz;
extern int bus_clk_khz;

typedef enum
{
    PLLUSR      ,  //不配置频率（自行编写超频函数）
    PLL80       ,
    PLL88       ,
    PLL96       ,
    PLL100       ,
    PLL120       ,
    PLL140       ,
    PLL152       ,
    PLL160       ,
    PLL180       ,
    PLL188       ,
    PLL200       ,
    PLL224       ,
    PLL232       ,
    PLL240       ,
    PLL248       ,
    PLL256       ,
    PLL264       ,

    //由于我们函数的时钟是以MHz为单位，不支持小数。
    PLL_MAX,
} PLL_e;

typedef void lcd_modeinit_t(Size_t *size);              //初始化。把分辨率写入到size
typedef void lcd_ptlon_t(Site_t site, Size_t size);
typedef void lcd_ramwr_t(void);
typedef void lcd_wr_data_t(uint16_t data);


typedef struct
{
    union {
        struct{
            uint16_t lcd_w;         //LCD 宽
            uint16_t lcd_h;         //LCD 高
        };
        Size_t size;
    } ;

    lcd_modeinit_t  *   init;      //初始化
    lcd_ptlon_t *       ptlon;     //开窗

    lcd_ramwr_t *       ramwr;     //写模式
    lcd_wr_data_t *     wr_data;   //写数据


}lcd_t;

extern lcd_t *  glcd;   //需要外部添加定义

#define LCD_H   glcd->lcd_h
#define LCD_W   glcd->lcd_w

/******常用颜色*****/
#define RED     0XF800    //红色
#define GREEN   0X07E0    //绿色
#define BLUE    0X001F    //蓝色
#define BRED    0XF81F
#define GRED    0XFFE0    //灰色
#define GBLUE   0X07FF    //
#define BLACK   0X0000    //黑色
#define WHITE   0XFFFF    //白色
#define YELLOW  0xFFE0    //黄色

#define FCOLOUR         BLUE    //定义字体颜色
#define BCOLOUR         RED     //定义背景颜色

//提取RGB的3元素，rgb565必须是16位
#define RGB565_R(rgb565)                ((uint8)(((uint16)(rgb565)>>11) &0x1F))
#define RGB565_G(rgb565)                ((uint8)(((uint16)(rgb565)>> 5) &0x3F))
#define RGB565_B(rgb565)                ((uint8)( (uint16)(rgb565)      &0x1F))


#define GRAY_2_RGB565(gray)             ((uint16)((((uint8)(gray)>>3)<<11)|(((uint8)(gray)>>2)<<5)|((uint8)(gray)>>3)))

#define RGB565_2_GRAY(rgb565)           ((uint8)(((RGB565_R(rgb565)*235+RGB565_G(rgb565)*613+RGB565_B(rgb565)*625)+1)>>8))  //  31*235+63*613+31*625+1  = 255*256


#define RGB565_H(rgb565)                ((uint8)(((uint16)(rgb565))>>8))
#define RGB565_L(rgb565)                (uint8)(rgb565))

#define RGB24_RGB565(R,G,B)             ((uint16)((((uint8)(R)>>3)<<11)|(((uint8)(G)>>2)<<5)|((uint8)(B)>>3)))
#define RGB555_RGB565(rgb555)           (((((uint16)(rgb555)) & (uint16)(~0x1F)) << 1) |  (((uint16)(rgb555)) & 0x1F))
#define BGR555_RGB565(bgr555)           ((((uint16)(bgr555)<<11) & (uint16)(0x1F<<11)) | (((uint16)(bgr555)) & (uint16)(0x1F<<5)) | (((uint16)(bgr555)>>10) & (uint16)(0x1F)))

#define LCD_MAX_NUM_BIT  5

#define lcd_num_c_6x12(site,num,color,bkColor)           lcd_num_bc_6x12(site, num,LCD_MAX_NUM_BIT,color,bkColor)
#define lcd_snum_c_6x12(site,num,color,bkColor)          lcd_snum_bc_6x12(site, num,LCD_MAX_NUM_BIT,color,bkColor)

#define lcd_num_c_8x16(site,num,color,bkColor)           lcd_num_bc_8x16(site, num,LCD_MAX_NUM_BIT,color,bkColor)
#define lcd_snum_c_8x16(site,num,color,bkColor)          lcd_snum_bc_8x16(site, num,LCD_MAX_NUM_BIT,color,bkColor)


#define  CHINESE            ((uint8_t *)(IN_ADDR(chinese)))            // 汉字字库  (尺寸看结构体mydata_in_t的注释)

#if( USE_FOUNT_SIZE == 16 )

#define    ASCII              ascii_8x16

#define    lcd_char           lcd_char_8x16
#define    lcd_str            lcd_str_8x16
#define    lcd_num            lcd_num_8x16
#define    lcd_snum           lcd_snum_8x16
#define    lcd_num_bc         lcd_num_bc_8x16
#define    lcd_snum_bc        lcd_snum_bc_8x16
#define    lcd_char_ch        lcd_char_ch_16x16
#define    lcd_str_ch         lcd_str_ch_16x16
#define    lcd_str_ench       lcd_str_ench_16x16

#define    lcd_num_c          lcd_num_c_8x16
#define    lcd_snum_c         lcd_snum_c_8x16

#elif ( USE_FOUNT_SIZE == 12 )

#define    ASCII              ascii_6x12

#define    lcd_char           lcd_char_6x12
#define    lcd_str            lcd_str_6x12
#define    lcd_num            lcd_num_6x12
#define    lcd_snum           lcd_snum_6x12
#define    lcd_num_bc         lcd_num_bc_6x12
#define    lcd_snum_bc        lcd_snum_bc_6x12
#define    lcd_char_ch        lcd_char_ch_12x12
#define    lcd_str_ch         lcd_str_ch_12x12
#define    lcd_str_ench       lcd_str_ench_12x12

#define    lcd_num_c          lcd_num_c_6x12
#define    lcd_snum_c         lcd_snum_c_6x12
#endif



//定义TPM模块号
typedef enum
{
    //TPM0,
    TPM1,
    TPM2,

    TPM_MAX,
} TPMn_e;

//定义TPM 通道号
typedef enum
{

    TPM_CH0,
    TPM_CH1,
    TPM_CH2,
    TPM_CH3,
    TPM_CH4,
    TPM_CH5,
    TPM_CH6,
    TPM_CH7,

} TPM_CHn_e;

typedef enum
{
    TPM_CLKIN0,         // 相应的管脚请看 TPM_CLKIN0_PIN 的定义
    TPM_CLKIN1,         // 相应的管脚请看 TPM_CLKIN1_PIN 的定义
} TPM_CLKIN_e;

//分频系数
typedef enum
{
    TPM_PS_1,
    TPM_PS_2,
    TPM_PS_4,
    TPM_PS_8,
    TPM_PS_16,
    TPM_PS_32,
    TPM_PS_64,
    TPM_PS_128,

    TPM_PS_MAX,
}TPM_PS_e;

//用于开始计数 或者 重载计数 (暂时没用到)
typedef enum
{
    EXTRG_IN,       //外部触发
    CMP0_OUT,       //CMP0 输出

    TRG_CFG_RES0,   //保留
    TRG_CFG_RES1,   //保留

    PIT0_TRG,       //PIT0 触发
    PIT1_TRG,       //PIT0 触发

    TRG_CFG_RES2,   //保留
    TRG_CFG_RES3,   //保留

    TPM0_OVER,      //TPM0溢出
    TPM1_OVER,      //TPM1溢出
    TPM2_OVER,      //TPM2溢出

    TRG_CFG_RES4,   //保留

    RTC_ALARM,      //RTC报警
    RTC_SEC,        //RTC秒中断

    LPTMR_TRG,      //LPTMR 触发

    TRG_CFG_RES5,   //保留


}TPMx_TRG_CFG_e;



//TPM 输入捕捉配置
typedef enum
{
    TPM_Rising,               //上升沿捕捉
    TPM_Falling,              //下降沿捕捉
    TPM_Rising_or_Falling     //跳变沿捕捉
} TPM_Input_cfg;


#define TPMN     ((TPM_MemMapPtr *) TPMN_addr )

#define TPM_IRQ_EN(TPMn,CHn)        TPM_CnSC_REG(TPMN[TPMn],CHn) |= TPM_CnSC_CHIE_MASK       //开启 TPMn_CHn 中断
#define TPM_IRQ_DIS(TPMn,CHn)       TPM_CnSC_REG(TPMN[TPMn],CHn) &= ~TPM_CnSC_CHIE_MASK      //关闭 TPMn_CHn 中断



//定义FTM模块号
typedef enum
{
    FTM0,
    FTM1,
    FTM2,
    FTM3,

    FTM_MAX,
} FTMn_e;

//定义FTM 通道号
typedef enum
{

    FTM_CH0,
    FTM_CH1,
    FTM_CH2,
    FTM_CH3,
    FTM_CH4,
    FTM_CH5,
    FTM_CH6,
    FTM_CH7,

} FTM_CHn_e;

//分频系数
typedef enum
{
    FTM_PS_1,
    FTM_PS_2,
    FTM_PS_4,
    FTM_PS_8,
    FTM_PS_16,
    FTM_PS_32,
    FTM_PS_64,
    FTM_PS_128,

    FTM_PS_MAX,
}FTM_PS_e;      //分频值 =  (1<< FTM_PS_e) ,例如  FTM_PS_2  对应的 分频值 = (1<<FTM_PS_2) = (1<<1) = 2

/*********************** 输入捕捉 **************************/
//FTM 输入捕捉配置
typedef enum
{
    FTM_Rising,               //上升沿捕捉
    FTM_Falling,              //下降沿捕捉
    FTM_Rising_or_Falling     //跳变沿捕捉
} FTM_Input_cfg;

#define FTMN     ((FTM_MemMapPtr *) FTMN_addr )


#define FTM_IRQ_EN(FTMn,CHn)        FTM_CnSC_REG(FTMN[FTMn],CHn) |= FTM_CnSC_CHIE_MASK       //开启 FTMn_CHn 中断
#define FTM_IRQ_DIS(FTMn,CHn)       FTM_CnSC_REG(FTMN[FTMn],CHn) &= ~FTM_CnSC_CHIE_MASK      //关闭 FTMn_CHn 中断


//定义PIT模块号
typedef enum
{
    PIT0,
    PIT1,
    PIT2,
    PIT3,

    PIT_MAX,
} PITn_e;

#define     PIT_Flag_Clear(PITn_e)          PIT_TFLG(PITn_e)|=PIT_TFLG_TIF_MASK        //清中断标志（写1 清空标志位）

//ms是毫秒=0.001秒 us是微秒=0.000001秒 ns是纳秒=0.000000001秒
#define     pit_clk_khz         bus_clk_khz
#define     pit_init_ms(PITn_e,ms)          pit_init(PITn_e,ms * pit_clk_khz);         //初始化PITn，并设置定时时间(单位为 ms)
#define     pit_init_us(PITn_e,us)          pit_init(PITn_e,us * pit_clk_khz/1000);    //初始化PITn，并设置定时时间(单位为 us)
#define     pit_init_ns(PITn_e,ns)          pit_init(PITn_e,ns * pit_clk_khz/1000000); //初始化PITn，并设置定时时间(单位为 ns)

#define     pit_delay_ms(PITn_e,ms)          pit_delay(PITn_e,ms * pit_clk_khz);        //PIT延时 ms
#define     pit_delay_us(PITn_e,us)          pit_delay(PITn_e,us * pit_clk_khz/1000);   //PIT延时 us
#define     pit_delay_ns(PITn_e,ns)          pit_delay(PITn_e,ns * pit_clk_khz/1000000);//PIT延时 ns

#define     pit_time_get_ms(PITn_e)   (pit_time_get(PITn_e)/pit_clk_khz)                    //获取计时时间（单位为 ms）
#define     pit_time_get_us(PITn_e)   (pit_time_get(PITn_e)/(pit_clk_khz/1000))             //获取计时时间（单位为 us）

/**
 *  @brief LPTMR脉冲计数输入管脚选项
 */
typedef enum
{
    //只有1、2管脚，并没有0、3管脚
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;

/**
 *  @brief LPTMR脉冲计数触发方式
 */
typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //上升沿触发
    LPT_Falling = 1     //下降沿触发
} LPT_CFG;

#define LPTMR_Flag_Clear()  (LPTMR0_CSR |= LPTMR_CSR_TCF_MASK)         //清除LPT比较标志位

typedef enum DACn       //DAC模块
{
    DAC0,
    DAC1
} DACn_e;


    //MCG 超频函数                                    ************************************************
//超频配置
typedef uint16_t    (*pll_init_t)        (PLL_e pll,uint16_t max_bus_mhz,uint16_t max_flexbus_mhz,uint16_t max_flash_mhz);
#define pll_init(pll)                             ENTER_FUN_ADDR(pll_init,pll,MAX_BUS_CLK,MAX_FLEXBUS_CLK,MAX_FLASH_CLK)

    //dwt 延时函数                                    ************************************************
//dwt延时函数，最终用 DWT_DELAY_US(us) 来代替
typedef void      (*dwt_delay_us_t)        (uint32_t core_clk,uint32_t us);
#define dwt_delay_us(...)                         ENTER_FUN_ADDR(dwt_delay_us,core_clk_khz,##__VA_ARGS__)


//dwt延时函数，最终用 DWT_DELAY_MS(ms) 来代替
typedef void      (*dwt_delay_ms_t)        (uint32_t core_clk,uint32_t ms);
#define dwt_delay_ms(...)                         ENTER_FUN_ADDR(dwt_delay_ms,core_clk_khz,##__VA_ARGS__)



    //SYSTICK 延时函数                                ************************************************
#define     SYSTICK_CLK_KHZ                 core_clk_khz
//延时 time 个core周期，24位变量
typedef void      (*systick_delay_t)       (uint32_t time);
#define systick_delay(...)                        ENTER_FUN_ADDR(systick_delay,##__VA_ARGS__)



//延时 time 毫米。最终会以宏定义 SYSTICK_DELAY_MS(ms) 替换
typedef void      (*systick_delay_ms_t)    (uint32_t ms,uint32_t core_khz);
#define     systick_delay_ms(ms)                     ENTER_FUN_ADDR(systick_delay_ms,ms,SYSTICK_CLK_KHZ)
#define     systick_delay_us(us)          systick_delay(us * SYSTICK_CLK_KHZ/1000);   //延时 us
#define     systick_delay_ns(ns)          systick_delay(ns * SYSTICK_CLK_KHZ/1000000);//延时 ns


    //SYSTICK 定时函数                                ************************************************
//定时中断初始化函数， time个core周期，24bit
typedef void      (*systick_timing_t)      (uint32_t time);
#define systick_timing(...)                       ENTER_FUN_ADDR(systick_timing,##__VA_ARGS__)

#define systick_timing_ms(ms)               systick_timing(ms * SYSTICK_CLK_KHZ);        //定时 us
#define systick_timing_us(us)               systick_timing(us * SYSTICK_CLK_KHZ/1000);   //定时 us
#define systick_timing_ns(ns)               systick_timing(ns * SYSTICK_CLK_KHZ/1000000);//定时 ns



    //执行新APP API接口 （和普通的 函数调用不同，下面的函数都重置栈地址）      ************************************************
//跳转到新的APP
typedef void      (*jmp_app_t)             (uint32_t sp, uint32_t pc);
#define jmp_app(...)                              ENTER_FUN_ADDR(jmp_app,##__VA_ARGS__)


//跳转到新的APP BIN文件 （bin文件的第1个32位数据是栈地址，第2个32位数据是启动函数入口）
typedef void      (*jmp_bin_t)             (uint32_t addr);
#define jmp_bin(...)                              ENTER_FUN_ADDR(jmp_bin,##__VA_ARGS__)








    //常用小功能                                       ************************************************
//内存复制
typedef void *    (*memcpy_t)  ( uint8_t *dst, uint8_t *src, uint32_t count);
#define memcpy(...)                               ENTER_FUN_ADDR(memcpy,##__VA_ARGS__)


//内存设置数值
typedef void *    (*memset_t)  (void *src, int c, int count);
#define memset(...)                               ENTER_FUN_ADDR(memset,##__VA_ARGS__)

//求字符串的长度
typedef int       (*strlen_t)(const char * str);
#define strlen(...)                               ENTER_FUN_ADDR0(strlen,##__VA_ARGS__)


//摄像头解压
typedef void      (*img_extract_t)(void *dst, void *src, uint32_t srclen);
#define img_extract(...)                          ENTER_FUN_ADDR(img_extract,##__VA_ARGS__)



//获取中断号所对应的具体意义
typedef int       (*get_isr_info_t)    (char * str);
#define get_isr_info(...)                         ENTER_FUN_ADDR(get_isr_info,##__VA_ARGS__)




    //字符串 变量 转换                                   ************************************************
//字符串格式化到缓冲区
typedef int       (*sprintf_t)         (char *buf, const char *fmt, ...) ;
#define sprintf(...)                              ENTER_FUN_ADDR(sprintf,##__VA_ARGS__)


//printf，打印函数，需要自己实现 enter_fputc 函数
extern int enter_fputc(char ch);                    //需要用户自定义实现此函数
typedef int fputc_t (char c);
typedef int (*printf_t)(fputc_t *fp, const char *fmt, ...);
#define  PRINTF_ENTER_FUN_ADDR(a,b,c,...)                 PENTER_FUN_ADDR(a,b,c,##__VA_ARGS__)
#define printf(fmt,...)                               PRINTF_ENTER_FUN_ADDR(printf,enter_fputc,fmt,##__VA_ARGS__)




//字符串转成整型
typedef int       (*atoi_t)            (char *str);
#define atoi(...)                                 ENTER_FUN_ADDR(atoi,##__VA_ARGS__)


//整型转换为字符串
typedef char *    (*itoa_t)            (int n, char * chBuffer);
#define itoa(...)                                 ENTER_FUN_ADDR(itoa,##__VA_ARGS__)


//浮点转换为字符串
typedef char *    (*ftoa_t)            (double dValue, char * chBuffer);
#define ftoa(...)                                 ENTER_FUN_ADDR(ftoa,##__VA_ARGS__)




// ****************** FLASH API 专区 ******************

//初始化Flash
typedef void      (*flash_init_t)          ();
#define flash_init(...)                           ENTER_FUN_ADDR(flash_init,##__VA_ARGS__)


//擦除指定flash扇区
typedef uint8_t   (*flash_erase_sector_t)  (uint16_t sectorNum);
#define flash_erase_sector(...)                   ENTER_FUN_ADDR(flash_erase_sector,##__VA_ARGS__)


//写入flash操作
typedef uint8_t   (*flash_write_t)         (uint16_t sectorNum, uint16_t offset, FLASH_WRITE_TYPE data);
#define flash_write(...)                          ENTER_FUN_ADDR(flash_write,##__VA_ARGS__)


//从缓存区写入flash操作
typedef uint8_t   (*flash_write_buf_t)     (uint16_t sectorNum, uint16_t offset, uint16_t cnt, uint8 buf[]);
#define flash_write_buf(...)                      ENTER_FUN_ADDR(flash_write_buf,##__VA_ARGS__)



#define     flash_off(sectorNo,offset)              ((uint32)(((sectorNo)*FLASH_SECTOR_SIZE) + (offset)))
#define     flash_read(sectorNo,offset,type)        (*(type *)flash_off(sectorNo,offset) )         //读取扇区

    //flash 存储数据                                  ************************************************
//对结构体数据进行预处理，若flash的数据还没初始化，则重置flash
typedef void      (*flash_data_init_t)     (flash_data_t *d);
#define flash_data_init(...)                      ENTER_FUN_ADDR(flash_data_init,##__VA_ARGS__)


//重置flash，就是对存储数据进行清空
typedef void      (*flash_data_reset_t)    (flash_data_t *d);
#define flash_data_reset(...)                     ENTER_FUN_ADDR(flash_data_reset,##__VA_ARGS__)


//从 flash 加载数据
typedef uint8_t   (*flash_data_load_t)     (flash_data_t *d);
#define flash_data_load(...)                      ENTER_FUN_ADDR(flash_data_load,##__VA_ARGS__)


//保存数据到flash
typedef uint8_t   (*flash_data_save_t)     (flash_data_t *d);
#define flash_data_save(...)                      ENTER_FUN_ADDR(flash_data_save,##__VA_ARGS__)








    //PIT. 定时函数                                   ************************************************
//初始化PITn，并设置定时时间(单位为bus时钟周期)
typedef void      (*pit_init_t)            (PITn_e, uint32_t cnt);
#define pit_init(...)                             ENTER_FUN_ADDR(pit_init,##__VA_ARGS__)



    //PIT. 延时函数                                   ************************************************
//PIT延时（不需要初始化的）
typedef void     (*pit_delay_t)           (PITn_e, uint32_t cnt);
#define pit_delay(...)                            ENTER_FUN_ADDR(pit_delay,##__VA_ARGS__)



    //PIT. 计时函数                                   ************************************************
//PIT开始计时
typedef void      (*pit_time_start_t)      (PITn_e);
#define pit_time_start(...)                       ENTER_FUN_ADDR(pit_time_start,##__VA_ARGS__)


//获取 PITn计时时间(超时时会关闭 定时器)（单位为 bus时钟）(若值为 0xFFFFFFFF，则表示溢出)
typedef uint32_t  (*pit_time_get_t)        (PITn_e);
#define pit_time_get(...)                         ENTER_FUN_ADDR(pit_time_get,##__VA_ARGS__)


//关闭 PIT 计时
typedef void      (*pit_close_t)           (PITn_e);
#define pit_close(...)                            ENTER_FUN_ADDR(pit_close,##__VA_ARGS__)




    //LPT  延时函数                                   ************************************************
//延时(ms)
typedef void      (*lptmr_delay_ms_t)       (uint16_t ms);
#define lptmr_delay_ms(...)                       ENTER_FUN_ADDR(lptmr_delay_ms,##__VA_ARGS__)


//延时(us)
typedef void      (*lptmr_delay_us_t)       (uint16_t us);
#define lptmr_delay_us(...)                       ENTER_FUN_ADDR(lptmr_delay_us,##__VA_ARGS__)



    //LPT  定时中断函数                                 ************************************************
//定时(ms)
typedef void      (*lptmr_timing_ms_t)      (uint16_t ms);
#define lptmr_timing_ms(...)                      ENTER_FUN_ADDR(lptmr_timing_ms,##__VA_ARGS__)


//定时(us)
typedef void      (*lptmr_timing_us_t)      (uint16_t us);
#define lptmr_timing_us(...)                      ENTER_FUN_ADDR(lptmr_timing_us,##__VA_ARGS__)



    //LPT  计时函数                                   ************************************************
//开始计时(ms)
typedef void      (*lptmr_time_start_ms_t)  (void);
#define lptmr_time_start_ms(...)                  ENTER_FUN_ADDR(lptmr_time_start_ms,##__VA_ARGS__)


//获取计时时间
typedef uint32_t  (*lptmr_time_get_ms_t)    (void);
#define lptmr_time_get_ms(...)                    ENTER_FUN_ADDR(lptmr_time_get_ms,##__VA_ARGS__)


//开始计时(ns)
typedef void      (*lptmr_time_start_us_t)  (void);
#define lptmr_time_start_us(...)                  ENTER_FUN_ADDR(lptmr_time_start_us,##__VA_ARGS__)


//获取计时时间
typedef uint32_t  (*lptmr_time_get_us_t)    (void);
#define lptmr_time_get_us(...)                    ENTER_FUN_ADDR(lptmr_time_get_us,##__VA_ARGS__)


//关闭计时器
typedef void      (*lptmr_time_close_t)     ();
#define lptmr_time_close(...)                     ENTER_FUN_ADDR(lptmr_time_close,##__VA_ARGS__)



    //LPT  脉冲计数函数                                 ************************************************
//脉冲计数器初始化设置
typedef void      (*lptmr_pulse_init_t)     (LPT0_ALTn, uint16_t count, LPT_CFG);
#define lptmr_pulse_init(...)                     ENTER_FUN_ADDR(lptmr_pulse_init,##__VA_ARGS__)


//获取计数值
typedef uint16_t  (*lptmr_pulse_get_t)      (void);
#define lptmr_pulse_get(...)                      ENTER_FUN_ADDR(lptmr_pulse_get,##__VA_ARGS__)


//清空计数值
typedef void      (*lptmr_pulse_clean_t)    (void);
#define lptmr_pulse_clean(...)                    ENTER_FUN_ADDR(lptmr_pulse_clean,##__VA_ARGS__)




    //TPM  PWM输出                                  ************************************************
//初始化TPM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个TPM，各通道的PWM频率是一样的
typedef int8_t    (*tpm_pwm_init_t)        (TPMn_e, TPM_CHn_e, uint32_t freq, float duty,PTXn_e pin,uint32_t core_khz);
#define tpm_pwm_init(tpn,ch,freq,duty,pin)                         ENTER_FUN_ADDR(tpm_pwm_init,tpn,ch,freq,duty,pin,core_clk_khz)


//设置通道占空比,占空比为 duty%
typedef void      (*tpm_pwm_duty_t)        (TPMn_e, TPM_CHn_e,                float duty);
#define tpm_pwm_duty(...)                         ENTER_FUN_ADDR(tpm_pwm_duty,##__VA_ARGS__)


//设置TPM的频率（改频率后，需要重新配置占空比）
typedef int8_t    (*tpm_pwm_freq_t)        (TPMn_e,            uint32_t freq, uint32_t core_khz);
#define tpm_pwm_freq(tpn,freq)                         ENTER_FUN_ADDR(tpm_pwm_freq,tpn,freq,core_clk_khz)



    //TPM  输入捕捉                                   ************************************************
//输入捕捉初始化
typedef void      (*tpm_input_init_t)      (TPMn_e, TPM_CHn_e, TPM_Input_cfg cfg,TPM_PS_e ps,PTXn_e pin);
#define tpm_input_init(...)                       ENTER_FUN_ADDR(tpm_input_init,##__VA_ARGS__)


//输入捕捉模式下，读取捕捉事件发生时的计数器值(根据两次捕捉之间的差值可判断周期频率)
typedef uint16_t  (*tpm_input_get_t)       (TPMn_e, TPM_CHn_e);
#define tpm_input_get(...)                        ENTER_FUN_ADDR(tpm_input_get,##__VA_ARGS__)


//清空计数器的值
typedef void      (*tpm_input_clean_t)     (TPMn_e );
#define tpm_input_clean(...)                      ENTER_FUN_ADDR(tpm_input_clean,##__VA_ARGS__)



    //TPM  脉冲计数                                   ************************************************
//脉冲计数初始化
typedef void      (*tpm_pulse_init_t)      (TPMn_e tpmn,TPM_CLKIN_e clkin,TPM_PS_e ps,PTXn_e pin);
#define tpm_pulse_init(...)                       ENTER_FUN_ADDR(tpm_pulse_init,##__VA_ARGS__)


//获取脉冲数
typedef uint16_t  (*tpm_pulse_get_t)       (TPMn_e tpmn);
#define tpm_pulse_get(...)                        ENTER_FUN_ADDR(tpm_pulse_get,##__VA_ARGS__)


//清脉冲计数
typedef void      (*tpm_pulse_clean_t)     (TPMn_e tpmn);
#define tpm_pulse_clean(...)                      ENTER_FUN_ADDR(tpm_pulse_clean,##__VA_ARGS__)



    //FTM  PWM输出                                  ************************************************
//初始化FTM的PWM功能并设置频率、占空比。设置通道输出占空比。同一个FTM，各通道的PWM频率是一样的，共3个FTM(FX有4个)
typedef int8_t    (*ftm_pwm_init_t)        (FTMn_e, FTM_CHn_e, uint32_t freq, float duty,PTXn_e pin,uint32_t bus_khz);
#define ftm_pwm_init(ftm,ch,freq,duty,pin)                         ENTER_FUN_ADDR(ftm_pwm_init,ftm,ch,freq,duty,pin,bus_clk_khz)


//设置通道占空比,占空比为 （duty * 精度） % ，如果 FTM_PRECISON 定义为 1000 ，duty = 100 ，则占空比 100*0.1%=10%
typedef void      (*ftm_pwm_duty_t)        (FTMn_e, FTM_CHn_e,              float duty);
#define ftm_pwm_duty(...)                         ENTER_FUN_ADDR(ftm_pwm_duty,##__VA_ARGS__)


//设置FTM的频率（改频率后，需要重新配置占空比）
typedef int8_t    (*ftm_pwm_freq_t)        (FTMn_e,            uint32_t freq,uint32_t bus_khz);
#define ftm_pwm_freq(ftm,freq)                         ENTER_FUN_ADDR(ftm_pwm_freq,ftm,freq,bus_clk_khz)



    //FTM  输入捕捉                                   ************************************************
//输入捕捉初始化函数
typedef void      (*ftm_input_init_t)      (FTMn_e, FTM_CHn_e, FTM_Input_cfg,FTM_PS_e ps,PTXn_e pin);
#define ftm_input_init(...)                       ENTER_FUN_ADDR(ftm_input_init,##__VA_ARGS__)


//输入捕捉模式下，读取捕捉事件发生时的计数器值(根据两次捕捉之间的差值可判断周期频率)
typedef uint16_t  (*ftm_input_get_t)       (FTMn_e, FTM_CHn_e);
#define ftm_input_get(...)                        ENTER_FUN_ADDR(ftm_input_get,##__VA_ARGS__)


//清空计数器的值
typedef void      (*ftm_input_clean_t)     (FTMn_e );
#define ftm_input_clean(...)                      ENTER_FUN_ADDR(ftm_input_clean,##__VA_ARGS__)



    //FTM  正交解码                                   ************************************************
//初始化FTM 的正交解码 功能
typedef void      (*ftm_quad_init_t)       (FTMn_e, FTM_PS_e ps,PTXn_e Apin,PTXn_e Bpin);
#define ftm_quad_init(...)                        ENTER_FUN_ADDR(ftm_quad_init,##__VA_ARGS__)


//获取FTM 正交解码 的脉冲数(负数表示反方向)
typedef int16_t   (*ftm_quad_get_t)        (FTMn_e);
#define ftm_quad_get(...)                         ENTER_FUN_ADDR(ftm_quad_get,##__VA_ARGS__)


//清 FTM 正交解码 的脉冲数
typedef void      (*ftm_quad_clean_t)      (FTMn_e);
#define ftm_quad_clean(...)                       ENTER_FUN_ADDR(ftm_quad_clean,##__VA_ARGS__)



    //DAC 模拟输出                                    ************************************************
//DAC一次转换初始化
typedef void      (*dac_init_t)            (DACn_e);
#define dac_init(...)                             ENTER_FUN_ADDR(dac_init,##__VA_ARGS__)


//DAC一次转换操作
typedef void      (*dac_out_t)             (DACn_e, uint16_t val);
#define dac_out(...)                              ENTER_FUN_ADDR(dac_out,##__VA_ARGS__)



    //LCD 显示                                      ************************************************
typedef void      (*lcd_init_t)            (lcd_t * lcd,uint16_t rgb565);
#define lcd_init(...)                             ENTER_FUN_ADDR(lcd_init,glcd,##__VA_ARGS__)



//画点
typedef void      (*lcd_point_t)           (lcd_t * lcd,Site_t,                            uint16_t rgb565);
#define lcd_point(...)                            ENTER_FUN_ADDR(lcd_point,glcd,##__VA_ARGS__)


//画一堆点
typedef void      (*lcd_points_t)          (lcd_t * lcd,Site_t *site,  uint32_t point_num, uint16_t rgb565);
#define lcd_points(...)                           ENTER_FUN(lcd_points,glcd,##__VA_ARGS__)



//画矩形
typedef void      (*lcd_rectangle_t)       (lcd_t * lcd,Site_t,Size_t,                     uint16_t rgb565);
#define lcd_rectangle(...)                        ENTER_FUN_ADDR(lcd_rectangle,glcd,##__VA_ARGS__)


//画十字形
typedef void      (*lcd_cross_t)           (lcd_t * lcd,Site_t,uint16_t len,               uint16_t rgb565);
#define lcd_cross(...)                            ENTER_FUN_ADDR(lcd_cross,glcd,##__VA_ARGS__)


//清屏
typedef void      (*lcd_clear_t)           (lcd_t * lcd,uint16_t rgb565);
#define lcd_clear(...)                            ENTER_FUN_ADDR(lcd_clear,glcd,##__VA_ARGS__)




    //LCD 英文字符、数字显示                               ************************************************
//显示英文字符
typedef void      (*lcd_char_6x12_t)       (lcd_t * lcd,Site_t site, uint8 ascii, uint16_t Color, uint16_t bkColor) ;
#define lcd_char_6x12(...)                        ENTER_FUN_ADDR(lcd_char_6x12,glcd,##__VA_ARGS__)


//显示英文字符串
typedef void      (*lcd_str_6x12_t)        (lcd_t * lcd,Site_t site, uint8 *Str,  uint16_t Color, uint16_t bkColor);
#define lcd_str_6x12(...)                         ENTER_FUN_ADDR(lcd_str_6x12,glcd,##__VA_ARGS__)


//显示无符号数字
typedef uint8_t   (*lcd_num_6x12_t)        (lcd_t * lcd,Site_t site, uint32_t num,uint16_t Color, uint16_t bkColor);
#define lcd_num_6x12(...)                         ENTER_FUN_ADDR(lcd_num_6x12,glcd,##__VA_ARGS__)


//显示有符号数字
typedef uint8_t   (*lcd_snum_6x12_t)       (lcd_t * lcd,Site_t site, int32 num,   uint16_t Color, uint16_t bkColor);
#define lcd_snum_6x12(...)                        ENTER_FUN_ADDR(lcd_snum_6x12,glcd,##__VA_ARGS__)


//显示无符号数字(擦除后面数据)
typedef void      (*lcd_num_bc_6x12_t)     (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);
#define lcd_num_bc_6x12(...)                      ENTER_FUN_ADDR(lcd_num_bc_6x12,glcd,##__VA_ARGS__)


//显示有符号数字(擦除后面数据)
typedef void      (*lcd_snum_bc_6x12_t)    (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);
#define lcd_snum_bc_6x12(...)                     ENTER_FUN_ADDR(lcd_snum_bc_6x12,glcd,##__VA_ARGS__)




    //LCD 汉字显示                                    ************************************************
#if (USE_FOUNT_SIZE == 12)
//显示汉字字符
typedef void      (*lcd_char_ch_12x12_t)   (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor);
//显示汉字字符串
typedef void      (*lcd_str_ch_12x12_t)    (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor);
//显示汉字英文混合字符串
typedef void      (*lcd_str_ench_12x12_t)  (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor);

#else
//显示汉字字符          （若 USE_FOUNT_SIZE 为12，最后一个参数不需要写）
typedef void      (*lcd_char_ch_12x12_t)   (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_12x12);
//显示汉字字符串        （若 USE_FOUNT_SIZE 为12，最后一个参数不需要写）
typedef void      (*lcd_str_ch_12x12_t)    (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_12x12);
//显示汉字英文混合字符串 （若 USE_FOUNT_SIZE 为12，最后一个参数不需要写）
typedef void      (*lcd_str_ench_12x12_t)  (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_12x12);
#endif

#define lcd_char_ch_12x12(...)                    ENTER_FUN(lcd_char_ch_12x12,glcd,##__VA_ARGS__)
#define lcd_str_ch_12x12(...)                     ENTER_FUN(lcd_str_ch_12x12,glcd,##__VA_ARGS__)
#define lcd_str_ench_12x12(...)                   ENTER_FUN(lcd_str_ench_12x12,glcd,##__VA_ARGS__)



//显示英文字符
typedef void      (*lcd_char_8x16_t)       (lcd_t * lcd,Site_t site, uint8 ascii, uint16_t Color, uint16_t bkColor);
#define lcd_char_8x16(...)                        ENTER_FUN_ADDR(lcd_char_8x16,glcd,##__VA_ARGS__)


//显示英文字符串
typedef void      (*lcd_str_8x16_t)        (lcd_t * lcd,Site_t site, uint8 *Str,  uint16_t Color, uint16_t bkColor);
#define lcd_str_8x16(...)                         ENTER_FUN_ADDR(lcd_str_8x16,glcd,##__VA_ARGS__)


//显示无符号数字
typedef uint8_t   (*lcd_num_8x16_t)        (lcd_t * lcd,Site_t site, uint32_t num,uint16_t Color, uint16_t bkColor) ;
#define lcd_num_8x16(...)                         ENTER_FUN_ADDR(lcd_num_8x16,glcd,##__VA_ARGS__)


//显示有符号数字
typedef uint8_t   (*lcd_snum_8x16_t)       (lcd_t * lcd,Site_t site, int32 num,   uint16_t Color, uint16_t bkColor) ;
#define lcd_snum_8x16(...)                        ENTER_FUN_ADDR(lcd_snum_8x16,glcd,##__VA_ARGS__)


//显示无符号数字(擦除后面数据)
typedef void      (*lcd_num_bc_8x16_t)     (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);
#define lcd_num_bc_8x16(...)                      ENTER_FUN_ADDR(lcd_num_bc_8x16,glcd,##__VA_ARGS__)


//显示有符号数字(擦除后面数据)
typedef void      (*lcd_snum_bc_8x16_t)    (lcd_t * lcd,Site_t site, uint32 num, uint8_t max_num_bit,uint16 Color, uint16 bkColor);
#define lcd_snum_bc_8x16(...)                     ENTER_FUN_ADDR(lcd_snum_bc_8x16,glcd,##__VA_ARGS__)





#if (USE_FOUNT_SIZE == 16)
//显示汉字字符
typedef void      (*lcd_char_ch_16x16_t)   (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor);
//显示汉字字符串
typedef void      (*lcd_str_ch_16x16_t)    (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor);
//显示汉字英文混合字符串
typedef void      (*lcd_str_ench_16x16_t)  (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor);
#else
//显示汉字字符          （若 USE_FOUNT_SIZE 为16，最后一个参数不需要写）
typedef void      (*lcd_char_ch_16x16_t)   (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_16x16);
//显示汉字字符串        （若 USE_FOUNT_SIZE 为16，最后一个参数不需要写）
typedef void      (*lcd_str_ch_16x16_t)    (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_16x16);
//显示汉字英文混合字符串 （若 USE_FOUNT_SIZE 为16，最后一个参数不需要写）
typedef void      (*lcd_str_ench_16x16_t)  (lcd_t * lcd,Site_t site, const uint8 *str, uint16_t Color, uint16_t bkColor,char * chinese_16x16);
#endif


#define lcd_char_ch_16x16(...)                    ENTER_FUN(lcd_char_ch_16x16,glcd,##__VA_ARGS__)
#define lcd_str_ch_16x16(...)                     ENTER_FUN(lcd_str_ch_16x16,glcd,##__VA_ARGS__)
#define lcd_str_ench_16x16(...)                   ENTER_FUN(lcd_str_ench_16x16,glcd,##__VA_ARGS__)



    //LCD 图像显示                                    ************************************************
//显示二值化图像
typedef void      (*lcd_img_binary_t)      (lcd_t * lcd,Site_t site, Size_t size, uint8 *img,                uint16_t Color, uint16_t bkColor);
#define lcd_img_binary(...)                       ENTER_FUN_ADDR(lcd_img_binary,glcd,##__VA_ARGS__)


//显示二值化图像(可缩放)
typedef void      (*lcd_img_binary_z_t)    (lcd_t * lcd,Site_t site, Size_t size, uint8 *img, Size_t imgsize,uint16_t Color, uint16_t bkColor);
#define lcd_img_binary_z(...)                     ENTER_FUN_ADDR(lcd_img_binary_z,glcd,##__VA_ARGS__)


//显示灰度图像
typedef void      (*lcd_img_gray_t)        (lcd_t * lcd,Site_t site, Size_t size, uint8 *img);
#define lcd_img_gray(...)                         ENTER_FUN_ADDR(lcd_img_gray,glcd,##__VA_ARGS__)


//显示灰度图像(可缩放)
typedef void      (*lcd_img_gray_z_t)      (lcd_t * lcd,Site_t site, Size_t size, uint8 *img, Size_t imgsize);
#define lcd_img_gray_z(...)                       ENTER_FUN_ADDR(lcd_img_gray_z,glcd,##__VA_ARGS__)


//彩色
typedef void      (*lcd_img_rgb565_t)      (lcd_t * lcd,Site_t site, Size_t size, uint8 *img) ;
#define lcd_img_rgb565(...)                       ENTER_FUN_ADDR(lcd_img_rgb565,glcd,##__VA_ARGS__)


//彩色(可缩放)
typedef void      (*lcd_img_rgb565_z_t)    (lcd_t * lcd,Site_t site, Size_t size, uint8 *img, Size_t imgsize);
#define lcd_img_rgb565_z(...)                     ENTER_FUN_ADDR(lcd_img_rgb565_z,glcd,##__VA_ARGS__)



    //LCD 波形显示                                    ************************************************
//波形显示，带背景颜色（maxval 最大值表示超过此值的数都当作最大值处理）
typedef void      (*lcd_wave_t)            (lcd_t * lcd,Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16_t Color ,uint16_t bkColor);
#define lcd_wave(...)                             ENTER_FUN_ADDR(lcd_wave,glcd,##__VA_ARGS__)


//波形显示，不带背景颜色
typedef void      (*lcd_wave_display_t)    (lcd_t * lcd,Site_t site,Size_t size,uint8 *img,uint8 maxval,uint16_t Color);
#define lcd_wave_display(...)                     ENTER_FUN_ADDR(lcd_wave_display,glcd,##__VA_ARGS__)



static inline char   enter_init(void)
{
    while(  ( (uint16_t)BOOT_ADDR(flag)!=0x55A5)  ||  (uint16_t)BOOT_ADDR(ver)!=BOOT_VER  )
    {
        //卡是在这死循环，请用UD升级工具，重新更新数据
        printf("\nBOOT版本数据异常，标志位:0x%x，版本号：0x%x", (uint16_t)BOOT_ADDR(flag),(uint16_t)BOOT_ADDR(ver)) ;

    }

    while(  ( (uint16_t)IN_ADDR(flag)!=0x55A5)  ||  (uint16_t)IN_ADDR(ver)!=FWD_VER  )
    {
        //卡是在这死循环，请用UD下载，更新EXT FWD
        printf("\nEXT FWD版本数据异常，标志位:0x%x，版本号：0x%x", (uint16_t)IN_ADDR(flag),(uint16_t)IN_ADDR(ver)) ;
    }
    return 1;

}


#endif    //

#endif   //_ENTER_H_

