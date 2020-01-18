#ifndef __INFOMATION_H__
#define __INFOMATION_H__

typedef struct
{
    char k0;
    char k1;
    char k2;
    char k3;
} DIR_ERR;
extern DIR_ERR DIR_gain;

#define GAIN_SET(a, b, c, d) \
    do                       \
    {                        \
        DIR_gain.k0 = a;     \
        DIR_gain.k1 = b;     \
        DIR_gain.k2 = c;     \
        DIR_gain.k3 = d;     \
    } while (0)



#define SUB0 (Induct[2].Normalized_value - Induct[3].Normalized_value) //内侧
#define SUB1 (Induct[1].Normalized_value - Induct[4].Normalized_value) //中间
#define SUB2 (Induct[0].Normalized_value - Induct[5].Normalized_value) //外侧
#define SUB3 (Induct[6].Normalized_value - Induct[7].Normalized_value) //前置电感

#define ADD0 (Induct[0].Normalized_value + Induct[2].Normalized_value + \
              Induct[3].Normalized_value + Induct[5].Normalized_value)

#define ADD1(a, b, c, d) (Induct[0].Normalized_value * c + Induct[1].Normalized_value * b + \
                          Induct[2].Normalized_value * a + Induct[3].Normalized_value * a + \
                          Induct[4].Normalized_value * b + Induct[5].Normalized_value * c + \
                          Induct[6].Normalized_value * d + Induct[7].Normalized_value * d)

#define ADD2 (Induct[1].Normalized_value + Induct[4].Normalized_value)

#define MUL0 (Induct[2].Normalized_value * Induct[3].Normalized_value)
#define MUL1 (Induct[1].Normalized_value * Induct[4].Normalized_value)
#define MUL2 (Induct[0].Normalized_value * Induct[5].Normalized_value)
#define MUL3 (Induct[6].Normalized_value * Induct[7].Normalized_value)


int  Direct_error();
void Outside_judge(int16 *dir_error);

#endif