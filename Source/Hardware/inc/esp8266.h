/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                       	 	By(MR.Richard)
**
**----------------------------------File Information----------------------------
** FileName:      esp8266.h
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

#ifndef ___ESP8266_H___
#define ___ESP8266_H___

#include "sys.h"
#include "PUBLIC.H"
#include "usart.h"

#define   ESP8266_GPIO							GPIOB
#define   ESP8266_RST_PIN						GPIO_PIN_1	//复位引脚
#define   ESP8266_FIRMWARE_PIN					GPIO_PIN_7	//固件烧写控制引脚
#define   __HAL_RCC_UART1_ESP8266_CLK_ENABLE() 	__HAL_RCC_GPIOB_CLK_ENABLE();

//#define   ON								GPIO_PIN_SET		
//#define   OFF								GPIO_PIN_RESET


void esp8266_hardware_init(void);
void esp8266_RST_high(void);
void esp8266_RST_low(void);
bool get_reset_status(void);
void esp8266_write_firmware_mode(void);
void esp8266_run_mode(void);
bool get_run_mode_status(void);

#endif

