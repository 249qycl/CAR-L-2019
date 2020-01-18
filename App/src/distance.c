#include "common.h"
#include "include.h"


void Distance_init()
{
    gpio_init(ULT_PORT_TX_0, GPO, 0);
    gpio_init(ULT_PORT_RX_0, GPI, 0);
    port_init_NoALT(ULT_PORT_RX_0, IRQ_EITHER | PULLDOWN);
    gpio_init(LASER_PORT1, GPI, 0);
    port_init_NoALT(LASER_PORT1, PULLUP); //激光低电平有效，上拉保证无干扰
    gpio_init(LASER_PORT2, GPI, 0);
    port_init_NoALT(LASER_PORT1, PULLUP);
    gpio_init(HOARE_PORT, GPI, 0);
    port_init_NoALT(HOARE_PORT, IRQ_RISING | PULLDOWN); //霍尔元件，高电平有效，下拉保证无干扰
}

void Distance_send()
{
    static uint8 times = 0;
    if (times > 4)
    {
        gpio_turn(ULT_PORT_TX_0);
        DELAY_US(10);
        gpio_turn(ULT_PORT_TX_0);
        Ecc_Car->Distance.state = 0;
        Ecc_Car->Distance.time = 0;

        times = 0;
    }
    else
        times++;
}

void Distance_measure() //超声波解算
{
    static uint8 times = 0;

    if (2 == Ecc_Car->Distance.state)
    {
        Ecc_Car->Distance.length = Ecc_Car->Distance.time * 17 / 1000;
    }
    else
    {
        if (pit_time_get_us(ULT_TIMER_0) > 6000)
        {
            pit_close(ULT_TIMER_0);
            Ecc_Car->Distance.state = 3;
            Ecc_Car->Distance.length = 102; //85
        }
    }

    times = Ecc_Car->Distance.length < 102 ? (Ecc_Car->Distance.length > 10 ? (times + 1) : 0) : 0;
    if (times > 5)
        times = 5;
    else
        Ecc_Car->Distance.length = 102;

    Ecc_Car->Distance.length = RANGE(Ecc_Car->Distance.length, 102, 2);
}

/***************************************************************
*
*					避障运动
*	250		10000	-4000	-9000
*
***************************************************************/

void Obstacle_avoidance() //【大偏差锁死】
{

    static uint8 times = 0, re_judge = 0;
    switch (Ecc_Car->Flag.obstacle)
    {
    case 0:                                                                                                                                    //50  80
        if (1 == Ecc_Car->Flag.laser1 && (Ecc_Car->Distance.length > 40) && (Ecc_Car->Distance.length < 80) && (abs(Ecc_Car->Error.now) < 100) && 0 == re_judge) //&&(0==re_times==0)
            times++;
        else
            times = 0;
        if (times > 10) //至少两次值成立，即50ms
        {
            Ecc_Car->Flag.obstacle = 1;
            Ecc_Car->Flag.buzzer = 1;
            Ecc_Car->Protect.obstacle = 1800; //9s保护
            times = 0;
        }
        break;
    case 1:
        if (ADD3 < 100)
        {
            times = 1 == Ecc_Car->Flag.laser2 ? (times + 1) : 0;
            if (times > 10)
            {
                Ecc_Car->Flag.obstacle = 2;
                Ecc_Car->Flag.buzzer = 1;
                times = 0;
            }
        }
        Ecc_Car->Duty.servo = Ecc_Car->Duty.obstacle[0];
        break;
    case 2:
        Ecc_Car->Duty.servo = Ecc_Car->Duty.obstacle[1];
        times = 1 == Ecc_Car->Flag.laser2 ? (times + 1) : 0;
        if (times > 10)
        {
            times = 0;
            Ecc_Car->Flag.obstacle = 3;
        }
        break;
    case 3:
        Ecc_Car->Duty.servo = Ecc_Car->Duty.obstacle[2];
        //		if(Induct[1].Nor.now>50&&Induct[2].Nor.now>50)//垂直情况
        //			Ecc_Car->Duty.servo = Ecc_Car->Duty.obstacle[0];
        //        else
        if (ADD3 > Ecc_Car->Info.threshold && Ecc_Car->Error.now > 10)
        {
            Ecc_Car->Flag.buzzer = 1;
            Ecc_Car->Flag.obstacle = 0;
            re_judge = 1;
        }
        break;
    }
    if (Ecc_Car->Distance.length > 100)
        re_judge = 0;
}

void PORTE_Handler()
{
    if (PORTE_ISFR & (1 << ULT_ID0)) //写1清中断标志位
    {
        if (0 == Ecc_Car->Distance.state)
        {
            pit_time_start(ULT_TIMER_0);
            Ecc_Car->Distance.state = 1;
        }
        else if (1 == Ecc_Car->Distance.state)
        {
            Ecc_Car->Distance.time = pit_time_get_us(ULT_TIMER_0);
            pit_close(ULT_TIMER_0);
            Ecc_Car->Distance.state = 2;
        }
        PORTE_ISFR = (1 << ULT_ID0);
    }
    else if (PORTE_ISFR & (1 << HOARE_ID))
    {
        Ecc_Car->Flag.hoare = 1;
        PORTE_ISFR = (1 << HOARE_ID);
    }
}