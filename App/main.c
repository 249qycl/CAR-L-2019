#include "common.h"
#include "include.h"


void main()
{
    Vector_handler_init();		//中断配置	
    led_init(LED0);    			//初始化 LED0【仅用作中断是否工作的判断】
	led_init(LED3);    			//初始化 LED3
	led_init(LED2);    			//初始化 LED2
	Distance_init();		   	//测距避障初始化
    All_PID_init();    			//PID数据初始化
    my_flash_init();   			//flash存储数据初始化
    Car_init();        			//车体初始化
    All_AD_init();     			//AD采集初始化
    lcd_init(BLACK);   			//屏幕初始化
    Boot_action();     			//开机交互
    key_init(KEY_MAX); 			//初始化全部 按键
    Tree_init();       			//关系树初始化 
	Vector_enable_init();		//中断使能
    while (1)                  
    {
        if (1==Ecc_Car->Scan.finish )
        {
            Show_Handler;
            key_IRQHandler(); //按键扫描
			Distance_measure();
            //#if   HOST_KEY==1
            //    Host_hander(10);
            //#elif HOST_KEY==2
            //    Host_Handler;
            //#endif
        }
    }
}