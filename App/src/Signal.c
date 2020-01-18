#include "common.h"
#include "include.h"

SCAN Scan;                     //赛道初始扫描
Inductance Induct[INDUCT_NUM]; //定义电感对象
ADCn_Ch_e ADC_Channel[INDUCT_NUM] = {ADC0_SE8, ADC0_SE9, ADC0_SE10, ADC0_SE11, ADC0_SE12, ADC0_SE13,
                                     ADC0_SE17, ADC0_SE18}; //AD采集通道

typedef struct
{
    void (*init)();
    void (*filter)();
    void (*get)();
    void (*scan)();
} SIGNAL;
SIGNAL Signal;

/*****************************************************
*
*       对ADC通道、电感初始状态信息及相关数据进行初始化
*
******************************************************/
void ADC_init()
{
    for (uint8 j = 0; j < INDUCT_NUM; j++) //电感信息存储空间初始化
    {
        adc_init(ADC_Channel[j]);
        Induct[j] = (Inductance){
            .ad_value = {0},
            .ad_mean_value = 0,
            .front = 0,
            .rear = 0,
            .Last_Normalized_value = 0,
            .Normalized_value = 0};
    }
    DIR_gain = (DIR_ERR){R_GAIN, M_GAIN, L_GAIN, F_GAIN};
}
/***************************************
*
*               均值滤波
*
***************************************/
void Mean_Filter()
{
    uint16 sum;
    for (uint8 j = 0; j < INDUCT_NUM; j++)
    {
        sum = 0;
        for (uint8 i = 0; i < MAXSIZE; i++)
        {
            sum += Induct[j].ad_value[i];
        }
        Induct[j].ad_mean_value = sum / 10;
    }
}
/*******************************************
*
*       首次AD采集，用采集结果填充样本空间
*
*******************************************/
static void AD_first_get()
{
    for (uint8 i = 0; i < MAXSIZE; i++)
    {
        for (uint8 j = 0; j < INDUCT_NUM; j++)
        {
            Induct[j].ad_value[i] = adc_once(ADC_Channel[j], ADC_10bit);
        }
        DELAY_MS(5);
    }
    Mean_Filter();
    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        Induct[i].Nor_min = Induct[i].ad_mean_value;
        Induct[i].Nor_max = Induct[i].ad_mean_value;
    }
}

/************************************************
*
*       借助队列，进行滑动采集
*
************************************************/
void AD_sliding_get()
{
    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        Induct[i].ad_value[Induct[i].rear] = adc_once(ADC_Channel[i], ADC_10bit); //采集函数的返回值
        Induct[i].front = (Induct[i].front + 1) % MAXSIZE;
        Induct[i].rear = (Induct[i].rear + 1) % MAXSIZE;
    }
}
/***************************************
*
*       更新用于归一化的最大最小值
*       Q:相应电感的地址
***************************************/
static void Get_min_max(pInductance Induct)
{
    if (Induct->ad_mean_value > Induct->Nor_max)
        Induct->Nor_max = Induct->ad_mean_value;
    else if (Induct->ad_mean_value < Induct->Nor_min)
        Induct->Nor_min = Induct->ad_mean_value;
}
/**********************************************
*
*       AD阈值扫描【在中断中调用】
*
**********************************************/
void AD_Scan()
{
    AD_sliding_get();
    Mean_Filter();
    for (uint8 i = 0; i < INDUCT_NUM; i++) //阈值扫描
    {
        Get_min_max(&Induct[i]);
        Induct[i].Nor_MM_value = Induct[i].Nor_max - Induct[i].Nor_min;
    }
    Scan.times--;
    if (Scan.times == 0)
        Scan.finish_flag = 1;
}

/****************************************
*
*       AD所有功能初始化
*
****************************************/
void All_AD_init()
{
    ADC_init();
    AD_first_get();

    Scan = (SCAN){0, SCAN_NUM};
}

/****************************************************
*
*               归一化处理
*
****************************************************/
void Normalized()
{
    int temp;
    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        temp = (Induct[i].ad_mean_value - Induct[i].Nor_min) * 100 / Induct[i].Nor_MM_value;
        Induct[i].Normalized_value = RANGE(temp, 255, 0);
    }
}

void Object_SIGNAL_init()
{
    Signal = (SIGNAL){
        .init = ADC_init,
        .filter = Mean_Filter,
        .scan = AD_Scan,
        .get = AD_sliding_get,
    };
}




//    DIR_error=RecursiveMeanFilter(DIR_error);
//    First_LowPassFilter(DIR_error);
