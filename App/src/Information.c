#include "common.h"
#include "include.h"

DIR_ERR DIR_gain;              //电感增益系数

/***************************************************
*
*               垂直方向偏差求取
*
***************************************************/
int32 Rt_error_gain(uint8 nor1, uint8 nor2)
{
    nor1 = RANGE(nor1, 60, 1);
    nor2 = RANGE(nor2, 60, 1);
    int32 temp = (nor1 + nor2) > 20 ? 1000 : 0;

    return temp;
}
/*****************************************************************************
*
*               差比和式电感信息偏差拟合
*
*****************************************************************************/
uint32 Error_fitting_sub_add(void)
{
    int32 DIR_error;
    uint16 ADD;
    if (DIR_gain.k0 != 0 && DIR_gain.k1 == 0 && DIR_gain.k2 == 0 && DIR_gain.k3 == 0)
        ADD = ADD1(1, 0, 0, 0); //内中外前
    else if (DIR_gain.k0 != 0 && DIR_gain.k1 != 0 && DIR_gain.k2 == 0 && DIR_gain.k3 != 0)
        ADD = ADD1(1, 1, 0, 1);
    else if (DIR_gain.k0 != 0 && DIR_gain.k1 != 0 && DIR_gain.k2 != 0 && DIR_gain.k3 != 0)
        ADD = ADD1(1, 1, 1, 1);
    else if (DIR_gain.k0 != 0 && DIR_gain.k1 == 0 && DIR_gain.k2 != 0 && DIR_gain.k3 == 0)
        ADD = ADD1(1, 0, 1, 0);
    else if (DIR_gain.k0 != 0 && DIR_gain.k1 == 0 && DIR_gain.k2 == 0 && DIR_gain.k3 != 0)
        ADD = ADD1(1, 0, 0, 1);
    else if (DIR_gain.k0 == 0 && DIR_gain.k1 == 0 && DIR_gain.k2 != 0 && DIR_gain.k3 == 0)
        ADD = ADD1(0, 0, 1, 0);
    else if (DIR_gain.k0 == 0 && DIR_gain.k1 == 0 && DIR_gain.k2 == 0 && DIR_gain.k3 != 0)
        ADD = ADD1(0, 0, 0, 1);
    else
        ADD = ADD1(1, 0, 1, 1);

    ADD = ADD > 0 ? ADD : 1;
    DIR_error = (int)((SUB0 * DIR_gain.k0 + SUB1 * DIR_gain.k1 + SUB2 * DIR_gain.k2 + SUB3 * DIR_gain.k3) * 100 / ADD);
    DIR_error = RANGE(DIR_error, DIR_U_LIMIT, DIR_D_LIMIT); //偏差限幅

    return DIR_error;
}
/*****************************************************************************
*
*               差比积式电感信息偏差拟合
*
*****************************************************************************/
uint32 Error_fitting_sub_mul(void)
{
#define CALCULATE(num, K) (pow(SUB##num,3) * K * DIR_gain.k##num / Mul##num)
    int32 Mul0, Mul1, Mul2, Mul3, Rt_gain, DIR_error;
    Mul0 = MUL0;
    Mul1 = MUL1;
    Mul2 = MUL2;
    Mul3 = MUL3;
    Mul0 = Mul0 != 0 ? Mul0 : 1;
    Mul1 = Mul1 != 0 ? Mul1 : 1;
    Mul2 = Mul2 != 0 ? Mul2 : 1;
    Mul3 = Mul3 != 0 ? Mul3 : 1;
    Rt_gain = Rt_error_gain(Induct[1].Normalized_value, Induct[4].Normalized_value);
    //常态输出值较小的电感系数方案：低差值不进行运算。双上限60，
    DIR_error = (int32)(CALCULATE(0, 5000) + CALCULATE(1, Rt_gain) + CALCULATE(2, 2000) + CALCULATE(3, Rt_gain));
    DIR_error = RANGE(DIR_error, DIR_U_LIMIT, DIR_D_LIMIT); //偏差限幅

    return DIR_error;
}
/*************************************************************
*
*                       出界判断及防出界处理
*
*************************************************************/
void Outside_judge(int16 *dir_error)
{
    static int last_dir_err = 0;
    static uint16 times = 0; //出界或丢线计时
    uint16 out_sum = 0;

    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        out_sum += abs(Induct[i].Normalized_value);
    }
    if (out_sum < OUTSIDE)
    {
        times++;
        Ecc_Car->Servo_mode = last_dir_err >= 0 ? 3 : 4; //短暂丢线沿用历史方向后角度打死
        if (times >= 400)                                //出界2秒后判定为出界
        {
            Ecc_Car->outside_flag = 1;
            times = 400;
        }
    }
    else
    {
        Ecc_Car->outside_flag = 0;
        last_dir_err = *dir_error;
        times = 0;
    }
}
/****************************************************
*
*               续一秒        
*
****************************************************/
void Keep_filter(uint8 flag)
{
    static uint16 times = 0;
    if (flag == 1)
    {
        times = times > 0 ? (times - 1) : 0;
        if (times > 0 || abs(Ecc_Car->Dir_Error) > 110)
        {
            Ecc_Car->Dir_Error = 150;
            if (times == 0)
                times = 100;
        }
    }
}

