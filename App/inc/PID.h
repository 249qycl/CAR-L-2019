#ifndef __PID__H_
#define __PID__H_

#define TURN_ON  1
#define TURN_OFF 0

#define S_P 66
#define S_I 0
#define S_D 360

#define M_P 80 //17
#define M_I 8  //1
#define M_D 0

typedef struct
{
    int set_value;  //目标值【电机：速度       舵机：方向偏差量】
    int prev_error; //Error[t-2]
    int last_error; //Error[t-1]
    int16 P;
    int16 I;
    int16 D;
    int32 sum_error; //累计误差,即积分量
} PID, *pPID;

extern pPID Servo_PID; //目标值为方向偏差
extern pPID Motor_PID; //目标值为车速
extern PID Adapt_S_Kp ,Adapt_S_Kd,Adapt_M_Kp;


void All_PID_init();
int PID_Position(pPID Q, int input);
int PID_Increase(pPID Car, int input);
void PID_Handler(uint8 S_frent, uint8 M_frent);

#endif