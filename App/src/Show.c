#include "common.h"
#include "include.h"

SHOW Show = {
    .state = 0,
    .PIT_over = 0,
    .color = {FONT_C, GREEN, RED},
    .Gear = {
        .chose = 3,
        .gear = 1.0f,
    },
};
KEY_MSG_t keymsg;  //按键消息队列
flash_data_t data; //flash初始化信息
Data md;           //flash数据

uint8 key_num;

//float Show.Gear.gear= 1.0;                 //挡位
//char  Show.Gear.chose=3;             //挡位选择
Node Tree[7];
/******************************************************************************/
#define EVENT1                                                                               \
    {                                                                                        \
        Ecc_Car->Flag.stop = Tree[i].temp != 0 ? (!Ecc_Car->Flag.stop) : Ecc_Car->Flag.stop; \
    }

#define EVENT2                                         \
    {                                                  \
        Servo_PID->P += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT3                                         \
    {                                                  \
        Adapt_S_Kp.P += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT4                                         \
    {                                                  \
        Adapt_S_Kp.I += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT5                                         \
    {                                                  \
        Adapt_S_Kp.D += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT6                                         \
    {                                                  \
        Servo_PID->D += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT7                                         \
    {                                                  \
        Adapt_S_Kd.P += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT8                                         \
    {                                                  \
        Adapt_S_Kd.I += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT9                                         \
    {                                                  \
        Adapt_S_Kd.D += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT10 \
    {           \
    }

#define EVENT11 \
    {           \
    }

#define EVENT12 \
    {           \
    }

/******************************************************************************/
#define EVENT13                                               \
    {                                                         \
        Ecc_Car->Duty.motor += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT14                                        \
    {                                                  \
        Motor_PID->P += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT15                                        \
    {                                                  \
        Motor_PID->I += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT16                                        \
    {                                                  \
        Motor_PID->D += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT17                                                \
    {                                                          \
        Motor_PID->set_value += Tree[i].temp * Show.Gear.gear; \
    }
#define EVENT18                                        \
    {                                                  \
        Adapt_M_Kp.P += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT19                                        \
    {                                                  \
        Adapt_M_Kp.I += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT20                                        \
    {                                                  \
        Adapt_M_Kp.D += Tree[i].temp * Show.Gear.gear; \
    }
#define EVENT21 \
    {           \
    }

#define EVENT22 \
    {           \
    }

#define EVENT23 \
    {           \
    }

#define EVENT24 \
    {           \
    }

/******************************************************************************/
#define EVENT25                                         \
    {                                                   \
        Show.Gear.chose += Tree[i].temp;                \
        Show.Gear.chose = RANGE(Show.Gear.chose, 6, 0); \
        gear_select(Show.Gear.chose);                   \
    }

#define EVENT26                                               \
    {                                                         \
        Ecc_Car->Duty.debug += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT27                                                                                          \
    {                                                                                                    \
        Ecc_Car->Flag.calibrat = Tree[i].temp != 0 ? (!Ecc_Car->Flag.calibrat) : Ecc_Car->Flag.calibrat; \
    }

#define EVENT28                                    \
    {                                              \
        Ecc_Car->Round.Debug.W[0] += Tree[i].temp; \
    }

#define EVENT29                                    \
    {                                              \
        Ecc_Car->Round.Debug.Z[0] += Tree[i].temp; \
    }

#define EVENT30                                    \
    {                                              \
        Ecc_Car->Round.Debug.N[0] += Tree[i].temp; \
    }

#define EVENT31                                    \
    {                                              \
        Ecc_Car->Round.Debug.W[1] += Tree[i].temp; \
    }

#define EVENT32                                    \
    {                                              \
        Ecc_Car->Round.Debug.Z[1] += Tree[i].temp; \
    }

#define EVENT33                                    \
    {                                              \
        Ecc_Car->Round.Debug.N[1] += Tree[i].temp; \
    }

#define EVENT34                                    \
    {                                              \
        Ecc_Car->Round.Debug.W[2] += Tree[i].temp; \
    }

#define EVENT35                                    \
    {                                              \
        Ecc_Car->Round.Debug.Z[2] += Tree[i].temp; \
    }

#define EVENT36                                    \
    {                                              \
        Ecc_Car->Round.Debug.N[2] += Tree[i].temp; \
    }

#define EVENT37                                    \
    {                                              \
        Ecc_Car->Round.Debug.W[3] += Tree[i].temp; \
    }

#define EVENT38                                    \
    {                                              \
        Ecc_Car->Round.Debug.Z[3] += Tree[i].temp; \
    }

#define EVENT39                                    \
    {                                              \
        Ecc_Car->Round.Debug.N[3] += Tree[i].temp; \
    }

#define EVENT40                                                     \
    {                                                               \
        Ecc_Car->Duty.obstacle[0] += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT41                                                     \
    {                                                               \
        Ecc_Car->Duty.obstacle[1] += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT42                                                     \
    {                                                               \
        Ecc_Car->Duty.obstacle[2] += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT43                                                   \
    {                                                             \
        Ecc_Car->Speed.obstacle += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT44                                                \
    {                                                          \
        Ecc_Car->Speed.round += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT45                                                  \
    {                                                            \
        Ecc_Car->Speed.outside += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT46                                                   \
    {                                                             \
        Ecc_Car->Speed.standard += Tree[i].temp * Show.Gear.gear; \
    }

#define EVENT47 \
    {           \
    }

#define EVENT48 \
    {           \
    }

#define EVENT49 \
    {           \
    }

#define EVENT50 \
    {           \
    }

#define SCOPE_DIV(x)                                                                                                                                                                                                                            \
    do                                                                                                                                                                                                                                          \
    {                                                                                                                                                                                                                                           \
        Scope.divide[x] = Scope.gear[x] > 5 ? (Scope.gear[x] > 10 ? (Scope.gear[x] > 15 ? (Scope.gear[x] > 20 ? 60000 : ((Scope.gear[x] - 15) * 10000)) : ((Scope.gear[x] - 10) * 2000)) : ((Scope.gear[x] - 5) * 200)) : (Scope.gear[x] * 20); \
    } while (0)

#define CAR_EVENT0                                                \
    {                                                             \
        Scope.key = Tree[i].temp != 0 ? (!Scope.key) : Scope.key; \
    }
#define CAR_EVENT1                                            \
    {                                                         \
        Scope.cursor_seat += Tree[i].temp * Scope.step;       \
        Scope.cursor_seat = RANGE(Scope.cursor_seat, 238, 1); \
    }
#define CAR_EVENT2                                                   \
    {                                                                \
        Scope.stop = Tree[i].temp != 0 ? (!Scope.stop) : Scope.stop; \
    }
#define CAR_EVENT3                  \
    {                               \
        Scope.step += Tree[i].temp; \
    }
#define CAR_EVENT4                                                                        \
    {                                                                                     \
        Scope.auto_switch = Tree[i].temp != 0 ? (!Scope.auto_switch) : Scope.auto_switch; \
    }

#define CAR_EVENT5                                                               \
    {                                                                            \
        Scope.state[0] = Tree[i].temp != 0 ? (!Scope.state[0]) : Scope.state[0]; \
    }

#define CAR_EVENT6                                                               \
    {                                                                            \
        Scope.state[1] = Tree[i].temp != 0 ? (!Scope.state[1]) : Scope.state[1]; \
    }

#define CAR_EVENT7                                                               \
    {                                                                            \
        Scope.state[2] = Tree[i].temp != 0 ? (!Scope.state[2]) : Scope.state[2]; \
    }

#define CAR_EVENT8                                                               \
    {                                                                            \
        Scope.state[3] = Tree[i].temp != 0 ? (!Scope.state[3]) : Scope.state[3]; \
    }

#define CAR_EVENT9                                                               \
    {                                                                            \
        Scope.state[4] = Tree[i].temp != 0 ? (!Scope.state[4]) : Scope.state[4]; \
    }
#define CAR_EVENT10                                  \
    {                                                \
        Scope.gear[0] += Tree[i].temp;               \
        Scope.gear[0] = RANGE(Scope.gear[0], 20, 1); \
        SCOPE_DIV(0);                                \
    }
#define CAR_EVENT11                                  \
    {                                                \
        Scope.gear[1] += Tree[i].temp;               \
        Scope.gear[1] = RANGE(Scope.gear[1], 20, 1); \
        SCOPE_DIV(1);                                \
    }

#define CAR_EVENT12                                  \
    {                                                \
        Scope.gear[2] += Tree[i].temp;               \
        Scope.gear[2] = RANGE(Scope.gear[2], 20, 1); \
        SCOPE_DIV(2);                                \
    }

#define CAR_EVENT13                                  \
    {                                                \
        Scope.gear[3] += Tree[i].temp;               \
        Scope.gear[3] = RANGE(Scope.gear[3], 20, 1); \
        SCOPE_DIV(3);                                \
    }

#define CAR_EVENT14                                  \
    {                                                \
        Scope.gear[4] += Tree[i].temp;               \
        Scope.gear[4] = RANGE(Scope.gear[4], 20, 1); \
        SCOPE_DIV(4);                                \
    }

#define CAR_EVENT15 \
    {               \
    }

/*********************************************************
*
*       对用于辅助关系操作的关系树进行初始化
*
*********************************************************/
void Tree_init()
{
    /*亲子代归属关系建立*/
    Tree[0].parents = -1;
    Tree[1].parents = 0; //induct
    Tree[2].parents = 0; //other
    Tree[3].parents = 0; //pid
    Tree[4].parents = 3; //1
    Tree[5].parents = 3; //2
    Tree[6].parents = 3; //3

    /*子代筛选及限幅参数*/
    Tree[0].Grade.up = 3;
    Tree[0].Grade.down = 1;
    Tree[0].Grade.now = 1;

    Tree[2].Grade.up = 15;
    Tree[2].Grade.down = 0;
    Tree[2].Grade.now = 0;

    Tree[3].Grade.up = 6;
    Tree[3].Grade.down = 4;
    Tree[3].Grade.now = 4;

    Tree[4].Grade.up = 11;
    Tree[4].Grade.down = 0;
    Tree[4].Grade.now = 0;

    Tree[5].Grade.up = 11;
    Tree[5].Grade.down = 0;
    Tree[5].Grade.now = 0;

    Tree[6].Grade.up = 25;
    Tree[6].Grade.down = 0;
    Tree[6].Grade.now = 0;

    /*触底保护特性*/
    Tree[0].Flag.only = 0;
    Tree[1].Flag.only = 1;
    Tree[2].Flag.only = 0;
    Tree[3].Flag.only = 0;
    Tree[4].Flag.only = 1;
    Tree[5].Flag.only = 1;
    Tree[6].Flag.only = 1;
    /*显示界面编号*/
    Tree[0].show_mode = 0;
    Tree[1].show_mode = 1; //电感信息展示
    Tree[2].show_mode = 2; //其他信息展示
    Tree[3].show_mode = 3; //PID种类信息展示
    Tree[4].show_mode = 4; //第一类PID
    Tree[5].show_mode = 5; //第二类PID
    Tree[6].show_mode = 6; //第三类PID

    /*是否具有变量修改权限*/
    Tree[0].Flag.wr = 0; //只可读
    Tree[1].Flag.wr = 0; //只读,仅作显示用（不响应key==3）
    Tree[2].Flag.wr = 1; //【0】
    Tree[3].Flag.wr = 0; //只读，仅作显示用（不响应key==3）
    Tree[4].Flag.wr = 1;
    Tree[5].Flag.wr = 1;
    Tree[6].Flag.wr = 1;

    for (uint8 i = 0; i < 7; i++)
    {
        Tree[i].temp = 0;
        Tree[i].child = Tree[i].Grade.down;
        Tree[i].Flag.ok = 0;
        Tree[0].Flag.state = 0; //子代存在状态
    }
}
/**************************************************
*
*       调节幅度的选择
*
**************************************************/
void gear_select(char gear)
{
    switch (gear)
    {
    case 0:
        Show.Gear.gear = 0.001;
        break;
    case 1:
        Show.Gear.gear = 0.01;
        break;
    case 2:
        Show.Gear.gear = 0.1;
        break;
    case 3:
        Show.Gear.gear = 1;
        break;
    case 4:
        Show.Gear.gear = 10;
        break;
    case 5:
        Show.Gear.gear = 100;
        break;
    case 6:
        Show.Gear.gear = 1000;
        break;
    }
}
/***************************************************
*
*       服务函数，用于搜索最底层的子代
*
***************************************************/
uint8 find()
{
    uint8 i = 0;
    while (1 == Tree[i].Flag.state)
    {
        if (1 == Tree[i].Flag.only)
            break;
        i = Tree[i].child;
    }
    return i;
}
/*********************************************************
*
*               从消息队列中读取按键信息到key_num
*
*********************************************************/
void key_num_get()
{
    if (1 == get_key_msg(&keymsg))
    {
        if (1 == keymsg.status) //短按
        {
            switch (keymsg.key)
            {
            case 0:
                key_num = 1;
                break;
            case 1:
                key_num = 2;
                break;
            case 2:
                key_num = 3;
                break;
            case 3:
                key_num = 4;
                break;
            }
            keymsg.status = KEY_DOWN;
        }
    }
    if (key_num != 0)
        Show.state = 0;
}
/*************************************************
*
*       按键操作处理及对要调节的变量值进行更新
*
*************************************************/
void key_process(uint8 i, uint8 key)
{
    uint8 event;
    key_num = 0;
    switch (key)
    {
    case 1:
        if (1 == Tree[i].Flag.wr && 1 == Tree[i].Flag.ok)
            Tree[i].temp++;
        else
            Tree[i].Grade.now--;
        break;

    case 2:
        if (1 == Tree[i].Flag.wr && 1 == Tree[i].Flag.ok)
            Tree[i].temp--;
        else
            Tree[i].Grade.now++;
        break;

    case 3:
        if (1 == Tree[i].Flag.wr && 0 == Tree[i].Flag.ok)
            Tree[i].Flag.ok = 1;
        else
        {
            if (2 == i && 1 == Scope.key)
                break;
            Tree[i].Flag.state = 1;
            Tree[i].child = Tree[i].Grade.now; //通过grade访问相应子空间
        }
        break;

    case 4:
        if (1 == Tree[i].Flag.ok)
            Tree[i].Flag.ok = 0;
        else
        {
            if (2 == i && 1 == Scope.key)
                break;
            Tree[Tree[i].parents].Flag.state = 0;
            Tree[i].Grade.now = Tree[i].Grade.down;
            Tree[i].Flag.ok = 0;
        }
        if (0 == i)
        {
            my_data_save(); //写入flash操作
            DELAY_MS(1);
            lcd_clear(BLACK);
            lcd_STR_ench(85, 150, YELLOW, BLACK, "写入成功！");
        }
        break;
    }
    /*限幅*/
    Tree[i].Grade.now = RANGE(Tree[i].Grade.now, Tree[i].Grade.up, Tree[i].Grade.down);
    if (i < 7 && i > 3) //可写页面的数据更新
    {
        event = (i - 4) * 12 + Tree[i].Grade.now + 1;
        switch (event)
        {
        case 1:
            EVENT1;
            break;
        case 2:
            EVENT2;
            break;
        case 3:
            EVENT3;
            break;
        case 4:
            EVENT4;
            break;
        case 5:
            EVENT5;
            break;
        case 6:
            EVENT6;
            break;
        case 7:
            EVENT7;
            break;
        case 8:
            EVENT8;
            break;
        case 9:
            EVENT9;
            break;
        case 10:
            EVENT10;
            break;
        case 11:
            EVENT11;
            break;
        case 12:
            EVENT12;
            break;

        case 13:
            EVENT13;
            break;
        case 14:
            EVENT14;
            break;
        case 15:
            EVENT15;
            break;
        case 16:
            EVENT16;
            break;
        case 17:
            EVENT17;
            break;
        case 18:
            EVENT18;
            break;
        case 19:
            EVENT19;
            break;
        case 20:
            EVENT20;
            break;
        case 21:
            EVENT21;
            break;
        case 22:
            EVENT22;
            break;
        case 23:
            EVENT23;
            break;
        case 24:
            EVENT24;
            break;

        case 25:
            EVENT25;
            break;
        case 26:
            EVENT26;
            break;
        case 27:
            EVENT27;
            break;
        case 28:
            EVENT28;
            break;
        case 29:
            EVENT29;
            break;
        case 30:
            EVENT30;
            break;
        case 31:
            EVENT31;
            break;
        case 32:
            EVENT32;
            break;
        case 33:
            EVENT33;
            break;
        case 34:
            EVENT34;
            break;
        case 35:
            EVENT35;
            break;
        case 36:
            EVENT36;
            break;
        case 37:
            EVENT37;
            break;
        case 38:
            EVENT38;
            break;
        case 39:
            EVENT39;
            break;
        case 40:
            EVENT40;
            break;
        case 41:
            EVENT41;
            break;
        case 42:
            EVENT42;
            break;
        case 43:
            EVENT43;
            break;
        case 44:
            EVENT44;
            break;
        case 45:
            EVENT45;
            break;
        case 46:
            EVENT46;
            break;
        case 47:
            EVENT47;
            break;
        case 48:
            EVENT48;
            break;
        case 49:
            EVENT49;
            break;
        case 50:
            EVENT50;
            break;
        }
    }
    else if (2 == i)
    {
        switch (Tree[i].Grade.now)
        {
        case 0:
            CAR_EVENT0;
            break;
        case 1:
            CAR_EVENT1;
            break;
        case 2:
            CAR_EVENT2;
            break;
        case 3:
            CAR_EVENT3;
            break;
        case 4:
            CAR_EVENT4;
            break;
        case 5:
            CAR_EVENT5;
            break;
        case 6:
            CAR_EVENT6;
            break;
        case 7:
            CAR_EVENT7;
            break;
        case 8:
            CAR_EVENT8;
            break;
        case 9:
            CAR_EVENT9;
            break;
        case 10:
            CAR_EVENT10;
            break;
        case 11:
            CAR_EVENT11;
            break;
        case 12:
            CAR_EVENT12;
            break;
        case 13:
            CAR_EVENT13;
            break;
        case 14:
            CAR_EVENT14;
            break;
        case 15:
            CAR_EVENT15;
            break;
        }
    }
    /*中间变量归零*/
    for (uint8 j = 0; j < 7; j++)
        Tree[j].temp = 0;
}

/*****************************************************
*
*       人机交互界面显示部分      
*       
*****************************************************/
void key_show(uint8 i)
{
    static uint8 last_mode = 0;

    switch (Tree[i].show_mode)
    {
    case 0:
        CLEAR; //顶部显示main page
        SEAT(i);
        SHOW_0();
        break;

    case 1:
        CLEAR;
        SHOW_1_0; //显示电感信息
        break;

    case 2:
        CLEAR;
        SEAT(i);
        SHOW_2; //显示车体信息
        break;

    case 3:
        CLEAR;
        SEAT(i);
        SHOW_3;
        break;

    case 4:
        CLEAR;
        SEAT(i);
        SHOW_4;
        break;

    case 5:
        CLEAR;
        SEAT(i);
        SHOW_5;
        break;

    case 6:
        CLEAR;
        SEAT(i);
        SHOW_6;
        break;
    }
}
/*************************************************
*
*       人机交互执行部分函数封装
*
*************************************************/
void All_show()
{
    uint8 temp = find();
    key_process(temp, key_num);
    key_show(temp);
}
/***************************************************
*
*       开机界面及参数加载
*
***************************************************/
void Boot_action()
{
    uint8 temp0, temp1 = 0, temp2 = 0, temp3 = 0, boot = 0;
    uint16 times = 0;

    gpio_init(PTD11, GPI, 0);
    port_init_NoALT(PTD11, PULLUP); //保持复用不变，仅仅改变配置选项
    gpio_init(PTD12, GPI, 0);
    port_init_NoALT(PTD12, PULLUP); //保持复用不变，仅仅改变配置选项
    lcd_STR_ench(0, 0, YELLOW, BLACK, "是否加载历史参数？");
    lcd_STR_ench(0, 40, YELLOW, BLACK, "若无选择，将在3秒后自动确认");
    lcd_STR_ench(0, 304, YELLOW, BLACK, "确认");
    lcd_STR_ench(208, 304, YELLOW, BLACK, "返回");
    while (1)
    {
        temp0 = gpio_get(PTD11);
        temp1 = (0 == temp0) ? (temp1 + 1) : 0;
        temp0 = gpio_get(PTD12);
        temp2 = (0 == temp0) ? (temp2 + 1) : 0;
        if (temp1 > 10)
        {
            boot = 1;
            break;
        }
        if (temp2 > 10)
        {
            boot = 0;
            break;
        }
        DELAY_MS(5);

        times++;
        if (times > 0 && times <= 200)
            lcd_STR_ench(70, 130, YELLOW, BLACK, "      3！       ");
        else if (times > 200 && times <= 400)
            lcd_STR_ench(70, 130, YELLOW, BLACK, "      2！       ");
        else if (times > 400 && times <= 600)
            lcd_STR_ench(70, 130, YELLOW, BLACK, "      1！       ");
        else if (times > 600) //3秒后自动加载
        {
            boot = 1;
            led(LED0, LED_ON);
            break;
        }
    }

    lcd_clear(BLACK);

    if (1 == boot)
    {
        lcd_STR_ench(50, 50, YELLOW, BLACK, "准备加载参数。。。");
        my_data_load(); //读入flash数据
        DELAY_MS(50);
        lcd_STR_ench(50, 50, YELLOW, BLACK, "参数加载中。。。");
        DELAY_MS(500);
        lcd_STR_ench(50, 50, YELLOW, BLACK, "参数加载成功！   ");
        temp3 = 1;
        DELAY_MS(1500);
    }
    if (0 == temp3)
    {
        lcd_STR_ench(50, 50, YELLOW, BLACK, "即将开始赛道扫描");
        DELAY_MS(1000);
        lcd_STR_ench(70, 130, YELLOW, BLACK, "      3！       ");
        DELAY_MS(1000);
        lcd_STR_ench(70, 130, YELLOW, BLACK, "      2！       ");
        DELAY_MS(1000);
        lcd_STR_ench(70, 130, YELLOW, BLACK, "      1！       ");
        DELAY_MS(1000);
    }
    lcd_clear(BLACK);
}
/***********************************************************
*
*       对即将使用的flash空间进行初始化
*
************************************************************/
void my_flash_init()
{

    data.sectornum_start = FLASH_SECTOR_NUM - 2; //起始扇区      用最后的3个扇区来作为保存参数
    data.sectornum_end = FLASH_SECTOR_NUM - 1;   //结束扇区
    data.data_addr = &md;                        //数据的地址
    data.data_size = sizeof(md);                 //数据的大小

    flash_data_init(&data); //初始化
}
/*****************************************************
*
*       把要保存的数据写入flash
*
*****************************************************/
void my_data_save()
{
    md.Servo.P = Servo_PID->P;
    md.Servo.D = Servo_PID->D;

    md.Motor.P = Motor_PID->P;
    md.Motor.I = Motor_PID->I;
    md.Motor.D = Motor_PID->D;

    md.S_Kp.P = Adapt_S_Kp.P;
    md.S_Kp.I = Adapt_S_Kp.I;
    md.S_Kp.D = Adapt_S_Kp.D;

    md.S_Kd.P = Adapt_S_Kd.P;
    md.S_Kd.I = Adapt_S_Kd.I;
    md.S_Kd.D = Adapt_S_Kd.D;

    md.threshold = Ecc_Car->Round.threshold;
    md.Motor.duty = Ecc_Car->Duty.motor;

    for (uint8 i = 0; i < INDUCT_NUM; i++)
    {
        md.Nor.min[i] = Induct[i].Nor.min;
        md.Nor.range[i] = Induct[i].Nor.range;
    }
    md.password = PASSWORD;
    flash_data_save(&data); //写入数据
}
/****************************************************
*
*       从flash中读取保存的数据
*
****************************************************/
void my_data_load()
{
    flash_data_load(&data); //读出数据
    if (PASSWORD == md.password)
    {
        Servo_PID->P = md.Servo.P;
        Servo_PID->D = md.Servo.D;

        Adapt_S_Kp.P = md.S_Kp.P;
        Adapt_S_Kp.I = md.S_Kp.I;
        Adapt_S_Kp.D = md.S_Kp.D;

        Adapt_S_Kd.P = md.S_Kd.P;
        Adapt_S_Kd.I = md.S_Kd.I;
        Adapt_S_Kd.D = md.S_Kd.D;

        Motor_PID->P = md.Motor.P;
        Motor_PID->I = md.Motor.I;
        Motor_PID->D = md.Motor.D;

        Ecc_Car->Duty.motor = md.Motor.duty;
        Ecc_Car->Round.threshold = md.threshold;
        for (uint8 i = 0; i < INDUCT_NUM; i++)
        {
            Induct[i].Nor.min = md.Nor.min[i];
            Induct[i].Nor.range = md.Nor.range[i];
        }
    }
    Ecc_Car->Scan.finish = 1; //加载历史参数，免去参数重新扫描
}