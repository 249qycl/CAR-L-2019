#include "common.h"
#include "include.h"

pCar Ecc_Car; //创建车体
ADJ param;    //调试参数集合
/***********************************************
*
*       对用于描述车体信息的变量进行初始化创建
*
***********************************************/
pCar Car_create()
{
    pCar P = (pCar)malloc(sizeof(Car));

    P->outside_flag = 0;
    P->M_Duty = 0;
    P->S_Duty = 0;
    P->mv_mode = 0;
    P->Servo_mode = 0;
    P->Motor_mode = 0;
    P->times = 0;
    P->Dir_Error = 0;
    P->Last_Error = 0;
    P->Delta_err = 0;
    P->DIR_k = 0;
    P->speed = 0;
    return P;
}

/******************************************************
*
*       读取编码器的返回值
*
******************************************************/
void Car_speed_get(pCar Q, uint8 frequency)
{
#define SPEED_NUM 5

    static int16 queue[SPEED_NUM] = {0};
    static uint8 rear = 0, sample_times = 0;
    int32 sum = 0;

    if (sample_times == frequency)
    {
        int16 val = ftm_quad_get(FTM1); //获取FTM 正交解码 的脉冲数(负数表示反方向)
        ftm_quad_clean(FTM1);           //清除计数，重新开始

        rear = (rear + 1) % SPEED_NUM;
        queue[rear] = val;
        for (uint8 i = SPEED_NUM; i > 0; i--)
        {    
            sum += queue[(rear + i) % SPEED_NUM] * (i + 1);
        }
        Q->speed = sum / 210; //递推均值滤波后把值缩小10倍
        sample_times = 0;
    }
    sample_times++;

#undef SPEED_NUM
}
/****************************************************
*
*               调试用参数初始化
*
****************************************************/
void Parameter_init()
{
    param = (ADJ){.Rt_k = 50,
                  .All_ad_sum = 0,
                  .IRt_k = 10, //25
                  .ORt_k = -20,
                  .URt_k = 3,
                  .S_duty = M_VALUE};
}
/*************************************************
*
*       定期更新历史方向偏差信息
*       frequency
*
*************************************************/
void Error_update(uint8 frequency)
{
    static uint8 times = 0;
    if (times++ > frequency && Ecc_Car->outside_flag == 0)
    {
        times = 0;
        Ecc_Car->Delta_err  = Ecc_Car->Last_Error - Ecc_Car->Dir_Error;
        Ecc_Car->Last_Error = Ecc_Car->Dir_Error;
    }
}
/***********************************************************
*
*               方向偏差变化的斜率
*
***********************************************************/
void DIR_error_slope()
{
    Ecc_Car->DIR_k = (int16)(Least_square_method2(Ecc_Car->Dir_Error, 0) * 100);
}
/*****************************************************
*
*       初始化电机、舵机位置及相应驱动模块
*
*****************************************************/
void Car_init()
{
    Ecc_Car = Car_create();
    Motor_PID->set_value = 300; //设定速度
    Parameter_init();
    Kalman_filter_init(&Err_Kalman, 0, 0, 0, 0);
    /*模块0的通道2  3*/
    ftm_pwm_init(FTM0, FTM_CH2, M_FRENT, 0.0f, FTM0_CH2_PIN); //电机初始化频率为10k ，占空比为 00.0%
    ftm_pwm_init(FTM0, FTM_CH3, M_FRENT, 0.0f, FTM0_CH3_PIN); //电机初始化频率为10k ，占空比为 00.0%
    /*FTM1 正交解码初始化*/
    ftm_quad_init(FTM1, FTM_PS_1, FTM1_QDPHA_PIN, FTM1_QDPHB_PIN);

    /*模块1的通道0*/

#if (CAR_MODEL == L_CAR)
    ftm_pwm_init(FTM2, FTM_CH0, S_FRENT, M_VALUE, FTM2_CH0_PIN); //L车舵机初始化50Hz,处于正中位置
#else
    ftm_pwm_init(FTM2, FTM_CH0, Hz_300, M_VALUE, FTM2_CH0_PIN); //B车舵机初始化300Hz
#endif
}

