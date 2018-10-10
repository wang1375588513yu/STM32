/********************************Copyright (c)******************************\
**
**                    (c) Copyright 2018, WangYu, China, SZ.
**                            All Rights Reserved
**
**                               By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      timer.c
** The Author:    WangYu
** Creation Date: 2018-08-27
** Description:
**
**--------------------------------Version Information-----------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**--------------------------------------------------------------------------
\********************************End of Head********************************/
#include "timer.h"
#include "sys.h"
#include "led.h"
#include "PUBLIC.H"

unsigned int sec_tick = 0;
unsigned int runsec_tick = 0;
unsigned int millisec_tick = 0;
unsigned int wifi_delay_millisec=0;

TIM_HandleTypeDef TIM2_Handler;
TIM_HandleTypeDef TIM3_Handler;

void TIM3_Init(u16 arr,u16 psc)
{
    TIM3_Handler.Instance = TIM3;
    TIM3_Handler.Init.Period = arr;							//自动重装载值
    TIM3_Handler.Init.Prescaler = psc;						//分频系数
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		//向上计数模式
    HAL_TIM_Base_Init(&TIM3_Handler);

    HAL_TIM_Base_Start_IT(&TIM3_Handler);					//中断启动
}

void TIM2_Init(u16 arr,u16 psc)
{
    TIM2_Handler.Instance = TIM2;
    TIM2_Handler.Init.Period = arr;							//自动重装载值
    TIM2_Handler.Init.Prescaler = psc;						//分频系数
    TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;		//向上计数模式
    HAL_TIM_Base_Init(&TIM2_Handler);

    HAL_TIM_Base_Start_IT(&TIM2_Handler);					//中断启动
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim)
{
    if(htim->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();		//打开定时器时钟
        HAL_NVIC_SetPriority(TIM3_IRQn,0,3);//设置主优先级为0，从优先级为3
        HAL_NVIC_EnableIRQ(TIM3_IRQn);		//Enable a device specific interrupt in the NVIC interrupt controller.
    }
    if(htim->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();		//打开定时器时钟
        HAL_NVIC_SetPriority(TIM2_IRQn,0,3);//设置主优先级为0，从优先级为3
        HAL_NVIC_EnableIRQ(TIM2_IRQn);		//Enable a device specific interrupt in the NVIC interrupt controller.
    }
}

void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);	//This function handles TIM interrupts requests.
  //  Led_Status_Reversal();
    sec_tick++;
    runsec_tick++;
}

void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM2_Handler);	//This function handles TIM interrupts requests.
    millisec_tick++;
}

unsigned int Get_SysTick(void)
{
    return sec_tick;
}

unsigned int Get_SysmilliTick(void)
{
    return millisec_tick;
}

unsigned int Get_SysRunTick(void)
{
    return runsec_tick;
}

void Set_MillisecTimer_Wifi_delay(unsigned int delay)
{
    wifi_delay_millisec=delay;
}
unsigned int Get_MillisecTimer_Wifi_delay(void)
{
    return wifi_delay_millisec;
}


