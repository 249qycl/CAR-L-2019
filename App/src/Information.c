#include "common.h"
#include "include.h"

DIR_ERR 	DIR_gain; //电感增益系数
T_ERR 		WZN;//仅作显示用

/****************************************************************
*
*			差比积与差比和结合计算
*
****************************************************************/
int32 Calcu_sub_mul_add(uint8 induct1, uint8 induct2, int16 gain)
{
	int32 error;
	if (induct1 < 20 && induct2 < 20)
	{
		error = (induct1 - induct2) * 3 / 2;//放大1.5倍
	}
	else
	{
		induct1 = RANGE(induct1, 0xfe, 1);
		induct2 = RANGE(induct2, 0xfe, 1);
		error = (induct1 - induct2) * abs(induct1 - induct2) * gain / induct1 / induct2;
	}
	error = RANGE(error, 1000, -999);
	return error;
}
/*************************************************************
*
*                       出界判断及防出界处理
*
*************************************************************/
void Outside_judge() //【阈值需测试】
{
	//	static uint8 keep_flag = 0,outside_flag=0;
	static int16 last_error;
	static uint16 times = 0;
	if (ADD3 < Ecc_Car->Info.threshold)
	{
		Ecc_Car->Flag.outside = 1;
		Ecc_Car->Error.now = last_error > 0 ? DIR_U_LIMIT : DIR_D_LIMIT;
		times++;
		if (times > 2000) //出界10s后停车
		{
			Ecc_Car->Flag.stop = 1;
			times = 0;
		}
	}
	//	else if (1==out_flag && (Induct[1].Nor.now > 50 && Induct[4].Nor.now > 50)||1==keep_flag)
	//	{
	//		if (0 == keep_flag)
	//		{
	//			keep_flag = 1;
	//			Ecc_Car->Error.now = last_error > 0 ? DIR_D_LIMIT : DIR_U_LIMIT;
	//		}
	//		else if (ADD0>200)//水平电感值恢复
	//			keep_flag = 0;
	//		else
	//			Ecc_Car->Error.now = last_error > 0 ? DIR_U_LIMIT : DIR_D_LIMIT;
	//	}
	else
	{
		Ecc_Car->Flag.outside = 0;
		times = 0;
	}
	last_error = Ecc_Car->Error.now;
}

