/********************************Copyright (c)******************************\
**
**                    (c) Copyright 2018, WangYu, China, SZ.
**                            All Rights Reserved
**
**                               By(MR.Richard)
**
**----------------------------------File Information------------------------
** FileName:      led.c
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
/*PA5  --   LED*/

#include "led.h"
#include "sys.h"
#include "user_config.h"
#include "timer.h"
#include "PUBLIC.h"
#include "usart.h"

bool led_on_flag = false;


#ifdef	LED_ENABLE
void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    LED_RCC_GPIO_CLK_ENABLE();					//初始化时钟

    GPIO_Initure.Pin = LED_PIN;					//LED引脚
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;	//高速率
    GPIO_Initure.Pull = GPIO_PULLUP;			//上拉
    HAL_GPIO_Init(LED_GPIO, &GPIO_Initure);

    HAL_GPIO_WritePin(LED_GPIO, LED_PIN, LED_OFF);//
}

void Led_On(void)
{
    led_on_flag = true;
    HAL_GPIO_WritePin(LED_GPIO, LED_PIN, LED_ON);
}

void Led_Off(void)
{
    led_on_flag = false;
    HAL_GPIO_WritePin(LED_GPIO, LED_PIN, LED_OFF);//初始为灭灯
}

void Led_Status_Reversal(void)
{
    if(!led_on_flag)
        Led_On();
    else
        Led_Off();
}

static void led_loop(void)
{
    static unsigned int softimersec=0xffffffff;
    if(softimersec==Get_SysTick())return;
    softimersec=Get_SysTick();

    Led_Status_Reversal();
}

void led_task(void)
{
    led_loop();
}



#endif

