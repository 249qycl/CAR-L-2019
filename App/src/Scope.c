#include "common.h"
#include "include.h"

SCOPE Scope;

void Scope_init()
{
    static uint8 re_init=0;
    if(re_init==0)
    {
        Scope=(SCOPE){.gear={5,5,5,5,5},
                      .key=1,
                      .step=1,
                      .stop=1,
                      .auto_switch=1,
                      .cursor_seat=50,
                      .var_name={"数据","数据","数据","数据","数据"},
                      .state={1},
                      .divide={0},//最大65535
                      .color ={RED,GREEN,BRED,YELLOW,GBLUE}};
        re_init=1;
    } 
    lcd_clear(BACK_C);
    lcd_rectangle((Site_t){0,0},(Size_t){SCOPE_W+2,SCOPE_H+2},WHITE);
    lcd_rectangle((Site_t){1,1},(Size_t){SCOPE_W,SCOPE_H},BLACK);
    for(uint8 i=0;i<(SCOPE_W+2);i++)
    {
        if(i%4==0||i%4==1)
          lcd_point((Site_t){i,MID_LINE},WHITE);//0线
    }
}
void Insert_to_queue()
{
    uint8 temp;
    for(uint8 i=0;i<SCOPE_VAR_NUM;i++)
    {
        if(Scope.state[i]==0)
            Scope.data[i]=0;
        temp=Scope.img[Scope.rear[i]][i];
        if(temp!=61&&temp!=0)
            lcd_point((Site_t){Scope.rear[i]+1,temp},BLACK);
        else if(temp!=0)
        {
            if((Scope.rear[i]+1)%4==0||(Scope.rear[i]+1)%4==1)
                lcd_point((Site_t){Scope.rear[i]+1,61},WHITE);
        }
        if(Scope.auto_switch==1)
            Scope.divide[i]=abs(Scope.data[i])>Scope.divide[i]?abs(Scope.data[i]):Scope.divide[i];
            
        Scope.cursor_data[Scope.rear[i]][i]=Scope.data[i];
        Scope.data[i]=RANGE(Scope.data[i],Scope.divide[i],-Scope.divide[i]);
        temp=61 - Scope.data[i]*60/Scope.divide[i];
        Scope.img[Scope.rear[i]][i]=temp;
        lcd_point((Site_t){Scope.rear[i]+1,temp},Scope.color[i]);
        Scope.rear[i]=(Scope.rear[i]+1)%SCOPE_W;
    }
}

void Cursor_move(uint8 seat)
{
    static uint8 last_seat=1;
    uint8 temp;
    
    seat=RANGE(seat,SCOPE_W,1);
    for(uint8 i=1;i<SCOPE_H;i++)
    {    
        lcd_point((Site_t){last_seat,i},BLACK);
        lcd_point((Site_t){seat,i},WHITE);
    }
    for(uint8 i=0;i<SCOPE_VAR_NUM;i++)
    {
        temp=Scope.img[last_seat][i];
        lcd_point((Site_t){last_seat,temp},Scope.color[i]);    
    }
    last_seat=seat;
}

void Scope_data_show()
{
    for(uint8 i=0;i<SCOPE_VAR_NUM;i++)
    {
        lcd_STR_ench(0  ,200+i*20,Scope.color[i],BLACK,"数据:%d",Scope.cursor_data[Scope.cursor_seat][i]);
        lcd_STR_ench(80 ,200+i*20,Scope.color[i],BLACK,"DIV:%d",Scope.divide[i]);
        lcd_STR_ench(160,200+i*20,Scope.color[i],BLACK,"状态:%c","OI"[Scope.state[i]]);
    }  
}

void Scope_show()
{
    if(Scope.re_window==0)
    {
        Scope_init();
        Scope.re_window=1;
    } 
    if(Scope.stop==1)
        Insert_to_queue();
    Cursor_move(Scope.cursor_seat);
//    Scope_data_show();
}