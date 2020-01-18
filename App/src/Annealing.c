#include "common.h"
#include "include.h"

typedef struct
{
    uint8  Find_finish;
    int16  P;
    int16  I;
    uint64 last_result;//最优和
    uint32 ITAE_t;//时间
    uint64 ITAE_value;//值
}ANNEAL;

ANNEAL Anneal;

void Anneal_init()
{
  Anneal=(ANNEAL){.P=1,.I=1,.last_result=0xffffffffffffffff};
  Motor_PID->set_value=300;
}

//ITAE标准，累计t*e(t)的值
//按评价标准进行迭代求和 
void Evaluation(int16 Error)
{
    Anneal.ITAE_value+=Anneal.ITAE_t*abs(Error);    
    Anneal.ITAE_t++;
    if(Anneal.ITAE_t==1500)
        Motor_PID->set_value=500;
}

void Creat_new()
{
    static uint8 i=1,j=5;
    
    Motor_PID->P=i;
    Motor_PID->I=j;
    Motor_PID->sum_error=0;
    
    if(j++>20)
    {
        j=0;
        i++;
    }
    
    Motor_PID->set_value=300;
    Anneal.ITAE_value=0;
    Anneal.ITAE_t=0;
    
    if(i>250&&j>20)
        Anneal.Find_finish=1;
}

void Anneal_run()
{
    static uint8 L_list=0;
    static uint8 times=0;
    if(Anneal.Find_finish==0)
    {            
        if(L_list>0)
        {
            if( Anneal.ITAE_value>Anneal.last_result )//结果恶化
            {
                times++;
                Motor_PID->set_value=300;
                Anneal.ITAE_value=0;
                Anneal.ITAE_t=0;
                if(times>4)//连续恶化，跳出本次搜索
                    L_list=1;
            }
            else
            {
                times=0;
                Anneal.P=Motor_PID->P;//更新最优参数
                Anneal.I=Motor_PID->I;
                Anneal.last_result=Anneal.ITAE_value;
                Motor_PID->set_value=300;
                Anneal.ITAE_value=0;
                Anneal.ITAE_t=0;
            }    
            L_list--;
        }
        if(L_list==0)
        {
            L_list=5;//链长，每个参数的运行次数
            times=0;
            Creat_new();
        }
    }
}
//    if(TEST_key==1)
//    {
//        if(Anneal_time>0)
//            Ecc_Car->Motor_mode=9;//停车
//        else
//        {
//            Motor_PID_Handler;
//            Evaluation(Motor_PID->set_value - Ecc_Car->speed);
//            if( Anneal_flag==0 )
//            {
//                Anneal_flag=1;
//                Anneal_run();
//                Anneal_time=200;//3s
//            }
//            Ecc_Car->Motor_mode=0;
//        }
//        if(Anneal.ITAE_t>2000||Anneal.ITAE_value>Anneal.last_result)//运行停止条件
//            Anneal_flag=0;
//        
//        Move_Handler;
//    
//        Anneal_time=Anneal_time>0?(Anneal_time-1):Anneal_time;
//    }