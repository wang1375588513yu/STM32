#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include "sys.h"
#include <string.h>
#include "common.h"

#define VERSION_CODE 0X0001

#define bool unsigned char
#define false 0
#define true 1
	
#define PIPE_BUF_LEN  			   	4096 
#define UART_TX_LEN					1200
#define PUBLIC_SEND_BUFSIZE	 		10240 

#pragma pack(1)
typedef struct PIPE_TYPE
{
	unsigned char pipe_buf[PIPE_BUF_LEN];  //存放信息的buffer
	volatile unsigned short in_pos;   //当前进入buffer的位置
	volatile unsigned short out_pos;  //当前出buffer的位置
	volatile unsigned short sum;     //当前buffer的剩余字节数
	volatile unsigned int time;
}Pipe_t;

typedef struct PIPE1_TYPE
{
	unsigned char *buf;  //存放信息的buffer
	volatile unsigned short size;//buf的大小
	volatile unsigned short pin;   //当前进入buffer的位置
	volatile unsigned short pout;  //当前出buffer的位置
	volatile unsigned short sum;     //当前buffer的剩余字节数
	volatile unsigned int time;
}Pipe1_t;


typedef struct HCM300B_DATA
{
	float pitch;
	float roll;
	float heading;
	float temperature;	
}HCM300B_Data_t;

typedef struct SERVER_INFO
{
	unsigned int ip;
	unsigned short port;
}Server_Info_t;
#pragma pack()


#define VERSON_HIGH 0X00
#define VERSON_LOW  (28)
extern bool remote_keep_control_enable;
extern unsigned int remote_keep_control_delay;


#define  UART_TX_LEN	  1200  

extern unsigned char _txBuffer[UART_TX_LEN];

/*****************************************PUBLIC INTERFACE************************************************/
void SoftReset(void);
unsigned char CheckSum(const void *buf,int len);
unsigned char CheckXOR(const void *buf,int len);
void Pipe_Reset(Pipe_t *pipe);
void Pipe_Remove(Pipe_t *pipe,unsigned int len);
long long NMEA_Pow(unsigned char m,unsigned char n);
long long NMEA_Str2num(const unsigned char *buf,unsigned char jinzhi,unsigned char *dx);
unsigned long long HEX2DEC(unsigned long long BCDHEX);
unsigned long long DEC2HEX(unsigned long long DEC10);
unsigned short Caculate_CRC16(unsigned char *DAT, int Lenth);
bool isBCDencodeing(unsigned long long BCDHEX);


void Pipe1_Reset(Pipe1_t *pipe);
void Pipe1_Remove(Pipe1_t *pipe,unsigned int len);
bool Pipe_Block_Input(Pipe1_t *pipe,const char*packet,unsigned short packetlen);
bool Pipe_Check(Pipe1_t *pipe);

unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char *ucBuffer); 
bool IPaddressPhare(const char *string,unsigned char (*ip)[4], unsigned int len);
char* GetBuffer(void);


signed int st_strlen(unsigned char* str);
signed int st_strcmp(unsigned char *s,unsigned char *t);
unsigned char *st_strcpy(unsigned char *t,unsigned char *s);
signed int st_strncmp(unsigned char *dest,unsigned char *src,unsigned int Len);
signed char st_toupper(unsigned char c);
unsigned char *st_strcat(unsigned char *s,unsigned char *t);
signed int st_memcpy(unsigned char *dest, unsigned char *src, unsigned int Len);
signed int st_memset(unsigned char *dest,unsigned char byte, unsigned int Len);
signed int st_memcmp(unsigned char* dest, unsigned char* src, unsigned int Len);

#endif

