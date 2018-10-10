#include "adc.h"
#include "delay.h"

#define ADC_SAMPLE_COUNT 20
#define ADC_CHANNAL_COUNT 2

vu16 AD_Value[ADC_SAMPLE_COUNT][ADC_CHANNAL_COUNT];

ADC_HandleTypeDef ADC1_Handler;
void adc_init(void)
{
	ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   			//4��Ƶ��ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             			//12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             			//�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      			//��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                      			//�ر�EOC�ж�
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                			//�ر�����ת��
    ADC1_Handler.Init.NbrOfConversion=1;                         			//1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             			//��ֹ����������ģʽ
    ADC1_Handler.Init.NbrOfDiscConversion=0;                     			//����������ͨ����Ϊ0
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       			//��������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;	//ʹ����������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             			//�ر�DMA����
    HAL_ADC_Init(&ADC1_Handler);                                 			//��ʼ�� 
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
u16 Get_Adc(u32 ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
    ADC1_ChanConf.Rank=1;                                       //��1�����У�����1
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_247CYCLES_5;      //����ʱ��
    ADC1_ChanConf.Offset=0;                 
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
 	return (u16)HAL_ADC_GetValue(&ADC1_Handler);	        	//�������һ��ADC1�������ת�����
}

//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
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

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(��λ:��.)
float Get_CPU_Temprate(void)
{
    u32 adcx;
    float temperate;
	
    adcx=Get_Adc_Average(ADC_CHANNEL_17);	//��ȡͨ��16�ڲ��¶ȴ�����ͨ��,10��ȡƽ��
    temperate=(float)adcx*(3.3/4096);		//��ѹֵ
    temperate=(temperate-1.43)/0.0043 + 25; //ת��Ϊ�¶�ֵ
    return temperate;
}

