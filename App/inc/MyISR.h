#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#define Time_Handler            \
    do                          \
    {                           \
        if (Ecc_Car->times > 0) \
            Ecc_Car->times--;   \
    } while (0) //用于特定动作的执行时间控制

#define FLAGS_UPDATE       \
    do                     \
    {                      \
        Show.count++;      \
        Show.PIT_over = 1; \
    } while (0)

#define TFT_SCOPE                                                                                          \
    do                                                                                                     \
    {                                                                                                      \
        TFT_INPUT("差比和:", (SUB0 * 100 / (Induct[2].Normalized_value + Induct[3].Normalized_value)), 0); \
        TFT_INPUT("差比积:", (SUB0 * 5000 / MUL0), 1);                                                     \
        TFT_INPUT("电感2:", Induct[2].Normalized_value, 2);                                                \
        TFT_INPUT("电感3:", Induct[3].Normalized_value, 3);                                                \
        TFT_INPUT("电感4:", Induct[4].Normalized_value, 4);                                                \
    } while (0)

void PIT0_Handler();
#endif

/* //参考帖子：Coertx-M4系列中断嵌套、优先级说明（新增M0+系列说明） - 智能车资料区 - 山外论坛 http://vcan123.com/forum.php?mod=viewthread&tid=959&ctid=1
NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);    //设置优先级分组（分组方式4）,4bit 抢占优先级,没有亚优先级(可设置0~15个级别)

NVIC_SetPriority(PORTA_IRQn,0);         //配置优先级    
VIC_SetPriority(PORTE_IRQn,1);         //配置优先级
优先级分组：分组方式0（所有4个bit为均用来配置响应优先级，所有中断之间不能互相打断）
            分组方式1（1个bit位用来配置抢占优先级，剩余3个bit位用来配置响应优先级）
            分组方式2（2个bit位用来配置抢占优先级，剩余2个bit位用来配置响应优先级）
            分组方式3（3个bit位用来配置抢占优先级，剩余1个bit位用来配置响应优先级）
            分组方式4（4个bit位全用来配置抢占优先级）
抢占优先级：指中断打断其他中断的能力，优先级高的打断优先级低的中断函数
响应优先级（亚优先级）：指两个优先级相同的中断同时到来时，系统对中断的选择优先性 
*/
