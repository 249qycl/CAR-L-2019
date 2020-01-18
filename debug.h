#ifndef __DEBUG_H__
#define __DEBUG_H__
#define Vol_ratio 3.3/4096*5.7
extern double Variable[20];
extern double Parameter[20];

//取一个数据的各个位
#define BYTE0(Temp)       (*(char *)(&Temp))     
#define BYTE1(Temp)       (*((char *)(&Temp) + 1))
#define BYTE2(Temp)       (*((char *)(&Temp) + 2))
#define BYTE3(Temp)       (*((char *)(&Temp) + 3))

extern float *Control_Para[14];
extern uint8 send_data,SendPara,stop_contorl,send_data_contorl,SendSD,SD_Save,beep,SD_Write_info;

extern uint8 Starting;
extern int  Start_Cnt;
extern  uint8 Page_Index,Para_Index,Para_Checked,OLED_Refresh,Camera_Drawed;
extern uint8 send_data,Uart_Send;

 
extern  void UART0_RX_IRQHandler(); 
 
extern void Check_BottonPress();

extern void Variable_update();
extern void Para_Update();
extern void Send_SD();
extern void Send_Begin();
extern void Send_Parameter();
extern void Send_Variable(int Variable_num);
extern void Modify_Parameter(uint8 *buff);


#endif 