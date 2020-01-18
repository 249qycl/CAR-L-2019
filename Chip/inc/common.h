/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       common.h
 * @brief      山外K66 平台常用类型声明和宏定义
 * @author     山外科技
 * @version    v6.0
 * @date       2014-04-25
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*
 * 数据类型声明
 */
typedef unsigned char       uint8;  /*  8 bits */
typedef unsigned short int  uint16; /* 16 bits */
typedef unsigned long int   uint32; /* 32 bits */
typedef unsigned long long  uint64; /* 64 bits */

typedef char                int8;   /*  8 bits */
typedef short int           int16;  /* 16 bits */
typedef long  int           int32;  /* 32 bits */
typedef long  long          int64;  /* 64 bits */

typedef volatile int8       vint8;  /*  8 bits */
typedef volatile int16      vint16; /* 16 bits */
typedef volatile int32      vint32; /* 32 bits */
typedef volatile int64      vint64; /* 64 bits */

typedef volatile uint8      vuint8;  /*  8 bits */
typedef volatile uint16     vuint16; /* 16 bits */
typedef volatile uint32     vuint32; /* 32 bits */
typedef volatile uint64     vuint64; /* 64 bits */

/*
 * 定义带位域的联合体类型
 */
typedef union
{
    uint32  DW;  //双字
    uint16  W[2];//字
    uint8   B[4];//字节
    struct
    {
        uint32 b0: 1;
        uint32 b1: 1;
        uint32 b2: 1;
        uint32 b3: 1;
        uint32 b4: 1;
        uint32 b5: 1;
        uint32 b6: 1;
        uint32 b7: 1;
        uint32 b8: 1;
        uint32 b9: 1;
        uint32 b10: 1;
        uint32 b11: 1;
        uint32 b12: 1;
        uint32 b13: 1;
        uint32 b14: 1;
        uint32 b15: 1;
        uint32 b16: 1;
        uint32 b17: 1;
        uint32 b18: 1;
        uint32 b19: 1;
        uint32 b20: 1;
        uint32 b21: 1;
        uint32 b22: 1;
        uint32 b23: 1;
        uint32 b24: 1;
        uint32 b25: 1;
        uint32 b26: 1;
        uint32 b27: 1;
        uint32 b28: 1;
        uint32 b29: 1;
        uint32 b30: 1;
        uint32 b31: 1;
    };
} Dtype;    //sizeof(Dtype) 为 4

/*
 * 定义坐标结构体
 */
typedef struct
{
    uint16 x;
    uint16 y;
} Site_t;

/*
 * 定义矩形大小结构体
 */
typedef struct
{
    uint16 W;       //宽
    uint16 H;       //高
} Size_t;

#define TRUE    1
#define FALSE   0


/*
 * 中断号类型声明
 */