/**********************************************
*
*               十字、直角判断及处理
*
***********************************************/
void Cross_Rt_judge(uint8 finish_flag)
{
    static uint16 c_times = 0, r_times = 0;
    c_times = c_times > 0 ? (c_times - 1) : 0;
    r_times = r_times > 0 ? (r_times - 1) : 0;

    if (r_times == 0 && finish_flag == 0 && (Induct[6].Normalized_value > 35 && Induct[7].Normalized_value > 35)) //十字
    {
        GAIN_SET(10, 0, 10, 1); //2 10 1
        c_times = 20;
    }
    else if (c_times == 0 && finish_flag == 0 && abs(SUB3) > 20 && (Induct[6].Normalized_value < 25 || Induct[7].Normalized_value < 25))
    {
        GAIN_SET(2, 1, 2, 25);
        r_times = 100;
    }
    else if (finish_flag == 0 && c_times == 0 && r_times == 0)
        GAIN_SET(R_GAIN, M_GAIN, L_GAIN, F_GAIN);
}
/************************************************************
*
*                       环岛处理
*
************************************************************/
uint8 Round_judge(int input)
{
#define THRE 420
#define TIMES 10
    static uint8 times1 = 0,
                 times2 = 0,
                 re_sample = 0,
                 Round_state = 0,           //进环状态级别记录
                 Round_move = 0,            //运动状态处理
                 limit = TIMES;             //用于判断环岛状态

    static int16 max_input = 0, threshold = THRE, re_times = 0;

    if (re_sample == 0 && re_times == 0)
    {
        re_sample = times1 > limit ? 1 : re_sample;
        times1 = input > threshold ? ((abs(SUB3) > 15 || Round_state == 1) ? (times1 + 1) : 0) : 0; //20 【判断是否是环岛】
    }
    else if (re_sample == 1)
    {
        times2 = input < max_input ? (times2 + 1) : 0;
        if (input < max_input && times2 > limit) //到达交叉点
        {
            Round_move = Round_state == 1 ? 6 : 2;
            Round_state = Round_state == 0 ? 1 : 2;

            re_sample = 0;
            max_input = 0;
            re_times = 400; //2秒内不再响应环判断
            /*刷新环岛判断标准*/
            threshold = Round_state == 1 ? 400 : THRE;
            limit = Round_state == 1 ? 5 : TIMES;
        }
        else //对状态建立后的值进行最大值追踪
        {
            max_input = input;
            Round_move = Round_state == 1 ? 5 : 1;
        }
    }
    re_times = re_times > 0 ? (re_times - 1) : 0; //防重复判断计时
    switch (Round_state)
    {
    case 1:
        if (input < (THRE - 40))
            Round_move = (Induct[6].Normalized_value > 45 && Induct[7].Normalized_value > 45) ? 4 : 3;
        break;
    case 2:
        if (input < threshold) //完全恢复，正常引导,相关信息复位
        {
            Round_move = 0;
            Round_state = 0;
        }
        break;
    }

    switch (Round_move)
    {
    case 0:
        GAIN_SET(R_GAIN, M_GAIN, L_GAIN, F_GAIN);
        break;
    case 1:
        GAIN_SET(1, 0, 0, 0);
        break; //进环点前引导
    case 2:
        GAIN_SET(0, 0, 0, 125);
        break; //进环点引导55
    case 3:
        GAIN_SET(1, 40, 3, 40);
        break; //环内引导【判断已经在环内后】
    case 4:
        GAIN_SET(3, 15, 1, 0);
        break; //即将出环点引导
    case 5:
        GAIN_SET(5, 15, 0, 0);
        break; //出环点前引导//5 0 0 0
    case 6:
        GAIN_SET(0, -4, 0, -30);
        break; //出环点引导
    }

    param.in_round = Round_state;
    /*判断环岛是否运行完毕*/
    if (Round_state != 0 || re_sample != 0)
        return 1;
    else
        return 0;

#undef THRE
#undef TIMES
}



/************************************************
*
*       求取方向偏差，并返回
*
************************************************/
int Direct_error()
{
    uint8 round_finished_flag; //环岛结束标志
    int32 DIR_error;
    uint16 AD_Sum;

    Normalized(); //数值归一化处理
    AD_Sum = ADD0;
    param.All_ad_sum = AD_Sum;

    //    round_finished_flag=Round_judge(AD_Sum);//室外420
    //    Cross_Rt_judge(round_finished_flag);
    DIR_gain.k1 = 0;
    DIR_gain.k3 = 0;
	DIR_error=Error_fitting_sub_mul();
    //    Keep_Filter(round_finished_flag);

    return DIR_error;
}