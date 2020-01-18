#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define INDUCT_NUM 8U //电感数量
#define MAXSIZE 10U   //用于均值滤波的数据量

#define OUTSIDE 50U        //标识出界的电压值
#define DIR_U_LIMIT 220    //最大方向偏差上限
#define DIR_D_LIMIT (-220) //最大方向偏差下限

#define R_GAIN 1 //内侧电感常增益 1
#define M_GAIN 1 //param.URt_k             //中间电感常增益1
#define L_GAIN 1 //外侧电感常增益2
#define F_GAIN 7 //前伸电感常增益//3

typedef struct
{
    uint8 front; //滑动采集的队首
    uint8 rear;  //滑动采集的队尾

    uint16 Nor_min;      //用于归一化计算的最小参考值
    uint16 Nor_max;      //电感的最大值
    uint16 Nor_MM_value; //最大值与最小值的差

    uint8 Normalized_value;      //归一化后的数据
    uint8 Last_Normalized_value; //历史归一化的数据

    uint16 ad_mean_value;     //均值
    uint16 ad_value[MAXSIZE]; //AD采集的原始数据,十个数据求平均
} Inductance, *pInductance;

extern Inductance Induct[INDUCT_NUM]; //定义8个电感

#define SCAN_NUM 2000 //扫描次数
typedef struct
{
    uint8 finish_flag; //扫描结束标志
    uint16 times;      //扫描次数
} SCAN;
extern SCAN Scan;

void AD_first_get();
void AD_sliding_get();
void AD_Scan();
void All_AD_init();
void Mean_Filter();
void Normalized();

#endif