typedef enum
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_IRQn             = -14,      /*!< 2 Non Maskable Interrupt                                                                                    */
    HardFault_IRQn                  = -13 ,     /*!< 3 Hard Fault                                                                                */
    MemoryManagement_IRQn           = -12,      /*!< 4 Cortex-M4 Memory Management Interrupt                                                     */
    BusFault_IRQn                   = -11,      /*!< 5 Cortex-M4 Bus Fault Interrupt                                                                     */
    UsageFault_IRQn                 = -10,      /*!< 6 Cortex-M4 Usage Fault Interrupt                                                               */
    SVCall_IRQn                     = -5,       /*!< 11 Cortex-M4 SV Call Interrupt                                                                      */
    DebugMonitor_IRQn               = -4,       /*!< 12 Cortex-M4 Debug Monitor Interrupt                                                            */
    PendSV_IRQn                     = -2,       /*!< 14 Cortex-M4 Pend SV Interrupt                                                                      */
    SysTick_IRQn                    = -1,       /*!< 15 Cortex-M4 System Tick Interrupt                                                              */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    DMA0_DMA16_IRQn                       = 0,        // DMA Channel 0/16 Transfer Complete
    DMA1_DMA17_IRQn                       = 1,        // DMA Channel 1/17 Transfer Complete
    DMA2_DMA18_IRQn                       = 2,        // DMA Channel 2/18 Transfer Complete
    DMA3_DMA19_IRQn                       = 3,        // DMA Channel 3/19 Transfer Complete
    DMA4_DMA20_IRQn                       = 4,        // DMA Channel 4/20 Transfer Complete
    DMA5_DMA21_IRQn                       = 5,        // DMA Channel 5/21 Transfer Complete
    DMA6_DMA22_IRQn                       = 6,        // DMA Channel 6/22 Transfer Complete
    DMA7_DMA23_IRQn                       = 7,        // DMA Channel 7/23 Transfer Complete
    DMA8_DMA24_IRQn                       = 8,        // DMA Channel 8/24 Transfer Complete
    DMA9_DMA25_IRQn                       = 9,        // DMA Channel 9/25 Transfer Complete
    DMA10_DMA26_IRQn                      = 10,       // DMA Channel 10/26 Transfer Complete
    DMA11_DMA27_IRQn                      = 11,       // DMA Channel 11/27 Transfer Complete
    DMA12_DMA28_IRQn                      = 12,       // DMA Channel 12/28 Transfer Complete
    DMA13_DMA29_IRQn                      = 13,       // DMA Channel 13/29 Transfer Complete
    DMA14_DMA30_IRQn                      = 14,       // DMA Channel 14/30 Transfer Complete
    DMA15_DMA31_IRQn                      = 15,       // DMA Channel 15/31 Transfer Complete
    DMA_Error_IRQn                  = 16,       // DMA Error Interrupt
    MCM_IRQn                        = 17,       // Normal Interrupt
    FTFE_IRQn                       = 18,       // FTFE Interrupt
    Read_Collision_IRQn             = 19,       // Read Collision Interrupt
    LVD_LVW_IRQn                    = 20,       // Low Voltage Detect, Low Voltage Warning
    LLW_IRQn                        = 21,       // Low Leakage Wakeup
    Watchdog_IRQn                   = 22,       // WDOG Interrupt
    RNG_IRQn                        = 23,       // RNGB Interrupt
    I2C0_IRQn                       = 24,       // I2C0 interrupt
    I2C1_IRQn                       = 25,       // I2C1 interrupt
    SPI0_IRQn                       = 26,       // SPI0 Interrupt
    SPI1_IRQn                       = 27,       // SPI1 Interrupt
    I2S0_Tx_IRQn                    = 28,       // I2S0 transmit interrupt
    I2S0_Rx_IRQn                    = 29,       // I2S0 receive interrupt
    Reserved46_IRQn                 = 30,        // 保留
    UART0_RX_TX_IRQn                = 31,       // UART0 Receive/Transmit interrupt
    UART0_ERR_IRQn                  = 32,       // UART0 Error interrupt
    UART1_RX_TX_IRQn                = 33,       // UART1 Receive/Transmit interrupt
    UART1_ERR_IRQn                  = 34,       // UART1 Error interrupt
    UART2_RX_TX_IRQn                = 35,       // UART2 Receive/Transmit interrupt
    UART2_ERR_IRQn                  = 36,       // UART2 Error interrupt
    UART3_RX_TX_IRQn                = 37,       // UART3 Receive/Transmit interrupt
    UART3_ERR_IRQn                  = 38,        // UART3 Error interrupt
    ADC0_IRQn                       = 39,       // ADC0 interrupt
    CMP0_IRQn                       = 40,       // CMP0 interrupt
    CMP1_IRQn                       = 41,       // CMP1 interrupt
    FTM0_IRQn                       = 42,       // FTM0 fault, overflow and channels interrupt
    FTM1_IRQn                       = 43,       // FTM1 fault, overflow and channels interrupt
    FTM2_IRQn                       = 44,       // FTM2 fault, overflow and channels interrupt
    CMT_IRQn                        = 45,       // CMT interrupt
    RTC_IRQn                        = 46,       // RTC interrupt
    RTC_Seconds_IRQn                = 47,       // RTC seconds interrupt
    PIT0_IRQn                       = 48,       // PIT timer channel 0 interrupt
    PIT1_IRQn                       = 49,       // PIT timer channel 1 interrupt
    PIT2_IRQn                       = 50,       // PIT timer channel 2 interrupt
    PIT3_IRQn                       = 51,       // PIT timer channel 3 interrupt
    PDB0_IRQn                       = 52,       // PDB0 Interrupt
    USB0_IRQn                       = 53,       // USB0 interrupt
    USBDCD_IRQn                     = 54,       // USBDCD Interrupt
	Reserved71_IRQn                 = 55,       // Reserved interrupt 71
    DAC0_IRQn                       = 56,       // DAC0 interrupt
    MCG_IRQn                        = 57,       // MCG Interrupt
    LPTMR_IRQn                      = 58,       // LPTimer interrupt
    PORTA_IRQn                      = 59,       // Port A interrupt
    PORTB_IRQn                      = 60,       // Port B interrupt
    PORTC_IRQn                      = 61,       // Port C interrupt
    PORTD_IRQn                      = 62,       // Port D interrupt
    PORTE_IRQn                      = 63,       // Port E interrupt
	SWI_IRQn                        = 64,       // Software interrupt
    SPI2_IRQn                       = 65,       // SPI2 Interrupt
    UART4_RX_TX_IRQn                = 66,       // UART4 Receive/Transmit interrupt
    UART4_ERR_IRQn                  = 67,        // UART4 Error interrupt
    Reserved84_IRQn                 = 68,       // Reserved interrupt 84
    Reserved85_IRQn                 = 69,       // Reserved interrupt 85
    CMP2_IRQn                       = 70,       // CMP2 interrupt
    FTM3_IRQn                       = 71,       // FTM3 fault, overflow and channels interrupt
    DAC1_IRQn                       = 72,       // DAC1 interrupt
    ADC1_IRQn                       = 73,       // ADC1 interrupt
    I2C2_IRQn                       = 74,       // I2C2 interrupt
    CAN0_ORed_MB_IRQn               = 75,       // CAN0 OR'd Message Buffers Interrupt
    CAN0_Bus_Off_IRQn               = 76,       // CAN0 Bus Off Interrupt
    CAN0_Error_IRQn                 = 77,       // CAN0 Error Interrupt
    CAN0_Tx_Warning_IRQn            = 78,       // CAN0 Tx Warning Interrupt
    CAN0_Rx_Warning_IRQn            = 79,       // CAN0 Rx Warning Interrupt
    CAN0_Wake_Up_IRQn               = 80,       // CAN0 Wake Up Interrupt
    SDHC_IRQn                       = 81,       // SDHC Interrupt
    ENET_1588_Timer_IRQn            = 82,       // Ethernet MAC IEEE 1588 Timer Interrupt
    ENET_Transmit_IRQn              = 83,       // Ethernet MAC Transmit Interrupt
    ENET_Receive_IRQn               = 84,       // Ethernet MAC Receive Interrupt
    ENET_Error_IRQn                 = 85,       // Ethernet MAC Error and miscelaneous Interrupt
	LPUART0_IRQn                    = 86,       // LPUART0 status/error interrupt
    TSI0_IRQn                       = 87,       // TSI0 Interrupt
    TPM1_IRQn                       = 88,       // TPM1 fault, overflow and channels interrupt
    TPM2_IRQn                       = 89,       // TPM2 fault, overflow and channels interrupt
    USBHSDCD_IRQn                   = 90,       // USBHSDCD, USBHS Phy Interrupt
    I2C3_IRQn                       = 91,       // I2C3 interrupt
    CMP3_IRQn                       = 92,       // CMP3 interrupt
    USBHS_IRQn                      = 93,       // USB high speed OTG interrupt
    CAN1_ORed_MB_IRQn               = 94,       // CAN1 OR'd Message Buffers Interrupt
    CAN1_Bus_Off_IRQn               = 95,       // CAN1 Bus Off Interrupt
    CAN1_Error_IRQn                 = 96,       // CAN1 Error Interrupt
    CAN1_Tx_Warning_IRQn            = 97,       // CAN1 Tx Warning Interrupt
    CAN1_Rx_Warning_IRQn            = 98,       // CAN1 Rx Warning Interrupt
    CAN1_Wake_Up_IRQn               = 99,       // CAN1 Wake Up Interrupt
} IRQn_t, IRQn_Type;


