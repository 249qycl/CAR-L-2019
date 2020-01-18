#include "common.h"
#include "include.h"

 
double Variable[20];

uint8 Uart_Send=0,SendPara,stop_contorl,send_data_contorl=0,SendSD,SD_Save=0,beep=0,SD_Write_info=0;

extern float a;
 
float *Control_Para[14] =
{/*
   &Kp,
   &Ki,
   &a,
  */

   &Angle_Inner.P,
   &Angle_Inner.D,
   &Angle_Outer.P,  //   速度参数
   &Angle_Outer.D,      //   直道消抖常数  
   &Direction_Inner.P,
   &Direction_Inner.D,
   &Direction_Outer.P,
   &Direction_Outer.D,
   &Bizhang_flag,//Yawset,     //Gyr_set,      //    电机补值 
   &Setspeed,         //Setspeed,
   &p_speed,         //Bizhang_flag,
   &i_speed,         //huan_Output,//d_d,
   &Yaw_offset,//p_speed,    
   &Gyr_set,    //i_speed,
};


void my_putchar(char temp)
{
      uart_putchar(UART0,temp); //根据实际的串口号来修改
}


/*用来通知上位机新的一组数据开始，要保存数据必须发送它*/
void Send_Begin()
{
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xae);
}

void Variable_update()
{
  Variable[0] = huan_cha;//gyr[0]; 
  Variable[1] = huan_flag;//gyr[1];         
  Variable[2] = DC_output;//CarSpeed.fCarspeed*huan_Output;//gyr[2];      
  Variable[3] = Direction_output_new;//g_nLeftMotorPulseSigma;//CarSpeed.LeftSpeed; //HY_output;//acc[0];//    
  Variable[4] = HY_lpf;//g_nRightMotorPulseSigma;//CarSpeed.RightSpeed;//Bizhang_flag;//acc[1];//     
  Variable[5] = CarSpeed.fCarspeed;//Offset_Yaw_flag;//(float)(180.0/3.1416*(atan2(acc[0],acc[2])));        
  Variable[6] = Pitch; 
  Variable[7] = ABS((uint16)(Yaw+180)%360);//dc;    //(float) CarSpeed.LeftSpeed;
  Variable[8] = CarspeedNew;//CarSigma;//Yawset;//DC_output;//g_fSpeedControlOut;//(float) CarSpeed.RightSpeed;
  Variable[9] = Yaw_flag;//CarSpeed.fCarspeed;  
  Variable[10] = AD[0];        
  Variable[11] = AD[1]; 
  Variable[12] = AD[2]; 
  Variable[13] = AD[3];
  Variable[14] = AD[4];//HY_lpf;//NewSpeedOut;  //Direction_output_new;//Gyr_Offset; 
  Variable[15] = NewSpeedOut;//Yaw_output;   //NewSpeedOut;//Angle_Inner.Tran;//Turn_Offset;
  
}


void Send_Variable(int Variable_num)
{
  uint8 i=0,ch=0;
  float temp=0;  
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xad);
  my_putchar(Variable_num);
 for(i=0;i<Variable_num;i++)
  {
    temp=Variable[i];
    ch=BYTE0(temp);
      my_putchar(ch);
    ch=BYTE1(temp);
      my_putchar(ch);
    ch=BYTE2(temp);
      my_putchar(ch);
    ch=BYTE3(temp);
      my_putchar(ch);      
  }
     my_putchar(0x0d);
}


 


void Modify_Parameter(uint8 *buff)
{
   uint8 i=0,addr=0;
   float temp;
   uint8 Parameter_num= 14; //14个可改参数
  /*          修改参数数组         */
   for(i=0;i<Parameter_num;i++)
  {
       BYTE0(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE1(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE2(temp)=*(uint8*)(buff+addr);
       addr++;
       BYTE3(temp)=*(uint8*)(buff+addr);
       addr++;
       *Control_Para[i]=temp;
   }
   
}



void Send_Parameter()
{
  uint8 i=0,ch=0;
  float temp=0;
  uint8 Parameter_num=14;  //14个可改参数
  
 
  my_putchar(0x55);
  my_putchar(0xaa);
  my_putchar(0xab);
  my_putchar(Parameter_num);
  for(i=0;i<Parameter_num;i++)
  { 
    temp=*Control_Para[i];
    ch=BYTE0(temp);
    my_putchar(ch);
    ch=BYTE1(temp);
    my_putchar(ch);
    ch=BYTE2(temp);
    my_putchar(ch);
    ch=BYTE3(temp);
    my_putchar(ch);
  }
    my_putchar(0X0b);//帧尾
}


void UART0_RX_IRQHandler()
{
	static uint8 recv;
	static uint8 data_cnt = 0;
	static uint8 Recv_Buff[100];
	static uint8 Data_Receiving = FALSE, Block_Index_Receiving = FALSE;
	if (uart_query(UART0) == 1)  uart_getchar(UART0, (char*)(&recv));  //根据实际的串口来修改
	/**********代表正在接收来自上位机的参数数据*********/
	if (Data_Receiving || Block_Index_Receiving)
	{
		if (Data_Receiving)
		{
			if (data_cnt < 56)
			{
				Recv_Buff[data_cnt] = recv;
				data_cnt++;
			}
			else
			{
				data_cnt = 0;    //达到帧长
				Data_Receiving = FALSE;
				if (recv == 0xAB)  //帧尾
				{
					Modify_Parameter(Recv_Buff);
					SendPara = 1;      //参数回传，确认参数修改完成
				}
			}
		}
		if (Block_Index_Receiving)
		{
			if (data_cnt < 1)
			{
				Recv_Buff[data_cnt] = recv;
				data_cnt++;
			}
			else
			{
				Recv_Buff[data_cnt] = recv;
				// BYTE0(Block_Index)=*(uint8*)(Recv_Buff);
				// BYTE1(Block_Index)=*(uint8*)(Recv_Buff+1);
				SendSD = TRUE;

				data_cnt = 0;    //
				//Block_Index_Receiving=FALSE;
			}
		}
	}
        
	else
	{
		beep = 1;

		switch (recv)         //判断功能字
		{
		case 0x30:           //车模启停开关 收到数据后可以控制小车的启动和停止
		//	if (Smart_Car.Status == Stop)
		//		Smart_Car.CarStartRun();//Smart_Car.Status = Run;//Car_Speed.nSetSpeed = Car_Speed.nMaxSpeed;
			/***********这里可以用来停止小车，如把速度设置为零，关闭电机的PWM等************/
		//	else
		//		Smart_Car.CarStop();//Smart_Car.Status = Stop;//Car_Speed.nSetSpeed = 0;
			/***********这里可以用来启动小车，使小车正常运行************/
		//	break;

		case 0x31:           //数据发送开关
			if (send_data_contorl == 0)
				send_data_contorl = 1;
			else
				send_data_contorl = 0;
			/*********由于数据发送时要占用部分CPU资源和运行时间，小车运行时不建议发送数据，所以可以关闭发送*********/
			break;

		case 0x32:           //读取参数
			if (SendPara == 0)
				SendPara = 1;
			break;
		case 0x33:             //修改参数
			Data_Receiving = TRUE;
			data_cnt = 0;
			break;
		case 0x34:             //保存参数
			//EEPROM_Save();
			break;
		default:           //
			break;
		}
	}
	uart_rx_irq_en(UART0);//使能串口接收中断，防止出错串口中断被关闭     
}

