#include "adc.h"
#include "delay.h"

#define ADC_SAMPLE_COUNT 20
#define ADC_CHANNAL_COUNT 2

vu16 AD_Value[ADC_SAMPLE_COUNT][ADC_CHANNAL_COUNT];

ADC_HandleTypeDef ADC1_Handler;
void adc_init(void)
{
	ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   			//4分频，ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             			//12位模式
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             			//右对齐
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      			//非扫描模式
    ADC1_Handler.Init.EOCSelection=DISABLE;                      			//关闭EOC中断
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                			//关闭连续转换
    ADC1_Handler.Init.NbrOfConversion=1;                         			//1个转换在规则序列中 也就是只转换规则序列1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             			//禁止不连续采样模式
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     			//不连续采样通道数为0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       			//软件触发
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;	//使用软件触发
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             			//关闭DMA请求
    HAL_ADC_Init(&ADC1_Handler);                                 			//初始化 
}

//获得ADC值
//ch: 通道值 0~16，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_16
//返回值:转换结果
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //通道
    ADC1_ChanConf.Rank=1;                                       //第1个序列，序列1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_247CYCLES_5;      //采样时间
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //通道配置
    HAL_ADC_Start(&ADC1_Handler);                               //开启ADC
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //轮询转换
 	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        	//返回最近一次ADC1规则组的转换结果
}

//获取指定通道的转换值，取times次,然后平均 
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u32 nChannel)
{
	u32 temp_val=0;
	u8 __i;
	for(__i=0;__i<ADC_SAMPLE_COUNT;__i++)
	{
		temp_val+=Get_Adc(nChannel);
		delay_ms(5);
	}
	return temp_val/ADC_SAMPLE_COUNT;
} 

//得到温度值
//返回值:温度值(单位:℃.)
float Get_CPU_Temprate(void)
{
    u32 adcx;
    float temperate;
	
    adcx=Get_Adc_Average(ADC_CHANNEL_17);	//读取通道16内部温度传感器通道,10次取平均
    temperate=(float)adcx*(3.3/4096);		//电压值
    temperate=(temperate-1.43)/0.0043 + 25; //转换为温度值
    return temperate;
}