/*
 * 中断向量表编号声明
 */
typedef enum
{
    /******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
    NonMaskableInt_VECTORn          = 2 ,       /*!< 2 Non Maskable Interrupt                                                                                    */
    HardFault_VECTORn               = 3 ,       /*!< 3 Hard Fault                                                                                */
    MemoryManagement_VECTORn        = 4 ,       /*!< 4 Cortex-M4 Memory Management Interrupt                                                     */
    BusFault_VECTORn                = 5 ,       /*!< 5 Cortex-M4 Bus Fault Interrupt                                                                     */
    UsageFault_VECTORn              = 6 ,       /*!< 6 Cortex-M4 Usage Fault Interrupt
    */
    SVCall_VECTORn                  = 11,       /*!< 11 Cortex-M4 SV Call Interrupt                                                                      */
    DebugMonitor_VECTORn            = 12,       /*!< 12 Cortex-M4 Debug Monitor Interrupt
    */
    PendSV_VECTORn                  = 14,       /*!< 14 Cortex-M4 Pend SV Interrupt                                                                      */
    SysTick_VECTORn                 = 15,       /*!< 15 Cortex-M4 System Tick Interrupt                                                              */
    /******  Kinetis 60 specific Interrupt Numbers **********************************************************************/
    DMA0_DMA16_VECTORn                       = 16,        // DMA Channel 0/16 Transfer Complete
    DMA1_DMA17_VECTORn                       = 17,        // DMA Channel 1/17 Transfer Complete
    DMA2_DMA18_VECTORn                       = 18,        // DMA Channel 2/18 Transfer Complete
    DMA3_DMA19_VECTORn                       = 19,        // DMA Channel 3/19 Transfer Complete
    DMA4_DMA20_VECTORn                       = 20,        // DMA Channel 4/20 Transfer Complete
    DMA5_DMA21_VECTORn                       = 21,        // DMA Channel 5/21 Transfer Complete
    DMA6_DMA22_VECTORn                       = 22,        // DMA Channel 6/22 Transfer Complete
    DMA7_DMA23_VECTORn                       = 23,        // DMA Channel 7/23 Transfer Complete
    DMA8_DMA24_VECTORn                       = 24,        // DMA Channel 8/24 Transfer Complete
    DMA9_DMA25_VECTORn                       = 25,        // DMA Channel 9/25 Transfer Complete
    DMA10_DMA26_VECTORn                      = 26,       // DMA Channel 10/26 Transfer Complete
    DMA11_DMA27_VECTORn                      = 27,       // DMA Channel 11/27 Transfer Complete
    DMA12_DMA28_VECTORn                      = 28,       // DMA Channel 12/28 Transfer Complete
    DMA13_DMA29_VECTORn                      = 29,       // DMA Channel 13/29 Transfer Complete
    DMA14_DMA30_VECTORn                      = 30,       // DMA Channel 14/30 Transfer Complete
    DMA15_DMA31_VECTORn                      = 31,       // DMA Channel 15/31 Transfer Complete
    DMA_Error_VECTORn                  = 32,       // DMA Error Interrupt
    MCM_VECTORn                        = 33,       // Normal Interrupt
    FTFE_VECTORn                       = 34,       // FTFE Interrupt
    Read_Collision_VECTORn             = 35,       // Read Collision Interrupt
    LVD_LVW_VECTORn                    = 36,       // Low Voltage Detect, Low Voltage Warning
    LLW_VECTORn                        = 37,       // Low Leakage Wakeup
    Watchdog_VECTORn                   = 38,       // WDOG Interrupt
    RNG_VECTORn                        = 39,       // RNGB Interrupt
    I2C0_VECTORn                       = 40,       // I2C0 interrupt
    I2C1_VECTORn                       = 41,       // I2C1 interrupt
    SPI0_VECTORn                       = 42,       // SPI0 Interrupt
    SPI1_VECTORn                       = 43,       // SPI1 Interrupt
    I2S0_Tx_VECTORn                    = 44,       // I2S0 transmit interrupt
    I2S0_Rx_VECTORn                    = 45,       // I2S0 receive interrupt
    Reserved46_VECTORn                 = 46,        // 保留
    UART0_RX_TX_VECTORn                = 47,       // UART0 Receive/Transmit interrupt
    UART0_ERR_VECTORn                  = 48,       // UART0 Error interrupt
    UART1_RX_TX_VECTORn                = 49,       // UART1 Receive/Transmit interrupt
    UART1_ERR_VECTORn                  = 50,       // UART1 Error interrupt
    UART2_RX_TX_VECTORn                = 51,       // UART2 Receive/Transmit interrupt
    UART2_ERR_VECTORn                  = 52,       // UART2 Error interrupt
    UART3_RX_TX_VECTORn                = 53,       // UART3 Receive/Transmit interrupt
    UART3_ERR_VECTORn                  = 54,        // UART3 Error interrupt
    ADC0_VECTORn                       = 55,       // ADC0 interrupt
    CMP0_VECTORn                       = 56,       // CMP0 interrupt
    CMP1_VECTORn                       = 57,       // CMP1 interrupt
    FTM0_VECTORn                       = 58,       // FTM0 fault, overflow and channels interrupt
    FTM1_VECTORn                       = 59,       // FTM1 fault, overflow and channels interrupt
    FTM2_VECTORn                       = 60,       // FTM2 fault, overflow and channels interrupt
    CMT_VECTORn                        = 61,       // CMT interrupt
    RTC_VECTORn                        = 62,       // RTC interrupt
    RTC_Seconds_VECTORn                = 63,       // RTC seconds interrupt
    PIT0_VECTORn                       = 64,       // PIT timer channel 0 interrupt
    PIT1_VECTORn                       = 65,       // PIT timer channel 1 interrupt
    PIT2_VECTORn                       = 66,       // PIT timer channel 2 interrupt
    PIT3_VECTORn                       = 67,       // PIT timer channel 3 interrupt
    PDB0_VECTORn                       = 68,       // PDB0 Interrupt
    USB0_VECTORn                       = 69,       // USB0 interrupt
    USBDCD_VECTORn                     = 70,       // USBDCD Interrupt
	Reserved71_VECTORn                 = 71,       // Reserved interrupt 71
    DAC0_VECTORn                       = 72,       // DAC0 interrupt
    MCG_VECTORn                        = 73,       // MCG Interrupt
    LPTMR_VECTORn                      = 74,       // LPTimer interrupt
    PORTA_VECTORn                      = 75,       // Port A interrupt
    PORTB_VECTORn                      = 76,       // Port B interrupt
    PORTC_VECTORn                      = 77,       // Port C interrupt
    PORTD_VECTORn                      = 78,       // Port D interrupt
    PORTE_VECTORn                      = 79,       // Port E interrupt
	SWI_VECTORn                        = 80,       // Software interrupt
    SPI2_VECTORn                       = 81,       // SPI2 Interrupt
    UART4_RX_TX_VECTORn                = 82,       // UART4 Receive/Transmit interrupt
    UART4_ERR_VECTORn                  = 83,        // UART4 Error interrupt
    Reserved84_VECTORn                 = 84,       // Reserved interrupt 84
    Reserved85_VECTORn                 = 85,       // Reserved interrupt 85
    CMP2_VECTORn                       = 86,       // CMP2 interrupt
    FTM3_VECTORn                       = 87,       // FTM3 fault, overflow and channels interrupt
    DAC1_VECTORn                       = 88,       // DAC1 interrupt
    ADC1_VECTORn                       = 89,       // ADC1 interrupt
    I2C2_VECTORn                       = 90,       // I2C2 interrupt
    CAN0_ORed_MB_VECTORn               = 91,       // CAN0 OR'd Message Buffers Interrupt
    CAN0_Bus_Off_VECTORn               = 92,       // CAN0 Bus Off Interrupt
    CAN0_Error_VECTORn                 = 93,       // CAN0 Error Interrupt
    CAN0_Tx_Warning_VECTORn            = 94,       // CAN0 Tx Warning Interrupt
    CAN0_Rx_Warning_VECTORn            = 95,       // CAN0 Rx Warning Interrupt
    CAN0_Wake_Up_VECTORn               = 96,       // CAN0 Wake Up Interrupt
    SDHC_VECTORn                       = 97,       // SDHC Interrupt
    ENET_1588_Timer_VECTORn            = 98,       // Ethernet MAC IEEE 1588 Timer Interrupt
    ENET_Transmit_VECTORn              = 99,       // Ethernet MAC Transmit Interrupt
    ENET_Receive_VECTORn               = 100,       // Ethernet MAC Receive Interrupt
    ENET_Error_VECTORn                 = 101,       // Ethernet MAC Error and miscelaneous Interrupt
	LPUART0_VECTORn                    = 102,       // LPUART0 status/error interrupt
    TSI0_VECTORn                       = 103,       // TSI0 Interrupt
    TPM1_VECTORn                       = 104,       // TPM1 fault, overflow and channels interrupt
    TPM2_VECTORn                       = 105,       // TPM2 fault, overflow and channels interrupt
    USBHSDCD_VECTORn                   = 106,       // USBHSDCD, USBHS Phy Interrupt
    I2C3_VECTORn                       = 107,       // I2C3 interrupt
    CMP3_VECTORn                       = 108,       // CMP3 interrupt
    USBHS_VECTORn                      = 109,       // USB high speed OTG interrupt
    CAN1_ORed_MB_VECTORn               = 110,       // CAN1 OR'd Message Buffers Interrupt
    CAN1_Bus_Off_VECTORn               = 111,       // CAN1 Bus Off Interrupt
    CAN1_Error_VECTORn                 = 112,       // CAN1 Error Interrupt
    CAN1_Tx_Warning_VECTORn            = 113,       // CAN1 Tx Warning Interrupt
    CAN1_Rx_Warning_VECTORn            = 114,       // CAN1 Rx Warning Interrupt
    CAN1_Wake_Up_VECTORn               = 115,       // CAN1 Wake Up Interrupt
} VECTORn_t;

