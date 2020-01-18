#ifndef __CAR_H__
#define __CAR_H__


typedef struct
{
	struct
	{
		uint8 servo;
		uint8 motor;
		uint8 move;			//用于记录车体当前所处特殊模式状况
	}Mode;
	
	struct
	{	
		uint8 protect:1;	//驱动保护
		uint8 stop :1;		//永久停车
		uint8 outside :1;	//出界标志
		uint8 buzzer :1;	//蜂鸣器
		uint8 hoare :1;		//霍尔传感器
		uint8 calibrat:1;	//阈值校准标志
		uint8 obstacle:2;	//障碍物
		uint8 laser1:1;		//激光1
		uint8 laser2:1;		//激光2
	}Flag;
   
	struct
	{
		int32 motor;
		int32 servo;
		int32 obstacle[3];
		float debug;
	}Duty;

	struct
	{
		int16 delta;	//偏差的变化量
		int16 now;		//当前方向偏差  【车体正为0  车体左偏为负 右偏为正】
		int16 last;
	}Error;
	
	struct
	{
		uint8  state;
    	uint16 length;
    	uint32 time;
	}Distance;
	
	struct
	{
		uint8  finish; 		//扫描结束标志
    	uint16 times;      //扫描次数
	}Scan;
	
	struct
	{	
		uint8  state;		
		uint16 threshold;		
		int32  ad_sum;//水平电感总和
		struct
		{
			int16 W[4];
			int16 Z[4];
			int16 N[4];
		}Debug;
	}Round;
	
	struct//对相关元素进行防失败保护处理
	{
		uint32 round;
		uint32 obstacle;
	}Protect;
	
	struct
	{
		int16  threshold;//环境电感信息强度阈值		
	}Info;
	
	struct
	{
		int16 now;//当前速度测量值
		int16 standard;//正常速度
		int16 round;//环岛速度
		int16 obstacle;//避障速度
		int16 outside;//出界速度
	}Speed;
	
	enum
	{
		Hz_50=50,
		Hz_100=100,
		Hz_200=200,
		Hz_250=250,
		Hz_300=300,
		S_FREQ=Hz_50,//舵机频率50Hz		
	}SERVO;
	enum
	{
		M_FREQ=13573,//电机频率【13~17KHz为宜，避免与电感的谐振频率冲突】
	}MOTOR;    
} Car, *pCar;

extern pCar Ecc_Car; //创建车体


/*******************************************************
*        中值1.4ms    左值1.8ms    右值1ms    频率Hz
*   duty    70          90           50        500 
*           56          72           40        400
*           35          45           25        250
*           28          36           20        200
*           14          18           10        100
*           7           9            5         50
*******************************************************/
//[B车300Hz:左：37  中：47    右：55]
//[L车50Hz：左：5.15  中6.8 右：8.25]
#define L_VALUE 4.8f
#define M_VALUE 6.4f
#define R_VALUE 8.0f 



void Car_speed_get(pCar Car, uint8 T);
void Error_update(uint8 T);
void Car_init(void);
void Servo_set(float duty);
void Motor_set(float duty);
void Servo_move(uint8 mode);
void Motor_move(uint8 mode);
void Buzzer();
void Hoare_process();
void Car_Speed_Target();
#endif