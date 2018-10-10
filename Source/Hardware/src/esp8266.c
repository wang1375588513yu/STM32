/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                           By(MR.Richard)
**
**----------------------------------File Information----------------------------
** FileName:      esp8266.c
** The Author:    WangYu
** Creation Date: 2018-09-30
** Description:   
**
**--------------------------------Version Information---------------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "esp8266.h"
#include "timer.h"
#include "PUBLIC.H"

bool esp8266_reset_status = false;
bool esp8266_run_mode_status = true;

void esp8266_hardware_init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_UART1_ESP8266_CLK_ENABLE();		//初始化时钟

    GPIO_Initure.Pin = ESP8266_RST_PIN | ESP8266_FIRMWARE_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;	//推挽输出
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;	//高速率
    GPIO_Initure.Pull = GPIO_PULLUP;			//上拉
    HAL_GPIO_Init(ESP8266_GPIO, &GPIO_Initure);

    HAL_GPIO_WritePin(ESP8266_GPIO, ESP8266_RST_PIN, GPIO_PIN_SET);			//复位引脚，低电平有效
    HAL_GPIO_WritePin(ESP8266_GPIO, ESP8266_FIRMWARE_PIN, GPIO_PIN_SET);	//默认高电平是运行模式
}

void esp8266_RST_high(void)
{
    HAL_GPIO_WritePin(ESP8266_GPIO, ESP8266_RST_PIN, GPIO_PIN_SET);
    esp8266_reset_status = false;
}

void esp8266_RST_low(void)
{
    HAL_GPIO_WritePin(ESP8266_GPIO, ESP8266_RST_PIN, GPIO_PIN_RESET);
    esp8266_reset_status = true;
}

void esp8266_write_firmware_mode(void)
{   /*烧写固件模式*/
    HAL_GPIO_WritePin(ESP8266_GPIO, ESP8266_FIRMWARE_PIN, GPIO_PIN_RESET);	//默认高电平是运行模式
    esp8266_run_mode_status = false;
}

void esp8266_run_mode(void)
{   /*正常运行模式*/
	HAL_GPIO_WritePin(ESP8266_GPIO, ESP8266_FIRMWARE_PIN, GPIO_PIN_SET);	//默认高电平是运行模式
    esp8266_run_mode_status = true;
}

bool get_reset_status(void)
{   /*获取是否复位的状态*/
    return esp8266_reset_status;
}

bool get_run_mode_status(void)
{
	return esp8266_run_mode_status;
}