/*
 * 定义运行到RAM的函数
 */
#if defined(__ICCARM__)
//IAR 用 __ramfunc 来声明
#define     __RAMFUNC __ramfunc
#else
#define     __RAMFUNC
#endif


/*
 * 包含Cortex-M内核的通用头文件
 */
#include    <stdio.h>                       //printf
#include    <string.h>                      //memcpy
#include    <stdlib.h>                      //malloc
#include "misc.h"        //这两个顺序不能反，因为 arm_math.h 里需要  __NVIC_PRIO_BITS 的定义

#define __MPU_PRESENT                  0         /**< Defines if an MPU is present or not */
#define __NVIC_PRIO_BITS               4         /**< Number of priority bits implemented in the NVIC */
#define __Vendor_SysTickConfig         0         /**< Vendor specific implementation of SysTickConfig is defined */
#define __FPU_PRESENT                  1         /**< Defines if an FPU is present or not */

#include "core_cm4.h"                  /* Core Peripheral Access Layer */
#include "arm_math.h"

/* 优先级分组  -------------------------------------------------*/
#define NVIC_PriorityGroup_0          ((uint32)0x7) /* 0位为抢占优先级，4位为响应优先级 */

#define NVIC_PriorityGroup_1          ((uint32)0x6) /* 1位为抢占优先级，3位为响应优先级 */
                                                     
