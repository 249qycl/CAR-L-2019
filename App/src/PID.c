#include "common.h"
#include "include.h"

pPID Servo_PID; //创建舵机PID参数体
pPID Motor_PID; //创建电机PID参数体

/************************************************************
*	作用：PID对象参数初始化
*	limit：积分分离的偏差界限
*	saturation：积分饱和值
*************************************************************/
pPID PID_create(int16 p, int16 i, int16 d)
{
    pPID P = (pPID)malloc(sizeof(PID));
    P->set_value = 0;
    P->prev_error = 0;
    P->last_error = 0;
    P->sum_error = 0;
    P->P = p;
    P->I = i;
    P->D = d;
    return P;
}

/***********************************************************
* 
*       PID参数初始化      
*
************************************************************/
void All_PID_init()
{
    Servo_PID = PID_create(S_P, S_I, S_D);
    Motor_PID = PID_create(M_P, M_I, M_D);
}
/************************************************************
*	作用:		位置式PD运算器【积分分离型】
*	Q:			准备运算的对象
*	input：		传感器测量值（即反馈）
*	out:		输出值
*************************************************************/
int PID_Position(pPID Q, int input)
{
    int iError;
    int output;

    iError = 0 - input;
    output = Q->P * iError + Q->D * (iError - Q->last_error);
    Q->last_error = iError;
    return output; //返回输出,即准备给驱动的PWM占空比
}
/***************************************************************
*	作用:		增量式PI运算器【积分分离型】
*	Q:		准备运算的对象
*	input：		传感器测量值（即反馈）
*	Increase_out: 输出的增量
****************************************************************/
int PID_Increase(pPID Car, int input) //增量式PID
{
    int32 iError;
    int32 increase_out;

    iError = Car->set_value - input;

    increase_out = Car->P * (iError - Car->last_error) + Car->I * iError;

    Car->last_error = iError;
    return increase_out;
}

/*************************************************************
*
*				对参数进行PID调节【自适应PID】
*	速度	Kp_P	Kp_I	Kp_D	Kd_P	Kd_I	Kd_D
*	360		5		18		0		2		10		0
*	400		7		11		0		3		9		0
*************************************************************/
PID Adapt_S_Kp = {.P = 7,
                  .I = 11,
                  .D = 0,
                  .last_error = 0}; //10000==1

PID Adapt_S_Kd = {.P = 3,
                  .I = 9,
                  .D = 0,
                  .last_error = 0}; //10000==1

PID Adapt_M_Kp = {.P = 3,
                  .I = 50,
                  .D = 0,
                  .last_error = 0}; //10000==1

int32 PID_Adapt(PID *sprt, int32 NowPiont)
{
    int32 iError; //当前误差
    int32 Actual; //最后得出的实际输出值

    iError = NowPiont; // 计算当前误差

    Actual = sprt->P * (iError * iError) // 动态参数与误差成平方关系
             + sprt->I * 10000           // 此处并非“积分”，而是动态参数的一个最小值
             + sprt->D * (iError - sprt->last_error);

    sprt->last_error = iError; //更新上次误差

    return Actual / 10000;
}

/**************************************************************
*
*               PID服务函数
*
**************************************************************/

void PID_Handler(uint8 Servo_T, uint8 Motor_T)
{
    static uint8 Servo_time = 0, Motor_time = 0;
    if (Servo_time > Servo_T)
    {
        Servo_PID->P = PID_Adapt(&Adapt_S_Kp, Ecc_Car->Error.now);
        Servo_PID->D = PID_Adapt(&Adapt_S_Kd, Ecc_Car->Error.delta);
        Ecc_Car->Duty.servo = -Ecc_Car->Error.now * Servo_PID->P +
                              Ecc_Car->Error.delta * Servo_PID->D;
        Ecc_Car->Duty.servo = RANGE(Ecc_Car->Duty.servo, 10000, -10000);
        Ecc_Car->Mode.servo = 0;
        Servo_time = 0;
    }

    if (Motor_time > Motor_T)
    {
        if (0 != Ecc_Car->Flag.obstacle)
            Motor_PID->set_value = Ecc_Car->Speed.obstacle; //避障速度Ecc_Car->Speed.standard
        else if (0 != Ecc_Car->Round.state)
            Motor_PID->set_value = Ecc_Car->Speed.round;
        else if (0 != Ecc_Car->Flag.outside)
            Motor_PID->set_value = Ecc_Car->Speed.outside;
        else
            Motor_PID->set_value = Ecc_Car->Speed.standard;

        Motor_PID->set_value = RANGE(Motor_PID->set_value, 1000, 200);
        Motor_PID->P = PID_Adapt(&Adapt_M_Kp, Ecc_Car->Speed.now);
        Ecc_Car->Duty.motor += PID_Increase(Motor_PID, Ecc_Car->Speed.now);
        /****************************电机限幅与保护*******************************************/
        if (Ecc_Car->Speed.now > 200)
            Ecc_Car->Duty.motor = RANGE(Ecc_Car->Duty.motor, 100000, 0);
        else
            Ecc_Car->Duty.motor = RANGE(Ecc_Car->Duty.motor, 40000, 0);
        if (Ecc_Car->Speed.now > 100 && 0 == Ecc_Car->Flag.protect)
            Ecc_Car->Flag.protect = 1;
        else if (1 == Ecc_Car->Flag.protect && Ecc_Car->Speed.now < 20)
        {
            Ecc_Car->Flag.protect = 0;
            Ecc_Car->Flag.stop = 1;
        }

        /***********************************************************************/

        Motor_time = 0;
    }

    Servo_time++;
    Motor_time++;
}