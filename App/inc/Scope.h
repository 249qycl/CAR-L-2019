#ifndef __SCOPE_H__
#define __SCOPE_H__

#define SCOPE_VAR_NUM   5
#define SCOPE_H         121
#define SCOPE_W         238
#define MID_LINE        61

typedef struct
{
    uint8       key :1;
    uint8       stop:1;
    uint8       re_window:1;
    uint8       auto_switch:1;
    uint8       step;
    uint8       cursor_seat;
    char*       var_name[SCOPE_VAR_NUM];
    uint8       gear[SCOPE_VAR_NUM];
    uint8       img[SCOPE_W][SCOPE_VAR_NUM];
    uint8       rear[SCOPE_VAR_NUM];   
    uint8       state[SCOPE_VAR_NUM];    
    uint16      divide[SCOPE_VAR_NUM];
    uint16      color[SCOPE_VAR_NUM];   
    int         cursor_data[SCOPE_W][SCOPE_VAR_NUM];
    int         data[SCOPE_VAR_NUM];
}SCOPE;
extern SCOPE Scope;

#define TFT_INPUT(name,var,num)     do{Scope.var_name[num]=name;Scope.data[num]=var;}while(0)

void Scope_init();
void Insert_to_queue();
void Cursor_move(uint8 seat);
void Scope_data_show();

void Scope_show();
#endif