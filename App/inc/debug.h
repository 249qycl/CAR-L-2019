#ifndef __DEBUG_H__
#define __DEBUG_H__

#define HOST_KEY 0

typedef struct
{
    uint8 switch_1;
    uint8 switch_2;
    uint8 switch_3;
    uint8 switch_4;
    uint8 switch_5;
} DEB;

extern DEB deb_control;

//Var[0]=DIR_gain.k2*(Induct[0].Normalized_value-Induct[5].Normalized_value);\
//Var[1]=DIR_gain.k1*(Induct[1].Normalized_value-Induct[4].Normalized_value);\
//Var[2]=DIR_gain.k0*(Induct[2].Normalized_value-Induct[3].Normalized_value);\

#define Host_Handler                    \
    do                                  \
    {                                   \
        float Var[8];                   \
        Var[0] = Ecc_Car->speed;        \
        Var[1] = Ecc_Car->M_Duty / 100; \
        Var[2] = Motor_PID->set_value;  \
        vcan_sendware(Var, sizeof Var); \
    } while (0) //【上位机16号端口】

void Variable_update();
void Send_Variable(uint8 Var_num);
void Send_Parameter();
void Uart_Handler();
void Host_hander(uint8 var_num);
#endif