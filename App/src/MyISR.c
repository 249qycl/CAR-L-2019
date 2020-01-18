#include "common.h"
#include "include.h"



void PIT0_Handler()
{
	static uint8 led_times = 0;
	PIT_Flag_Clear(PIT0);
	
	Car_speed_get(Ecc_Car, 20); //100ms采一次
		
	if (100==led_times++)
	{
		led_turn(LED0);
		led_times = 0;
	}
	
	if (0==Ecc_Car->Scan.finish)
		AD_Scan(); //AD阈值扫描
	else
	{
		AD_sliding_get();
		Mean_Filter();
		Ecc_Car->Error.now = Direct_error();
		if(0==Ecc_Car->Flag.obstacle)
			Outside_judge();
			
		Error_update(20);
		
		PID_Handler(4, 20);		
		
		if(0==Ecc_Car->Round.state)
			Obstacle_avoidance();
		
		Servo_move(Ecc_Car->Mode.servo);
		Motor_move(Ecc_Car->Mode.motor);
		
		Distance_send();
		Hoare_process();
		Buzzer();
		Flag_protection();
		//#if HOST_KEY
		//    Ecc_Car->Motor_mode=deb_control.switch_1==1?9:Ecc_Car->Motor_mode;
		//#endif
		
		//        TFT_SCOPE;
		//        Host_Handler;

		FLAGS_UPDATE;
	}
}
//    ftm_pwm_duty(FTM2, FTM_CH0,Ecc_Car->Duty.debug);
//    Motor_set(200);


void Vector_handler_init()
{
	/*****************中断初始化区************************/
	pit_init_ms(PIT0, 5); //定时器中断5ms
	ASSERT(enter_init());
	/*******************中断优先级设置区******************/
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
	NVIC_SetPriority(PORTE_IRQn,0);
	NVIC_SetPriority(UART3_RX_TX_IRQn, 1);
	NVIC_SetPriority(PIT0_IRQn, 2);
	/***************中断向量装填服务函数设置区********************/
	set_vector_handler(UART3_RX_TX_VECTORn, Uart_Handler);
	set_vector_handler(PIT0_VECTORn, PIT0_Handler);
	set_vector_handler(PORTE_VECTORn , PORTE_Handler);
}
void Vector_enable_init()
{
	/********************中断使能区***********************/
	enable_irq(PIT0_IRQn);     //开定时器中断
	uart_rx_irq_en(VCAN_PORT); //开串口接收中断
	enable_irq(PORTE_IRQn);	   //开外部中断
}