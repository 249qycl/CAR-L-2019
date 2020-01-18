#include "common.h"
#include "include.h"

pCar Ecc_Car; //创建车体
/***********************************************
*
*       对用于描述车体信息的变量进行初始化创建
*
***********************************************/
pCar Car_create()
{
    pCar P = (pCar)malloc(sizeof(Car));

    P->Flag.protect = 0;
    P->Flag.outside = 0;
    P->Flag.calibrat = 0;
    P->Flag.laser1 = 0;
    P->Flag.laser2 = 0;
    P->Flag.obstacle = 0;
    P->Flag.buzzer = 0;
    P->Flag.stop = 0;
    P->Flag.hoare = 0;

    P->Duty.servo = 0;
    P->Duty.motor = 0;
    P->Duty.debug = 1;

    P->Mode.move = 0;
    P->Mode.servo = 0;
    P->Mode.motor = 0;

    P->Error.now = 0;
    P->Error.last = 0;
    P->Error.delta = 0;

    P->Protect.round = 0;
    P->Protect.obstacle = 0;

    P->Info.threshold = 100;

    P->Speed.now = 0;
    P->Speed.standard = 200; //300
    P->Speed.round = 200;
    P->Speed.obstacle = 200;
    P->Speed.outside = 200;

    P->Duty.obstacle[0] = 8000;
    P->Duty.obstacle[1] = -3000;
    P->Duty.obstacle[2] = -10000;

	P->Scan.finish=0;
	P->Scan.times=SCAN_NUM;
	
    P->Round.state = 0;
    P->Round.threshold = 420;
    P->Round.ad_sum = 0;

    P->Round.Debug.W[0] = 1; //2  0  2
    P->Round.Debug.Z[0] = 0; //[抗入环扭动]
    P->Round.Debug.N[0] = 1;

    P->Round.Debug.W[1] = 0;
    P->Round.Debug.Z[1] = 2; //进
    P->Round.Debug.N[1] = 0;

    P->Round.Debug.W[2] = 5;  //5//2
    P->Round.Debug.Z[2] = 10; //增强贴内道能力
    P->Round.Debug.N[2] = 1;  //1

    P->Round.Debug.W[3] = 1; //101
    P->Round.Debug.Z[3] = 0;
    P->Round.Debug.N[3] = 1;

    return P;
}

/******************************************************
*
*       读取编码器的返回值
*
******************************************************/
void Car_speed_get(pCar Car, uint8 T)
{
#define SPEED_NUM 5

    static int16 queue[SPEED_NUM] = {0};
    static uint8 rear = 0, sample_times = 0;
    int32 sum = 0;

    if (sample_times > T)
    {
        int16 val = ftm_quad_get(FTM1); //获取FTM 正交解码 的脉冲数(负数表示反方向)
        ftm_quad_clean(FTM1);           //清除计数，重新开始

        rear = (rear + 1) % SPEED_NUM;
        queue[rear] = val;
        for (uint8 i = SPEED_NUM; i > 0; i--)
        {
            sum += queue[(rear + i) % SPEED_NUM] * i; //(i+1)
        }
        Car->Speed.now = sum / 150; //递推均值滤波后把值缩小10倍[210]
        sample_times = 0;
    }
    sample_times++;

#undef SPEED_NUM
}

/*************************************************
*
*       定期更新历史方向偏差信息【对偏差求微分】
*       T
*
*************************************************/
void Error_update(uint8 T)
{
    static uint8 times = 0;
    if (times++ > T && Ecc_Car->Flag.outside == 0)
    {
        times = 0;
        Ecc_Car->Error.delta = Ecc_Car->Error.last - Ecc_Car->Error.now;
        Ecc_Car->Error.last = Ecc_Car->Error.now;
    }
}

/*****************************************************
*
*       初始化电机、舵机位置及相应驱动模块
*
*****************************************************/
void Car_init()
{
    Ecc_Car = Car_create();
    gpio_init(PTE10, GPO, 1);//蜂鸣器
    /*模块0的通道2  3*/
    ftm_pwm_init(FTM0, FTM_CH2, M_FREQ, 0.0f, FTM0_CH2_PIN); //电机初始化频率为10k ，占空比为 00.0%
    ftm_pwm_init(FTM0, FTM_CH3, M_FREQ, 0.0f, FTM0_CH3_PIN); //电机初始化频率为10k ，占空比为 00.0%
    /*FTM1 正交解码初始化*/
    ftm_quad_init(FTM1, FTM_PS_1, FTM1_QDPHA_PIN, FTM1_QDPHB_PIN);
    /*模块1的通道0*/
    ftm_pwm_init(FTM2, FTM_CH0, S_FREQ, M_VALUE, FTM2_CH0_PIN); //L车舵机初始化50Hz,处于正中位置
}

/************************************************************
*
*					蜂鸣器驱动
*
************************************************************/
void Buzzer()
{
    static uint8 times = 0;
    if (1 == Ecc_Car->Flag.buzzer && times < 120)
    {
        gpio_set(PTE10, 0);
        times++;
    }
    else
    {
        gpio_set(PTE10, 1);
        times = 0;
        Ecc_Car->Flag.buzzer = 0;
    }
/****************读取激光的状态**********************/
    if (0 == gpio_get(LASER_PORT1))
    {
        Ecc_Car->Flag.laser2 = 1;
        led(LED3, LED_ON);
    }
    else
    {
        led(LED3, LED_OFF);
        Ecc_Car->Flag.laser2 = 0;
    }

    if (0 == gpio_get(LASER_PORT2))
    {
        led(LED2, LED_ON);
        Ecc_Car->Flag.laser1 = 1;
    }
    else
    {
        led(LED2, LED_OFF);
        Ecc_Car->Flag.laser1 = 0;
    }
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

    switch (mode)
    {
    case 0:
        Servo_set(Ecc_Car->Duty.servo / 100);
        break; //执行器自动运转
    case 3:
        Servo_set(-100);
        break; //左打死
    case 4:
        Servo_set(100);
        break; //右打死
    case 5:
        break;

    default:
        Servo_set(Ecc_Car->Duty.servo / 100);
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
    duty = RANGE(duty, 1000, -1000);
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
}
/********************************************************
*
*       电机及相应的模式运动
*
********************************************************/
void Motor_move(uint8 mode)
{

    if (1 == Ecc_Car->Flag.stop)
        mode = 9;

    switch (mode)
    {
    case 0:
        Motor_set((float)Ecc_Car->Duty.motor / 100);
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
        Motor_set(Ecc_Car->Duty.motor);
        break; //执行器自动运转
    }
}
/*************************************************************
*
*						霍尔停车检测
*
*************************************************************/
void Hoare_process()
{
    static uint8 times = 0, re_judge = 0;
    if (1 == Ecc_Car->Flag.hoare && 1 == gpio_get(HOARE_PORT))
        times++;
    else
    {
        times = 0;
        Ecc_Car->Flag.hoare = 0;
    }
    if (times > 1 && 0 == re_judge)
    {
        Ecc_Car->Flag.buzzer = 1;
        Ecc_Car->Flag.stop = 1;
        re_judge = 1;
        times = 0;
    }
    if (0 == gpio_get(HOARE_PORT))
        re_judge = 0;
}