#define NVIC_PriorityGroup_2          ((uint32)0x5) /* 2位为抢占优先级，2位为响应优先级 */
                                                    
#define NVIC_PriorityGroup_3          ((uint32)0x4) /* 3位为抢占优先级，1位为响应优先级 */
                                                      
#define NVIC_PriorityGroup_4          ((uint32)0x3) /* 4位为抢占优先级，0位为响应优先级 */
                                                      

/*
 * 包含山外K66平台的配置头文件
 */
#include "MK66_conf.h"

/*
 * 包含平台头文件
 */
#if   defined(MK66F18)

#include "MK66F18.h"
#else
#error "error"
#endif



/*
 * 包含常用头文件
 */
#include    "system_MK66FX18.h"             //系统配置
#include    "PORT_cfg.h"                    //管脚复用配置


#if (defined(IAR))
#include "intrinsics.h"
#endif




/**
 *  @brief 变量的位清0和置1
 */
#define BIT_CLEAN(var,n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var,n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var,n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）

/**
 *  @brief 仿二进制赋值
 */
#define  HEX__(n)   0x##n##UL
#define  B8__(x)   ( (x & 0x0000000FUL) ? 1:0 )\
    +( (x & 0x000000F0UL) ? 2:0 )\
    +( (x & 0x00000F00UL) ? 4:0 )\
    +( (x & 0x0000F000UL) ? 8:0 )\
    +( (x & 0x000F0000UL) ? 16:0 )\
    +( (x & 0x00F00000UL) ? 32:0 )\
    +( (x & 0x0F000000UL) ? 64:0 )\
    +( (x & 0xF0000000UL) ? 128:0 )
