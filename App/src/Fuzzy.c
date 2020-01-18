#include "common.h"
#include "include.h"
/*************************************************************************
*                       【比例参数规则库】
*
*                                     偏差增量EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  PB      PB      PM      PM      PS      ZE      ZE
*          NM  .  PB      PB      PM      PS      PS      ZE      NS
*          NS  .  PM      PM      PM      PS      ZE      NS      NS
* 偏差E    ZE  .  PM      PM      PS      ZO      NS      NM      NM
*          PS  .  PS      PS      ZE      NS      NS      NM      NM
*          PM  .  PS      ZE      NS      NM      NM      NM      NB
*          PB  .  ZE      ZE      NM      NM      NM      NB      NB
*************************************************************************/
const float RuleKp[7][7] = {
    PB, PB, PM, PM, PS, ZE, ZE,
    PB, PB, PM, PS, PS, ZE, NS,
    PM, PM, PM, PS, ZE, NS, NS,
    PM, PM, PS, ZE, NS, NM, NM,
    PS, PS, ZE, NS, NS, NM, NM,
    PS, ZE, NS, NM, NM, NM, NB,
    ZE, ZE, NM, NM, NM, NB, NB};
/*************************************************************************
*                       【积分参数规则库】
*
*                                     偏差增量EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  NB      NB      NM      NM      NS      ZE      ZE
*          NM  .  NB      NB      NM      NS      NS      ZE      ZE
*          NS  .  NB      NM      NS      NS      ZE      PS      PS
* 偏差E    ZE  .  NM      NM      NS      ZE      PS      PM      PM
*          PS  .  NM      NS      ZE      PS      PS      PM      PB
*          PM  .  ZE      ZE      PS      PS      PM      PB      PB
*          PB  .  ZE      ZE      PS      PM      PM      PB      PB
*************************************************************************/
const float RuleKi[7][7] = {
    NB, NB, NM, NM, NS, ZE, ZE,
    NB, NB, NM, NS, NS, ZE, ZE,
    NB, NM, NS, NS, ZE, PS, PS,
    NM, NM, NS, ZE, PS, PM, PM,
    NM, NS, ZE, PS, PS, PM, PB,
    ZE, ZE, PS, PS, PM, PB, PB,
    ZE, ZE, PS, PM, PM, PB, PB};
/*************************************************************************
*                       【微分参数规则库】
*
*                                     偏差增量EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  PS      NS      NB      NB      NB      NM      PS
*          NM  .  PS      NS      NB      NM      NM      NS      ZE
*          NS  .  ZE      NS      NM      NM      NS      NS      ZE
* 偏差E    ZE  .  ZE      NS      NS      NS      NS      NS      ZE
*          PS  .  ZE      ZE      ZE      ZE      ZE      ZE      ZE
*          PM  .  PB      NS      PS      PS      PS      PS      PB
*          PB  .  PB      PM      PM      PM      PS      PS      PB
*************************************************************************/
const float RuleKd[7][7] = {
    PS, NS, NB, NB, NB, NM, PS,
    PS, NS, NB, NM, NM, NS, ZE,
    ZE, NS, NM, NM, NS, NS, ZE,
    ZE, NS, NS, NS, NS, NS, ZE,
    ZE, ZE, ZE, ZE, ZE, ZE, ZE,
    PB, NS, PS, PS, PS, PS, PB,
    PB, PM, PM, PM, PS, PS, PB};
/*************************************************************************
*                       【执行器输出知识库】(课本上的,应用时根据具体情况调整)
*
*                                     偏差增量EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  NB      NB      NM      NM      NS      ZE      ZE
*          NM  .  NB      NB      NM      NM      NS      ZE      ZE
*          NS  .  NB      NB      NM      NS      ZE      PM      PM
* 偏差E    ZE  .  NB      NB      NM      ZE      PM      PB      PB
*          PS  .  NM      NM      ZE      PS      PM      PB      PB
*          PM  .  ZE      ZE      PS      PM      PM      PB      PB
*          PB  .  ZE      ZE      PS      PM      PM      PB      PB
*************************************************************************/
float Servo_duty[7] = {-100, -67, -34, 0, 34, 67, 100}; //舵机划分
const float RuleOut[7][7] = {
    NB, NB, NM, NM, NS, ZE, ZE,
    NB, NB, NM, NM, NS, ZE, ZE,
    NB, NB, NM, NS, ZE, PM, PM,
    NB, NB, NM, ZE, PM, PB, PB,
    NM, NM, ZE, PS, PM, PB, PB,
    ZE, ZE, PS, PM, PM, PB, PB,
    ZE, ZE, PS, PM, PM, PB, PB};
