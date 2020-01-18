#include "common.h"
#include "include.h"

pPID Servo_PID; //创建舵机PID参数体
pPID Motor_PID; //创建电机PID参数体

/************************************************************
*	作用：PID对象参数初始化
*	limit：积分分离的偏差界限
*	saturation：积分饱和值
*************************************************************/
pPID PID_create(int16 p, int16 i, int16 d, uint16 limit)
{
    pPID P = (pPID)malloc(sizeof(PID));
    P->set_value = 0;
    P->prev_error = 0;
    P->last_error = 0;
    P->sum_error = 0;
    P->P = p;
    P->I = i;
    P->D = d;
    P->error_limit = limit;
    P->saturation_flag = 0;
    P->last_out = 0;
    return P;
}
/**********************************************************
*
*       更新PID控制器的参数
*
***********************************************************/
void PID_update(pPID Q, int16 p, int16 i, int16 d)
{
    Q->P = p;
    Q->I = i;
    Q->D = d;
}
/***********************************************************
* 
*       PID参数初始化      
*
************************************************************/
void All_PID_init()
{
    Servo_PID = PID_create(S_P, S_I, S_D, S_LIMIT);
    Motor_PID = PID_create(M_P, M_I, M_D, M_LIMIT);
}
/************************************************************
*	作用:		位置式PID运算器【积分分离型】
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
    Q->last_out = output;
    return output; //返回输出,即准备给驱动的PWM占空比
}
/***************************************************************
*	作用:		增量式PID运算器【积分分离型】
*	Q:		准备运算的对象
*	input：		传感器测量值（即反馈）
*	Increase_out: 输出的增量
****************************************************************/
int PID_Increase(pPID Q, int input) //增量式PID
{
    int32 iError;
    int32 increase_out;
    uint8 index = 1;

    iError = Q->set_value - input;

#if M_DEAD_SWITCH
    if (iError < M_DEAD_LIMIT)
        iError = 0;
#endif
 
    increase_out = Q->P * (iError - Q->last_error) + index * Q->I * iError;
    //+Q->D * (iError- 2*Q->last_error+Q->prev_error);
    //Q->prev_error = Q->last_error;

    Q->last_error = iError;
    return increase_out;
}
/************************************************************
*
*                       舵机分段式PID
*
************************************************************/
SecPID Servo_pid = {120,                //BangBang作用阈值
                    50, 100, 220, 220,  //【0.66,3.6】      【130 160】
                    100, 400, 400, 440, //
                    10, 10,             //第一段
                    36, 110,            //第二段//160[110]
                    66, 3000,           //第三段//【3000】
                    66, 710};           //第四段

