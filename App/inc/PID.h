#ifndef __PID__H_
#define __PID__H_

#define TURN_ON  1
#define TURN_OFF 0

#define S_P 66
#define S_I 0
#define S_D 360

#define S_LIMIT 100
#define S_PIDMODE TURN_OFF     //舵机积分分离开关
#define S_DEAD_LIMIT 10        //死区大小
#define S_DEAD_SWITCH TURN_OFF //舵机死区设立开关

#define M_P 80 //17
#define M_I 8  //1
#define M_D 0
#define M_LIMIT 100
#define M_PIDMODE TURN_OFF     //电机积分分离开关
#define M_DEAD_LIMIT 2         //死区大小
#define M_DEAD_SWITCH TURN_OFF //电机死区设立开关

typedef struct
{
    int set_value;  //目标值【电机：速度       舵机：方向偏差量】
    int prev_error; //Error[t-2]
    int last_error; //Error[t-1]
    int16 P;
    int16 I;
    int16 D;
    int32 sum_error; //累计误差,即积分量

    int last_out;
    uint16 error_limit;    //进行积分分离时的偏差界限
    uint8 saturation_flag; //输出饱和标志
} PID, *pPID;

extern pPID Servo_PID; //目标值为方向偏差
extern pPID Motor_PID; //目标值为车速

typedef struct
{
    uint8 TH;    //启动棒棒控制的阈值
    uint8 P_Th0; //比例系数分段阈值
    uint8 P_Th1;
    uint8 P_Th2;
    uint8 P_Th3;

    uint16 D_Th0; //微分系数分段阈值
    uint16 D_Th1;
    uint16 D_Th2;
    uint16 D_Th3;

    int16 P0;
    int16 D0;

    int16 P1;
    int16 D1;

    int16 P2;
    int16 D2;

    int16 P3;
    int16 D3;
} SecPID;
extern SecPID Servo_pid;

#define SEC_JUDGE(x, y)     (temp > Q->x##_Th##y)
#define SYMBOL(x)           do{x = Ecc_Car->Dir_Error >= 0 ? x : (-x);}while(0)

void All_PID_init();
int PID_Position(pPID Q, int input);
int PID_Increase(pPID Q, int input);
int Section_PID(SecPID *Q, pPID T, int16 input1, int16 input2);
void PID_Handler(uint8 S_frent, uint8 M_frent);

#endif