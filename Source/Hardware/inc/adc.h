#ifndef ___ADC_H___
#define ___ADC_H___
#include "sys.h"



u16 Get_Adc(u32 ch);
void adc_init(void);
u16 Get_Adc_Average(u32 nChannel); 
float Get_CPU_Temprate(void);

#endif