/************************************************************
*
*                       环岛处理
*
************************************************************/
void Round_judge(int input)
{
#define TIMES 10
	static uint8 limit = TIMES, Round_move = 0, re_judge = 0;
	static int16 max_input = 0, re_times = 0;
	static uint32 speed_sum = 0, times = 0;

	/*****************阈值校准************************************/
	if (1 == Ecc_Car->Flag.calibrat)
	{
		Ecc_Car->Info.threshold = ADD3 / 3;
		Ecc_Car->Round.threshold = Ecc_Car->Info.threshold * 4;
		Ecc_Car->Flag.calibrat = 0;
	}
	/******************************************************/

	if (0 == re_times)
	{
		switch (Ecc_Car->Round.state)
		{
		case 0: //防坡道误判
			times = (input > Ecc_Car->Round.threshold && (ADD5 - ADD4) < Ecc_Car->Info.threshold && 0 == re_judge) ? (abs(SUB1) > 10
																														  ? (times + 1)
																														  : 0)
																												   : 0;
			if (times > limit)
			{
				Ecc_Car->Protect.round = 3000; //18s保护
				Ecc_Car->Round.state = 1;
				Ecc_Car->Flag.buzzer = 1;
				re_judge = 1;
				Round_move = 1; //抗扭动
				times = 0;
			}
			break;

		case 1:
			/******************************************************/
			if (input < max_input && input > Ecc_Car->Round.threshold) //防误判
				times++;
			else
			{
				times = 0;
				max_input = input; //对状态建立后的值进行最大值追踪
			}

			if (times > limit) //*3/2
			{
				if (max_input < (5 * Ecc_Car->Info.threshold))
				{
					Ecc_Car->Round.state = 0;
					Round_move = 0;
				}
				else
				{
					Ecc_Car->Round.state = 2;
					Ecc_Car->Flag.buzzer = 1;
					Round_move = 2; //套用差比积进环

					times = 0;
					max_input = 0;
				}
			}
			break;
		case 2:
			//差比积辅助入环
			Induct[1].Nor.now = RANGE(Induct[1].Nor.now, 200, 1); //把垂直电感纳入差比积运算范围
			Induct[4].Nor.now = RANGE(Induct[4].Nor.now, 200, 1);
			while (Induct[1].Nor.now < 20 && Induct[4].Nor.now < 20)
			{
				Induct[1].Nor.now = 2 * Induct[1].Nor.now;
				Induct[4].Nor.now = 2 * Induct[4].Nor.now;
			}
			speed_sum += Ecc_Car->Speed.now; //速度积分出路程
			if (speed_sum > 50000)
			{
				Round_move = 3; //环内运行
				Ecc_Car->Round.state = 3;
				Ecc_Car->Flag.buzzer = 1;
				speed_sum = 0;
				re_times = 400; //2s内不再响应状态选择
				times = 0;
			}
			break;

		case 3:
			times = (input > Ecc_Car->Round.threshold) ? (times + 1) : 0; //|| (Induct[1].Nor.now > 30 && Induct[4].Nor.now > 30)
			if (times > limit)											  //到达最后一个拐点处，即将出环
			{
				Ecc_Car->Round.state = 4;
				Ecc_Car->Flag.buzzer = 1;

				Round_move = 4;
				times = 0;
			}
			break;

		case 4: //追踪最大值后出环
			if (input < max_input && input > Ecc_Car->Round.threshold)
			{
				times++;
				times = RANGE(times, 200, 1);
			}
			else
			{
				times = 0;
				max_input = input; //对状态建立后的值进行最大值追踪
			}
			if ((times > limit && max_input > (5 * Ecc_Car->Info.threshold)) || ADD0 < Ecc_Car->Info.threshold)
			{
				Ecc_Car->Round.state = 5;
				Ecc_Car->Flag.buzzer = 1;
				Round_move = 4; //  [出环处理]
				max_input = 0;
				times = 0;
			}
			break;
		case 5:
			speed_sum += Ecc_Car->Speed.now; //速度积分出路程
			if (speed_sum > 50000)
			{
				Ecc_Car->Round.state = 6;
				Ecc_Car->Flag.buzzer = 1;
				speed_sum = 0;
			}
		case 6:
			times = input < Ecc_Car->Round.threshold ? (times + 1) : 0;
			if (times > limit)
			{
				Ecc_Car->Round.state = 0;
				Ecc_Car->Flag.buzzer = 1;
				times = 0;
				Round_move = 0;
				re_judge = 1;
				re_times = 600;
			}
			break;
		}
	}
	else
		re_times--;

	if (input < Ecc_Car->Round.threshold)
		re_judge = 0;

	switch (Round_move)
	{
	case 0:
		GAIN_SET(L_GAIN, M_GAIN, R_GAIN);
		break; //正常行驶
	default:
		GAIN_SET(Ecc_Car->Round.Debug.W[Round_move - 1], Ecc_Car->Round.Debug.Z[Round_move - 1], Ecc_Car->Round.Debug.N[Round_move - 1]);
		break;
	}
}

/************************************************
*
*       求取方向偏差，并返回
*
************************************************/
int Direct_error()
{
	int32 DIR_error, Error0, Error1, Error2;

	Normalized(); //数值归一化处理
	Ecc_Car->Round.ad_sum = ADD0;

	if (0 == Ecc_Car->Flag.obstacle)
		Round_judge(Ecc_Car->Round.ad_sum); //室外420

	Error0 = Calcu_sub_mul_add(Induct[0].Nor.now, Induct[5].Nor.now, DIR_gain.k0 * 90); //90
	Error1 = Calcu_sub_mul_add(Induct[1].Nor.now, Induct[4].Nor.now, DIR_gain.k1 * 110);
	Error2 = Calcu_sub_mul_add(Induct[2].Nor.now, Induct[3].Nor.now, DIR_gain.k2 * 90); //90
	DIR_error = Error0 + Error1 + Error2;
	/*用于观察的数据*/
	WZN.W = Error0;
	WZN.Z = Error1;
	WZN.N = Error2;
	DIR_error = RANGE(DIR_error, DIR_U_LIMIT, DIR_D_LIMIT); //偏差限幅

	return DIR_error;
}