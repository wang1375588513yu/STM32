/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2018, WangYu, China, SZ.
**                           All Rights Reserved
**
**                        By(~~~~~~~~~~~~~~~~~~~~~~~)
**
**----------------------------------File Information----------------------------
** FileName:      usart.c
** The Author:    WangYu
** Creation Date: 2018-09-05
** Description:
**
**--------------------------------Version Information---------------------------
** Version NO: V1.0
** Version Description: Initial Version
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/
#include "usart.h"
#include "timer.h"
#include "PUBLIC.H"

#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

#define RXBUFFERSIZE   1 //缓存大小
u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
UART_HandleTypeDef 	UART1_Handler;
UART_HandleTypeDef 	UART2_Handler; 
UART_HandleTypeDef 	UART3_Handler;
UART_HandleTypeDef 	UART4_Handler; 
GPIO_InitTypeDef 	GPIO_Init; 

#if UART1_ENABLE
#if USART1_USE_PIPE
Pipe_t uart1_pipe = {0};
#else
Pipe1_t uart1_pipe = {0};
#endif
void reset_uart_pipe1()
{
    #ifdef USART1_USE_PIPE
    Pipe_Reset(&uart1_pipe);
	#else
	Pipe1_Reset(&uart1_pipe);
	#endif
}
void UART1_init(u32 bound)
{     
    __HAL_RCC_UART1_GPIO_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    //PA9 Tx
    GPIO_Init.Pin 		= UART1_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(UART1_GPIO, &GPIO_Init);

    //PA10 Rx
    GPIO_Init.Pin 		= UART1_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(UART1_GPIO, &GPIO_Init);

    UART1_Handler.Instance          = USART1;
    UART1_Handler.Init.BaudRate     = bound;
    UART1_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART1_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART1_Handler.Init.Parity       = UART_PARITY_NONE;
    UART1_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART1_Handler);
	
	HAL_NVIC_EnableIRQ(USART1_IRQn);		//使能USART2中断通道
    HAL_NVIC_SetPriority(USART1_IRQn,3,3);	//抢占优先级3，子优先级3

    /*该函数会开启接收中断,标志位UART_IT_RXNE置位*/
    /*并且设置接收缓冲和接收缓冲最大的数量*/
	HAL_UART_Receive_IT(&UART1_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe1();
}

void USART1_IRQHandler(void)
{
	u8 Res;
    if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
		Res=USART1->RDR; 	//接收数据
		uart1_printf((char*)&Res);
	//	Pipe_Block_Input(&uart1_pipe,(const char*)&Res,1);
        uart1_pipe.pipe_buf[uart1_pipe.in_pos]=Res;
        uart1_pipe.in_pos++;
        uart1_pipe.in_pos%=PIPE_BUF_LEN;
        if(PIPE_BUF_LEN > uart1_pipe.sum)	
			uart1_pipe.sum++;
        else	
			uart1_pipe.out_pos=uart1_pipe.in_pos;
        uart1_pipe.time=Get_SysmilliTick();
    }
}