int Section_PID(SecPID *Q, pPID T, int16 input1, int16 input2)
{
    char indexP, indexD;
    int  Error,output, P_adj, D_adj;

    Error = T->set_value - input1;
    int16 temp = abs(Error);
    if (temp >= Q->TH) //舵机BangBang控制
    {
        Ecc_Car->Servo_mode = Error > Q->TH ? 4 : (Error < -Q->TH ? 3 : 0);
        return 0;
    }
    else
        Ecc_Car->Servo_mode = 0;

    indexP = SEC_JUDGE(P, 0) ? (SEC_JUDGE(P, 1) ? (SEC_JUDGE(P, 2) ? (SEC_JUDGE(P, 3) ? 4
                                                                                      : 3)
                                                                   : 2)
                                                : 1)
                             : 0;
    SYMBOL(indexP);

    temp = abs(input2);
    indexD = SEC_JUDGE(D, 0) ? (SEC_JUDGE(D, 1) ? (SEC_JUDGE(D, 2) ? (SEC_JUDGE(D, 3) ? 4
                                                                                      : 3)
                                                                   : 2)
                                                : 1)
                             : 0;
    SYMBOL(indexD);

    switch (indexP)
    {
        //    case -3:P_adj=(Error + Q->P_Th2)*Q->P3 - (Q->P_Th2 - Q->P_Th1)*Q->P2 -(Q->P_Th1 - Q->P_Th0)*Q->P1 + Q->P_Th0*Q->P0;
        //            break;
    case -2:
        P_adj = (Error + Q->P_Th1) * Q->P2 - (Q->P_Th1 - Q->P_Th0) * Q->P1 - Q->P_Th0 * Q->P0;
        break;
    case -1:
        P_adj = (Error + Q->P_Th0) * Q->P1 - Q->P_Th0 * Q->P0;
        break;
    case 0:
        P_adj = Error * Q->P0;
        break;
    case 1:
        P_adj = (Error - Q->P_Th0) * Q->P1 + Q->P_Th0 * Q->P0;
        break;
    case 2:
        P_adj = (Error - Q->P_Th1) * Q->P2 + (Q->P_Th1 - Q->P_Th0) * Q->P1 + Q->P_Th0 * Q->P0;
        break;
        //    case  3:P_adj=(Error - Q->P_Th2)*Q->P3 + (Q->P_Th2 - Q->P_Th1)*Q->P2 +(Q->P_Th1 - Q->P_Th0)*Q->P1 + Q->P_Th0*Q->P0;
        //            break;
    }
    switch (indexD)
    {
        //    case -3:D_adj=(input2 + Q->D_Th2)*Q->D3 -(Q->D_Th2 - Q->D_Th1)*Q->D2 -(Q->D_Th1 - Q->D_Th0)*Q->D1 + Q->D_Th0*Q->D0;
        //            break;
    case -2:
        D_adj = (input2 + Q->D_Th1) * Q->D2 - (Q->D_Th1 - Q->D_Th0) * Q->D1 - Q->D_Th0 * Q->D0;
        break;
    case -1:
        D_adj = (input2 + Q->D_Th0) * Q->D1 - Q->D_Th0 * Q->D0;
        break;
    case 0:
        D_adj = input2 * Q->D0;
        break;
    case 1:
        D_adj = (input2 - Q->D_Th0) * Q->D1 + Q->D_Th0 * Q->D0;
        break;
    case 2:
        D_adj = (input2 - Q->D_Th1) * Q->D2 + (Q->D_Th1 - Q->D_Th0) * Q->D1 + Q->D_Th0 * Q->D0;
        break;
        //    case  3:D_adj=(input2 - Q->D_Th2)*Q->D3 +(Q->D_Th2 - Q->D_Th1)*Q->D2 +(Q->D_Th1 - Q->D_Th0)*Q->D1 + Q->D_Th0*Q->D0;
        //            break;
    }
    output = P_adj + D_adj;
    param.P_adj = P_adj;
    param.D_adj = D_adj;
    output = RANGE(output, 10000, -10000);
    return output;
}

/**************************************************************
*
*               PID服务函数
*
**************************************************************/
void PID_Handler(uint8 S_frent, uint8 M_frent)
{
    static uint8 Servo_frent = 0, Motor_frent = 0;
    if (Servo_frent == S_frent)
    {
        Ecc_Car->S_Duty = Section_PID(&Servo_pid, Servo_PID, Ecc_Car->Dir_Error, Ecc_Car->Delta_err);
        //      Ecc_Car->S_Duty =PID_Position(Servo_PID, Ecc_Car->Dir_Error);舵机位置式PID
        Servo_frent = 0;
    }
    /* 
   if(Motor_frent==20)
   {
       Ecc_Car->M_Duty= Ecc_Car->speed>50? (Ecc_Car->M_Duty+PID_Increase(Motor_PID, Ecc_Car->speed)):26000;
       Motor_frent=0;
   }
   if(Motor_frent==M_frent)
   {
       if( Ecc_Car->speed>50)
           Ecc_Car->M_Duty+=PID_Increase(Motor_PID, Ecc_Car->speed);
       else 
           Ecc_Car->M_Duty=Motor_PID->P*(Motor_PID->P-Ecc_Car->speed);
       Motor_frent=0;
   } 
*/
    Servo_frent++;
    Motor_frent++;
    Ecc_Car->M_Duty = RANGE(Ecc_Car->M_Duty, 100000, 20000);
}