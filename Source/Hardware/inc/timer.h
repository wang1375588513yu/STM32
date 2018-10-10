#ifndef __TIMER_H__
#define __TIMER_H__

#include "sys.h"

void TIM2_Init(u16 arr,u16 psc);
void TIM3_Init(u16 arr,u16 psc);

unsigned int Get_SysmilliTick(void);
unsigned int Get_SysRunTick(void);
unsigned int Get_SysTick(void);

void Set_MillisecTimer_Wifi_delay(unsigned int delay);
unsigned int Get_MillisecTimer_Wifi_delay(void);

#endif

