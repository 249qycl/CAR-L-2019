#include "common.h"
#include "include.h"

/*************************************************
*
*               一阶低通滤波器
*
*************************************************/

int16 First_LowPassFilter(int16 input)
{
#define INDEX 10

    static int16 Last_output;
    int16 temp;

    temp = ((100 - INDEX) * Last_output + INDEX * input) / 100;
    Last_output = temp;

    return temp;
#undef INDEX
}
/*************************************************
*
*               一阶高通滤波器
*
*************************************************/
int16 First_HighPassFilter(int16 input)
{
#define INDEX 50

    static int16 Last_output, Last_input;
    int16 temp;

    temp = INDEX * (Last_output + input - Last_input);
    Last_output = temp;
    Last_input = input;

    return temp;

#undef INDEX
}
/*
*
*               递推均值滤波
*
*/
int16 RecursiveMeanFilter(int16 input)
{
#define BUFFER_NUM 5

    static int16 queue[BUFFER_NUM] = {0};
    static uint8 rear = 0;
    int32 sum = 0;

    rear = (rear + 1) % BUFFER_NUM;
    queue[rear] = input;
    for (uint8 i = BUFFER_NUM; i > 0; i--)
    {
        sum += queue[(rear + i) % BUFFER_NUM] * (i + 1);
    }

    return sum / 21; //递推均值滤波后把值缩小10倍
    
#undef BUFFER_NUM
}
/*******************************************************************************************
*                       【最小二乘法简化计算表】        
*
*       样本容量            分母【x平方的均值-x均值的平方】              分子【x的平均值】
*          1                            0.000f                                  0.00f
*          2                            0.250f                                  0.50f
*          3                            0.667f                                  1.00f
*          4                            1.250f                                  1.50f
*          5                            2.000f                                  2.00f
*          6                            2.917f                                  2.50f
*          7                            4.000f                                  3.00f
*          8                            5.250f                                  3.50f
*          9                            6.667f                                  4.00f
*          10                           8.250f                                  4.50f
*          11                           10.000f                                 5.00f
*          12                           11.917f                                 5.50f
*          13                           14.000f                                 6.00f
*          14                           16.250f                                 6.50f
*          15                           18.667f                                 7.00f
*          16                           21.250f                                 7.50f
*          17                           24.000f                                 8.00f
*          18                           26.917f                                 8.50f
*          19                           30.000f                                 9.00f
*          20                           33.250f                                 9.50f
*          21                           36.667f                                 10.00f
*          22                           40.250f                                 10.50f
*          23                           44.000f                                 11.00f
*          24                           47.917f                                 11.50f
*          25                           52.000f                                 12.00f
*          26                           56.250f                                 12.50f
*          27                           60.667f                                 13.00f
*          28                           65.250f                                 13.50f
*          29                           70.000f                                 14.00f
*          30                           74.917f                                 14.50f
*          31                           80.000f                                 15.00f
*          32                           85.250f                                 15.50f
*          33                           90.667f                                 16.00f
*          34                           96.250f                                 16.50f
*          35                           102.000f                                17.00f
*          36                           107.917f                                17.50f
*          37                           114.000f                                18.00f
*          38                           120.250f                                18.50f
*          39                           126.667f                                19.00f
*          40                           133.250f                                19.50f
*          41                           140.000f                                20.00f
*          42                           146.917f                                20.50f
*          43                           154.000f                                21.00f
*          44                           161.250f                                21.50f
*          45                           168.667f                                22.00f
*          46                           176.250f                                22.50f
*          47                           184.000f                                23.00f
*          48                           191.917f                                23.50f
*          49                           200.000f                                24.00f
*          50                           208.250f                                24.50f
*
*******************************************************************************************/
#define Number 5 //样本容量
#define DEN 2.0f //分母
#define NUM 2.0f //分子
/******************************************************************************************
*       
*       【最小二乘法求斜率K,即变化趋势】
*       interval:采样间隔
*       input:输入
*       num:适应多电感
*
*******************************************************************************************/
float Least_square_method1(float input, uint8 num, uint8 interval)
{
    static float queue[INDUCT_NUM][Number] = {0}, last_temp[INDUCT_NUM] = {0};
    static uint8 rear[INDUCT_NUM] = {0};
    static uint8 times[INDUCT_NUM] = {0};

    float t1 = 0, t2 = 0, temp;

    if (times[num]++ >= interval)
    {
        times[num] = 0;
        rear[num] = (rear[num] + 1) % Number;
        queue[num][rear[num]] = input;
        for (uint8 i = 0; i < Number; i++)
        {
            t1 += i * queue[num][i]; //常数
            t2 += queue[num][i];
        }
        t1 = t1 / Number;
        t2 = t2 / Number;
        temp = (t1 - (NUM)*t2) / DEN; //放大1倍
        last_temp[num] = temp;
        return temp;
    }
    else
        return last_temp[num];
}
/******************************************************************************************
*       
*       【最小二乘法求斜率K,即变化趋势】
*       interval:采样间隔
*       input:输入
*
*******************************************************************************************/
float Least_square_method2(float input, uint8 interval)
{
    static float queue[Number] = {0}, last_temp = 0;
    static uint8 rear = 0, times = 0;

    float t1 = 0, t2 = 0, temp;

    if (times++ >= interval)
    {
        times = 0;
        rear = (rear + 1) % Number;
        queue[rear] = input;
        for (uint8 i = 0; i < Number; i++)
        {
            t1 += i * queue[i]; //常数
            t2 += queue[i];
        }
        t1 = t1 / Number;
        t2 = t2 / Number;
        temp = (t1 - (NUM)*t2) / DEN; //放大1倍
        last_temp = temp;
        return temp;
    }
    else
        return last_temp;
}
