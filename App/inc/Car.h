#ifndef __CAR_H__
#define __CAR_H__

extern uint8  get_times;
typedef struct 
{
    uint8   mv_mode;            //用于记录车体当前所处特殊模式状况，形成保护锁抗干扰
    uint8   Servo_mode;
    uint8   Motor_mode;
    
    uint8   outside_flag;       //出界标志
     
    uint16  times;
    int32   M_Duty;
    int32   S_Duty;
    
    int16   Delta_err;          //偏差的变化量
    int16   Dir_Error;          //方向偏差  【车体正为0  车体左偏为负 右偏为正】
    int16   Last_Error;
    
    int16   DIR_k;              //方向变化的斜率
  
    int16   speed;              //车速
}Car,*pCar;

extern pCar Ecc_Car;//创建车体

typedef struct
{
    uint8   Rt_k;               //进环垂直电感作用强度【70】
    uint8   IRt_k;              //环内运行时垂直电感作用效果
    char    ORt_k;              //出环时垂直电感作用效果
    uint8   URt_k;              //正常垂直电感作用效果
    
    uint8   in_round;           //进环标志
    int     All_ad_sum;         //水平电感总和
    float   S_duty;             //舵机调试参数
    
    int     D_adj;
    int     P_adj;
}ADJ;

extern ADJ param;


#define CAR_SPEED_HIGH          500
#define CAR_SPEED_LOW           200


/*******************************************************
*        中值1.4ms    左值1.8ms    右值1ms    频率Hz
*   duty    70          90           50        500 
*           56          72           40        400
*           35          45           25        250
*           28          36           20        200
*           14          18           10        100
*           7           9            5         50
*******************************************************/
#define Hz_50           50
#define Hz_100          100
#define Hz_200          200
#define Hz_250          250
#define Hz_300			300

#define M_FRENT         13000      //电机频率【13~17KHz为宜，避免与电感的谐振频率冲突】

#define B_CAR           0
#define L_CAR           1
#define CAR_MODEL       1         //车模型号L或B

//[B车300Hz:左：37  中：47    右：55]  
//[L车50Hz：左：5.15  中6.8 右：8.25]
#define S_FRENT         Hz_300      //舵机频率50Hz                  
#define L_VALUE         37 
#define M_VALUE         47 
#define R_VALUE         55         

void Car_speed_get(pCar Q,uint8 frequency);
void Error_update(uint8 frequency);
void Car_init(void);
void Servo_set(float duty);
void Motor_set(float duty);
void Servo_move(uint8 mode);
void Motor_move(uint8 mode);
void Car_Speed_Target();
void DIR_error_slope();
void Death_limit();
uint8 Special_action(uint8 dir,uint8 speed1,uint8 speed2,uint8 add_time,uint8 sub_time);
#endif