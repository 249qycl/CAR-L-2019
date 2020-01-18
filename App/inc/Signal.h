#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#define INDUCT_NUM 6U 		//电感数量
#define MAXSIZE   10U   	//用于均值滤波的数据量

typedef struct
{
    uint8 front; //滑动采集的队首
    uint8 rear;  //滑动采集的队尾

	struct
	{
		uint16 min;//用于归一化计算的最小参考值
		uint16 max;//电感的最大值
		uint16 range;//最大值与最小值的极差
		uint8  now;//归一化后的数据
		uint8 last;//历史归一化的数据
	}Nor;

	struct
	{
		uint16 average;//均值
		uint16 data[MAXSIZE];//AD采集的原始数据,十个数据求平均
	}AD;
} Inductance, *pInductance;

extern Inductance Induct[INDUCT_NUM]; //定义8个电感
                    
#define SCAN_NUM 2000 //扫描次数

void AD_first_get();
void AD_sliding_get();
void AD_Scan();
void All_AD_init();
void Mean_Filter();
void Normalized();

#endif