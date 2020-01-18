#ifndef __INFOMATION_H__
#define __INFOMATION_H__

#define OUTSIDE 100U        //标识出界的电压值
#define DIR_U_LIMIT 220    //最大方向偏差上限
#define DIR_D_LIMIT (-220) //最大方向偏差下限

#define R_GAIN 1 //内侧电感常增益 1
#define M_GAIN 2 //中间电感常增益1
#define L_GAIN 1 //外侧电感常增益2

typedef struct
{
    int32 W;
    int32 Z;
    int32 N;
} T_ERR;

extern T_ERR WZN;

typedef struct
{
    int16 k0; //外
    int16 k1; //中
    int16 k2; //内
} DIR_ERR;
extern DIR_ERR DIR_gain;

static inline void GAIN_SET(int16 a,int16 b,int16 c)
{
	DIR_gain.k0 = a;
	DIR_gain.k1 = b;
	DIR_gain.k2 = c;
}

#define SUB0 (Induct[0].Nor.now - Induct[5].Nor.now) //内侧
#define SUB1 (Induct[1].Nor.now - Induct[4].Nor.now) //中间
#define SUB2 (Induct[2].Nor.now - Induct[3].Nor.now) //外侧
#define SUB3 (Induct[0].Nor.now - Induct[5].Nor.now+\
			  Induct[1].Nor.now - Induct[4].Nor.now+\
			  Induct[2].Nor.now - Induct[3].Nor.now) //外侧

#define ADD0 (Induct[0].Nor.now + Induct[5].Nor.now + \
              Induct[2].Nor.now + Induct[3].Nor.now)

#define ADD2 (Induct[1].Nor.now + Induct[4].Nor.now)

#define ADD3 (Induct[0].Nor.now + Induct[2].Nor.now + \
              Induct[1].Nor.now + Induct[4].Nor.now + \
              Induct[3].Nor.now + Induct[5].Nor.now)

#define ADD4 (Induct[0].Nor.now + Induct[5].Nor.now)
#define ADD5 (Induct[2].Nor.now + Induct[3].Nor.now)

#define MUL0 (Induct[2].Nor.now * Induct[3].Nor.now)
#define MUL1 (Induct[1].Nor.now * Induct[4].Nor.now)
#define MUL2 (Induct[0].Nor.now * Induct[5].Nor.now)

int Direct_error();
void Outside_judge();

#endif