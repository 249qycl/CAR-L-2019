#include "common.h"
#include "include.h"

//      DELAY_MS(100);                      
void main()
{
    /*****************中断初始化区************************/
    pit_init_ms(PIT0, 5); //定时器中断5ms
    ASSERT(enter_init());
    /*******************中断优先级设置区******************/
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);

    NVIC_SetPriority(UART3_RX_TX_IRQn, 0);
    NVIC_SetPriority(PIT0_IRQn, 1);
    /***************中断向量装填服务函数设置区********************/
    set_vector_handler(UART3_RX_TX_VECTORn, Uart_Handler);
    set_vector_handler(PIT0_VECTORn, PIT0_Handler);
    led_init(LED0);    //初始化 LED0【仅用作中断是否工作的判断】
    All_PID_init();    //PID数据初始化
    my_flash_init();   //flash存储数据初始化
    Car_init();        //车体初始化
    All_AD_init();     //AD采集初始化
    lcd_init(BLACK);   //屏幕初始化
    Boot_action();     //开机交互
    key_init(KEY_MAX); //初始化全部 按键
    Tree_init();       //关系树初始化

    /********************中断使能区***********************/
    enable_irq(PIT0_IRQn);     //开定时器中断
    uart_rx_irq_en(VCAN_PORT); //开串口接收中断

    while (1) //循环内与外界进行数据交换，程序容易卡死
    {
        if (Scan.finish_flag == 1)
        {
            Show_Handler;
            key_IRQHandler(); //按键扫描

            //#if   HOST_KEY==1
            //    Host_hander(10);
            //#elif HOST_KEY==2
            //    Host_Handler;
            //#endif
        }
    }
}