#define  B8(x)                                     ((unsigned char)B8__(HEX__(x)))
#define  B16(x_msb,x_lsb)                (((unsigned int)B8(x_msb)<<8) + B8(x_lsb))
#define  B32(x_msb,x_2,x_3,x_lsb)   (((unsigned long)B8(x_msb)<<24) + ((unsigned long)B8(x_2)<<16) + ((unsigned long)B8(x_3)<<8) + B8(x_lsb))
/* Sample usage（二进制赋值示例）:
B8(01010101) = 85
B16(10101010,01010101) = 43605
B32(10000000,11111111,10101010,01010101) = 2164238933
*/

/*
 * 打印宏定义展开结果
 */
#define _MKSTR(str)  #str
#define MKSTR(str)  _MKSTR(str)                       //把宏定义展开结果转换为字符串

#define DEFINE_PRINTF(def)  printf(MKSTR(def)"\n")    //串口打印宏定义展开结果


/*
 * 求最大值和最小值
 */
#define MAX( x, y ) ( ((x) > (y)) ? (x) : (y) )
#define MIN( x, y ) ( ((x) < (y)) ? (x) : (y) )


/*
 * 返回数组元素的个数
 */
#define ARR_SIZE( a ) ( sizeof( (a) ) / sizeof( ((a)[0]) ) )

