#ifndef __FUZZY_H__
#define __FUZZY_H__

#define DIR_RANGE       200     //方向偏差的范围
#define SERVO_RANGE     200     //舵机输出的范围
extern float Motor_duty[7];
extern const float RuleDuty[7][7];

#define NB      0
#define NM      1
#define NS      2
#define ZE      3
#define PS      4
#define PM      5
#define PB      6

float fuzzy(float range,float e,float ec,const float (*Rule)[7],float *object);

#endif