void uart1_output(const unsigned char * buf,int len)
{
    unsigned int __i,timespan = Get_SysmilliTick();
    if(buf==NULL ||len==0)  return;
	
    for(__i = 0; __i < len; __i++)
    {
        while((USART1->ISR & USART_FLAG_TC) == (uint16_t)RESET)//获取传输完成标志TC transmission complete
        {//有时候会死在这里 
            if(Get_SysmilliTick()-timespan>10)
            {/*添加一个定时器时间管理，如果在while里面停留超过10ms就自动跳出来*/
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        USART1->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);
    }
}

void uart1_printf(char * string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart1_output((const unsigned char *)_txBuffer,len);
}
#endif

#if UART2_ENABLE
#if USART2_USE_PIPE == 1
Pipe_t uart2_pipe = {0};
#else
Pipe1_t uart2_pipe = {0};
#define USART2_RX_LOG_BUFER_SIZE 200
unsigned char USART2_RX_LOG_BUFER[USART2_RX_LOG_BUFER_SIZE];
Pipe1_t uart_pipe2_log={USART2_RX_LOG_BUFER,USART2_RX_LOG_BUFER_SIZE,0,0,0,0};
#endif
void reset_uart_pipe2()
{
   	#if USART2_USE_PIPE == 1
    Pipe_Reset(&uart2_pipe);
	#else
	Pipe1_Reset(&uart2_pipe);
	#endif
}

void UART2_init(u32 bound)
{
    __HAL_RCC_UART2_GPIO_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    //PA2 Tx
    GPIO_Init.Pin 		= UART2_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(UART2_GPIO, &GPIO_Init);

    //PA3 Rx
    GPIO_Init.Pin 		= UART2_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(UART2_GPIO, &GPIO_Init);

    HAL_NVIC_EnableIRQ(USART2_IRQn);		//使能USART2中断通道
    HAL_NVIC_SetPriority(USART2_IRQn,3,3);	//抢占优先级3，子优先级3

    UART2_Handler.Instance          = USART2;
    UART2_Handler.Init.BaudRate     = bound;
    UART2_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART2_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART2_Handler.Init.Parity       = UART_PARITY_NONE;
    UART2_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART2_Handler);

    /*该函数会开启接收中断,标志位UART_IT_RXNE置位*/
    /*并且设置接收缓冲和接收缓冲最大的数量*/
    HAL_UART_Receive_IT(&UART2_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe2();
}

//串口2中断服务程序
void USART2_IRQHandler(void)
{
    u8 Res;
    if((__HAL_UART_GET_FLAG(&UART2_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
		Res=USART2->RDR; 	//接收数据
		uart2_printf((char*)&Res);
		Pipe_Block_Input(&uart2_pipe,(const char*)&Res,1);
		Pipe_Block_Input(&uart_pipe2_log,(const char*)&Res,1);
    }
}

void uart2_output(const unsigned char * buf,int len)
{
    unsigned int __i,timespan = Get_SysmilliTick();

    if(buf==NULL ||len==0)  return;
    for(__i = 0; __i < len; __i++)
    {
        //  while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {};
        while((USART2->ISR & USART_FLAG_TC) == (uint16_t)RESET)//获取传输完成标志TC transmission complete
        {//有时候会死在这里 
            if(Get_SysmilliTick()-timespan>10)
            {
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        USART2->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);// USART_SendData(USART2,(uint8_t) buf[__i]);
    }
}

void uart2_printf(char * string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart2_output((const unsigned char *)_txBuffer,len);
}
#endif

#if UART3_ENABLE
#if USART3_USE_PIPE
Pipe_t uart3_pipe = {0};
#else
Pipe1_t uart3_pipe = {0};
#endif
void reset_uart_pipe3()
{
    #if USART3_USE_PIPE == 1
    Pipe_Reset(&uart3_pipe);
	#else
	Pipe1_Reset(&uart3_pipe);
	#endif
}

void UART3_init(u32 bound)
{
    __HAL_RCC_UART3_GPIO_CLK_ENABLE();
    __HAL_RCC_USART3_CLK_ENABLE();

    //PB10 Tx
    GPIO_Init.Pin 		= UART3_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(UART3_GPIO, &GPIO_Init);

    //PB11 Rx
    GPIO_Init.Pin 		= UART3_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(UART3_GPIO, &GPIO_Init);

    UART3_Handler.Instance          = USART3;
    UART3_Handler.Init.BaudRate     = bound;
    UART3_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART3_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART3_Handler.Init.Parity       = UART_PARITY_NONE;
    UART3_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART3_Handler);

	HAL_NVIC_EnableIRQ(USART3_IRQn);		//使能USART2中断通道
    HAL_NVIC_SetPriority(USART3_IRQn,3,3);	//抢占优先级3，子优先级3
    /*该函数会开启接收中断,标志位UART_IT_RXNE置位*/
    /*并且设置接收缓冲和接收缓冲最大的数量*/
    HAL_UART_Receive_IT(&UART3_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe3();
}

//串口3中断服务程序
void USART3_IRQHandler(void)
{
    u8 Res;
    if((__HAL_UART_GET_FLAG(&UART3_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
		Res=USART3->RDR; 	//接收数据
		uart3_printf((char*)&Res);
//		Pipe_Block_Input(&uart3_pipe,(const char*)&Res,1);
//        uart3_pipe.pipe_buf[uart3_pipe.in_pos]=Res;
//        uart3_pipe.in_pos++;
//        uart3_pipe.in_pos%=PIPE_BUF_LEN;
//        if(PIPE_BUF_LEN > uart3_pipe.sum)	
//			uart3_pipe.sum++;
//        else	
//			uart3_pipe.out_pos=uart3_pipe.in_pos;
//        uart3_pipe.time=Get_SysmilliTick();
    }
}


void uart3_output(const unsigned char * buf,int len)
{
    unsigned int __i,timespan = Get_SysmilliTick();

    if(buf==NULL ||len==0)  return;
    for(__i = 0; __i < len; __i++)
    {
        //  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {};
        while((USART3->ISR & USART_FLAG_TC) == (uint16_t)RESET)//获取传输完成标志TC transmission complete
        {//有时候会死在这里 
            if(Get_SysmilliTick()-timespan>10)
            {
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        USART3->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);// USART_SendData(USART3,(uint8_t) buf[__i]);
    }
}

void uart3_printf(char * string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart3_output((const unsigned char *)_txBuffer,len);
}

#endif

#if UART4_ENABLE
#if USART4_USE_PIPE
Pipe_t uart4_pipe = {0};
#else
Pipe1_t uart4_pipe = {0};
#endif

void reset_uart_pipe4()
{
	#if USART4_USE_PIPE
    Pipe_Reset(&uart4_pipe);
	#else
	Pipe1_Reset(&uart4_pipe);
	#endif
}
void UART4_init(u32 bound)
{
    __HAL_RCC_UART4_GPIO_CLK_ENABLE();
    __HAL_RCC_UART4_CLK_ENABLE();

    //PC10 Tx
    GPIO_Init.Pin 		= UART4_TX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(UART4_GPIO, &GPIO_Init);

    //PC11 Rx
    GPIO_Init.Pin 		= UART4_RX_PIN;
    GPIO_Init.Mode 		= GPIO_MODE_AF_PP;
    GPIO_Init.Pull 		= GPIO_PULLUP;
    GPIO_Init.Speed 	= GPIO_SPEED_FREQ_HIGH;
    GPIO_Init.Alternate = GPIO_AF8_UART4;
    HAL_GPIO_Init(UART4_GPIO, &GPIO_Init);

    UART4_Handler.Instance          = UART4;
    UART4_Handler.Init.BaudRate     = bound;
    UART4_Handler.Init.WordLength   = UART_WORDLENGTH_8B;
    UART4_Handler.Init.StopBits     = UART_STOPBITS_1;
    UART4_Handler.Init.Parity       = UART_PARITY_NONE;
    UART4_Handler.Init.Mode         = UART_MODE_TX_RX;
    HAL_UART_Init(&UART4_Handler);

	HAL_NVIC_EnableIRQ(UART4_IRQn);		//使能USART2中断通道
    HAL_NVIC_SetPriority(UART4_IRQn,3,3);	//抢占优先级3，子优先级3
    /*该函数会开启接收中断,标志位UART_IT_RXNE置位*/
    /*并且设置接收缓冲和接收缓冲最大的数量*/
	HAL_UART_Receive_IT(&UART4_Handler, aRxBuffer, RXBUFFERSIZE);
    reset_uart_pipe4();
}

//串口4中断服务程序
void UART4_IRQHandler(void)
{
    u8 Res;
    if((__HAL_UART_GET_FLAG(&UART4_Handler,UART_FLAG_RXNE)!=RESET))  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
		Res=UART4->RDR; 	//接收数据
		uart4_printf((char*)&Res);
		Pipe_Block_Input(&uart4_pipe,(const char*)&Res,1);
    }
	
}


void uart4_output(const unsigned char * buf,int len)
{
    unsigned int __i,timespan = Get_SysmilliTick();

    if(buf==NULL ||len==0)  return;
    for(__i = 0; __i < len; __i++)
    {
        while((UART4->ISR & USART_FLAG_TC) == (uint16_t)RESET)//获取传输完成标志TC transmission complete
        {//有时候会死在这里 
            if(Get_SysmilliTick()-timespan>10)
            {
                break;
            }
        }		
        timespan = Get_SysmilliTick();
        UART4->TDR = ((uint8_t)buf[__i] & (uint16_t)0x01FF);
    }
}

void uart4_printf(char *string, ...)
{
    __va_list arg;
    int len=0;
    if(string==NULL)  return;

    if(strlen(string)>=UART_TX_LEN)return;
    va_start(arg, string);
    memset(_txBuffer,0,UART_TX_LEN);
    len=vsprintf((char *)_txBuffer, string, arg); /*must use "vsprintf" */
    va_end(arg);
    uart4_output((const unsigned char *)_txBuffer,len);
}

#endif

void usart_initinit(void)
{
#if UART1_ENABLE
    UART1_init(115200);
#endif

#if UART2_ENABLE
    UART2_init(115200);
#endif

#if UART3_ENABLE
    UART3_init(115200);
#endif

#if UART4_ENABLE
    UART4_init(115200);
#endif
}