/*
 * 宏定义实现返回绝对值（x里不能有自加自减的语句，否则变量出错）
 */
#define ABS(x) (((x) > 0) ? (x) : (-(x)))

/*
 * 获取结构体某成员偏移
 */
#define OFFSET(type, member)    (uint32)(&(((type *)0)->member))

/*
 * 确保x的范围为 min~max
 */
#define RANGE(x,max,min)        (((x)<(min) ? (min) : ( (x)>(max) ? (max):(x) )))

/*
 * 交换32位数据的4字节顺序
 */
#if 1         //两种方法，第一种是CMSIS库自带
#define SWAP32(data)    __REV(data)
#else
#define SWAP32(data)    (uint32)((((uint32)(data) & (0xFFu<<0 ))<<24)      \
                          | (((uint32)(data) & (0xFFu<<8 ))<<8)            \
                          | (((uint32)(data) & (0xFFu<<16))>>8)            \
                          | (((uint32)(data) & (0xFFu<<24))>>24)           \
                        )
#endif

/*
 * 交换16位数据的2字节顺序
 */
#if 1         //两种方法，第一种是CMSIS库自带
#define SWAP16(data)    __REVSH(data)
#else
#define SWAP16(data)    (uint32)((((uint16)(data) & (0xFF<<0 ))<<8)      \
                          | (((uint32)(data) & (0xFF<<8))>>8)            \
                        )
#endif

/*
 * 交换 x, y 的值
 */
#define SWAP(x,y)           do{x^=y;y^=x;x^=y;}while(0)


/********************************************************************/

#endif /* _COMMON_H_ */