/*************************************************************************
*                       【电机占空比输出知识库】
*
*                                     偏差增量EC
*                 NB      NM      NS      ZE      PS      PM      PB
*              -----------------------------------------------------
*          NB  .  NB      NB      NM      NM      NS      ZE      ZE
*          NM  .  NB      NB      NM      NM      NS      ZE      ZE
*          NS  .  NB      NB      NM      NS      ZE      PM      PM
* 偏差E    ZE  .  NB      NB      NM      ZE      PM      PB      PB
*          PS  .  NM      NM      ZE      PS      PM      PB      PB
*          PM  .  ZE      ZE      PS      PM      PM      PB      PB
*          PB  .  ZE      ZE      PS      PM      PM      PB      PB
*************************************************************************/
float Motor_duty[7] = {0, 150, 250, 350, 450, 550, 650}; //电机占空比输出划分
const float RuleDuty[7][7] = {
    NB, NB, NM, NB, NB, NB, NB,
    NB, NB, NB, NB, NB, NB, NB,
    NS, NS, NS, NM, NS, NS, NS,
    NS, NS, ZE, PS, ZE, NS, NS, //ze
    NS, NS, NS, NM, NS, NS, NS,
    NB, NB, NB, NB, NB, NB, NB,
    NB, NB, NB, NB, NB, NB, NB};
/*********************************************************
*
*       1、求输入的隶属度和隶属度值
*       2、按照隶属度值查表得出相应的几个输出隶属度值
*       3、对得到的隶属度值结合相应的隶属度进行重心法运算
*
*********************************************************/

float S_Kd[7] = {0};
float S_Kp[7] = {0};
/*********************************************
*
*       模糊运算器
*       Rule:规则库
*       range=maxmum-minmum
*       e :偏差
*       ec:偏差增量
*       object:输出对象
*********************************************/
float fuzzy(float range, float e, float ec, const float (*Rule)[7], float *object) /*调用前先对偏差及偏差增量量化*/
{

        float e_temp, ec_temp;     //隶属度左右值划定预处理中间变量
        float e_L_temp, ec_L_temp; //隶属度
        float e_R_temp, ec_R_temp;

        int e_L_Index, ec_L_Index; //隶属度索引
        int e_R_Index, ec_R_Index;
        float out; //解模糊后的输出值
        /*线性量化操作*/
        e = 6.0 * e / range;   //偏差统一成-6~6之间的量
        ec = 3.0 * ec / range; //偏差增量统一成-6~6之间的量
        /*偏差隶属度左右值划定预处理*/
        e_temp = e > 6.0 ? 0.0 : (e < -6.0 ? 0.0 : (e >= 0.0 ? (e >= 4.0 ? 5.0 : (e >= 2.0 ? 3.0 : 1.0)) : (e >= -2.0 ? -1.0 : (e >= -4.0 ? -3.0 : (e >= -6.0 ? -5.0 : 0.0)))));
        /*偏差隶属度左右值索引映射*/
        e_L_Index = (int)((e_temp - 1) / 2 + 3); //[-6,6] ---映射---> [0,6]
        e_R_Index = (int)((e_temp + 1) / 2 + 3);
        /*求偏差对左右值的隶属度*/
        e_L_temp = e_temp == 0.0 ? 0.0 : (((e_temp + 1) - e) / 2);
        e_R_temp = e_temp == 0.0 ? 0.0 : ((e - (e_temp - 1)) / 2);
        /*偏差增量隶属度左右值划定预处理*/
        ec_temp = ec > 6.0 ? 0.0 : (ec < -6.0 ? 0.0 : (ec >= 0.0 ? (ec >= 4.0 ? 5.0 : (ec >= 2.0 ? 3.0 : 1.0)) : (ec >= -2.0 ? -1.0 : (ec >= -4.0 ? -3.0 : (ec >= -6.0 ? -5.0 : 0.0)))));
        /*偏差隶属度左右值索引映射*/
        ec_L_Index = (int)((ec_temp - 1) / 2 + 3); //[-6,6] ---映射---> [0,6]
        ec_R_Index = (int)((ec_temp + 1) / 2 + 3);
        /*求偏差增量对左右值的隶属度*/
        ec_L_temp = ec_temp == 0.0 ? 0.0 : (((ec_temp + 1) - ec) / 2);
        ec_R_temp = ec_temp == 0.0 ? 0.0 : ((ec - (ec_temp - 1)) / 2);
        /************************************重心法解模糊*******************************************/

#if 1 //查表定下标
        out = e_L_temp * (ec_L_temp * object[(uint8)Rule[e_L_Index][ec_L_Index]] + ec_R_temp * object[(uint8)Rule[e_L_Index][ec_R_Index]]) 
            + e_R_temp * (ec_L_temp * object[(uint8)Rule[e_R_Index][ec_L_Index]] + ec_R_temp * object[(uint8)Rule[e_R_Index][ec_R_Index]]);
#else //查表定值
        out = e_L_temp * (ec_L_temp * Rule[e_L_Index][ec_L_Index] + ec_R_temp * Rule[e_L_Index][ec_R_Index]) + e_R_temp * (ec_L_temp * Rule[e_R_Index][ec_L_Index] + ec_R_temp * Rule[e_R_Index][ec_R_Index]);
#endif
        return out;
}