/****************************************************
*
*       设置舵机的位置
*       正为右打角，负为左打角
****************************************************/
void Servo_set(float duty) //-100~100
{
    duty = duty > 0 ? (duty / 100 * (R_VALUE - M_VALUE) + M_VALUE) : (duty / 100 * (M_VALUE - L_VALUE) + M_VALUE);
    duty = duty < L_VALUE ? L_VALUE : (duty > R_VALUE ? R_VALUE : duty);
    ftm_pwm_duty(FTM2, FTM_CH0, duty);
}
/*****************************************************
*
*       舵机相应模式运动
*
******************************************************/
void Servo_move(uint8 mode)
{
    static uint8 last_mode = 0; //历史状态【特定动作未执行完之前，均沿用历史状态】
    static uint16 times = 0;
    if (times == 0)
    {
        last_mode = mode;
        times += Ecc_Car->times;
        Ecc_Car->times = 0;
    }
    else
    {
        mode = last_mode;
        times--;
    }

    switch (mode)
    {
    case 0:
        Servo_set(Ecc_Car->S_Duty / 100);
        break; //执行器自动运转
    case 3:
        Servo_set(-100);
        break; //左打死
    case 4:
        Servo_set(100);
        break; //右打死
    default:
        Servo_set(Ecc_Car->S_Duty / 100);
        break;
    }
}
/*******************************************************
*
*       电机的占空比设置
*
*******************************************************/
void Motor_set(float duty)
{
    //#define SMOOTH  50        //平滑幅度前后两次不大于50
    //
    //    static float last_duty=0;//平滑输出
    //
    //    duty=duty>0?((duty-last_duty)>SMOOTH?(last_duty+SMOOTH):duty)
    //               :((duty-last_duty)>SMOOTH?(last_duty+SMOOTH):duty);

    Motor_PID->saturation_flag = fabs(duty) > 1000 ? 1 : 0;
    duty = RANGE(duty, 1000, -1000);
    //    last_duty = duty;
    duty /= 10;

    if (duty > 0)
    {
        ftm_pwm_duty(FTM0, FTM_CH2, duty); //设置占空比
        ftm_pwm_duty(FTM0, FTM_CH3, 0);
    }
    else
    {
        ftm_pwm_duty(FTM0, FTM_CH2, 0); //设置占空比
        ftm_pwm_duty(FTM0, FTM_CH3, -duty);
    }
    //#undef SMOOTH
}
/********************************************************
*
*       电机及相应的模式运动
*
********************************************************/
void Motor_move(uint8 mode)
{
    static uint8 last_mode = 0; //历史状态【特定动作未执行完之前，均沿用历史状态】
    static uint16 times = 0;
    if (times == 0)
    {
        last_mode = mode;
        times += Ecc_Car->times;
        Ecc_Car->times = 0;
    }
    else
    {
        mode = last_mode;
        times--;
    }

    if (Ecc_Car->outside_flag == 1)
        mode = 9;

    switch (mode)
    {
    case 0:
        Motor_set(Ecc_Car->M_Duty / 100);
        break; //执行器自动运转
    case 1:
        Motor_set(150);
        break; //进环岛速度
    case 3:
        Motor_set(0);
        break; //点刹
    case 4:
        Motor_set(1000);
        break; //全速
    case 9:
        Motor_set(0);
        break; //停车或刹车
    default:
        Motor_set(Ecc_Car->M_Duty);
        break; //执行器自动运转
    }
}
/************************************************************
*
*               特殊动作
*
************************************************************/
uint8 Special_action(uint8 dir, uint8 speed1, uint8 speed2, uint8 add_time, uint8 sub_time)
{
    static uint16 times1 = 0, times2 = 0;

    if (times1 == 0 && times2 == 0)
    {
        times1 += add_time;
        times2 += sub_time;
    }

    if (times1 > 0)
    {
        Ecc_Car->Servo_mode = dir;
        Ecc_Car->Motor_mode = speed1;
        times1--;
    }
    else if (times1 == 0 && times2 > 0)
    {
        Ecc_Car->Motor_mode = speed2;
        times2--;
    }

    if (times1 == 0 && times2 == 0) //执行完成
    {
        Ecc_Car->Servo_mode = 0;
        Ecc_Car->Motor_mode = 0;
        return 0;
    }
    else
        return 1;
}
/************************************************************
*
*               速度采集
*
************************************************************/

/************************************************************
*
*                       按偏差配速
*
************************************************************/
void Car_Speed_Target()
{
#define Error_L 30
#define Speed_K 5
#define TARGET Motor_PID->set_value

    int Error;
    Error = abs(Ecc_Car->Dir_Error);
    Error = RANGE(Error, 150, 0);
    if (Error <= Error_L) //低偏差时，全速运行
        TARGET = CAR_SPEED_HIGH;

    if (TARGET > 200)                                                                                       //只在高速时进行速度调节
        TARGET = CAR_SPEED_HIGH - ((CAR_SPEED_HIGH - CAR_SPEED_LOW) * (Error / Speed_K * Error / Speed_K)); //利用偏差进行非线性的速度调节

    if (TARGET < CAR_SPEED_LOW) //目标速度下限
        TARGET = CAR_SPEED_LOW;

#undef Error_L
#undef Speed_K
#undef TARGET
}
void Death_limit()
{
    if (abs(Ecc_Car->Dir_Error) < 20)
        Ecc_Car->Dir_Error = Ecc_Car->Dir_Error >= 0 ? 1 : -1;
}