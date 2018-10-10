/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                       		 By(MR.Richard)
**
**----------------------------------File Information----------------------------
** FileName:      user_wifi.c
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

#include "timer.h"
#include "esp8266.h"
#include "user_wifi.h"

#define user_wifi_printf(fmt, args...) 	do{uart2_printf(fmt,##args);}while(0)	//发送AT指令
#define WIFI_AT_ENDSTR 					("\r\n")

Pipe1_t *user_wifi_recv_pipe=NULL;
#define WIFI_SEND_PIPE_MAXSIZE 		4096
unsigned char WIFI_SEND_PIPE_BUF[WIFI_SEND_PIPE_MAXSIZE];
Pipe1_t user_wifi_send_pipe= {WIFI_SEND_PIPE_BUF,WIFI_SEND_PIPE_MAXSIZE,0,0,0,0};



static void user_wifi_Pipe_Reset(void)
{
    Pipe1_Reset(user_wifi_recv_pipe);
}

void user_wifi_init(void)
{
    esp8266_hardware_init();	
	user_wifi_recv_pipe = &uart_pipe2_log;
	user_wifi_Pipe_Reset();
}

static void user_wifi_loop(void)
{
    static unsigned int softimersec=0xffffffff;
    if(softimersec==Get_SysTick())return;
    softimersec=Get_SysTick();


}

static void user_wifi_AT_loop(void)
{

}

static void user_wifi_parse_AT_loop(void)
{

}

static void Socket_normal_output(void)
{
    if(user_wifi_send_pipe.sum>0)
    {
        if((USART2->ISR & USART_FLAG_TC) == (uint16_t)SET ||(Get_SysmilliTick()-user_wifi_send_pipe.time > 10))
        {
            //如果可以发送就发送
            if(Get_MillisecTimer_Wifi_delay())return;

            user_wifi_send_pipe.time=Get_SysmilliTick();
			USART2->TDR = ((uint8_t)user_wifi_send_pipe.buf[user_wifi_send_pipe.pout++] & (uint16_t)0x01FF);
            user_wifi_send_pipe.pout %= user_wifi_send_pipe.size;
            user_wifi_send_pipe.sum--;
            if(user_wifi_send_pipe.pout%1000==0)
            {
                Set_MillisecTimer_Wifi_delay(100);
            }
        }
    }
}

void user_wifi_task(void)
{
    user_wifi_loop();
    user_wifi_parse_AT_loop();
    user_wifi_AT_loop();
	Socket_normal_output();
}





