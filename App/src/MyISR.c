#include "common.h"
#include "include.h"

void PIT0_Handler()
{
    static uint8 led_times = 0;
    PIT_Flag_Clear(PIT0);

    Car_speed_get(Ecc_Car, 20); //100ms采一次
    Time_Handler;
    if (led_times++ == 100)
    {
        led_turn(LED0);
        led_times = 0;
    }

    if (Scan.finish_flag == 0)
        AD_Scan(); //AD阈值扫描
    else
    {
//        AD_sliding_get();
//        Mean_Filter();
//        Ecc_Car->Dir_Error = Direct_error();
        //        Ecc_Car->Dir_Error=Kalman_filter(&Err_Kalman,Ecc_Car->Dir_Error);
        PID_Handler(4, 20);
        //
        //        Outside_judge(&Ecc_Car->Last_Error);
//        Error_update(20);
//        Servo_move(Ecc_Car->Servo_mode);
        //        Motor_move(Ecc_Car->Motor_mode);
//        Motor_set(200);
        //#if HOST_KEY
        //    Ecc_Car->Motor_mode=deb_control.switch_1==1?9:Ecc_Car->Motor_mode;
        //#endif

        //        TFT_SCOPE;
        //        Host_Handler;
		ftm_pwm_duty(FTM2, FTM_CH0,param.S_duty);
        FLAGS_UPDATE;
    }
}

//    Death_limit();
//    Bang_PID();//自动分配舵机PID参数【手动被禁止】
//    ftm_pwm_duty(FTM2, FTM_CH0,param.S_duty);
//    Motor_set(200);
//    DIR_error_slope();                                                       //方向偏差变化的斜率