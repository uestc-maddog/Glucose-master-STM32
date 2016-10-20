#include "stm32f10x.h"


void adc1_init()
{
   GPIO_InitTypeDef GPIO_InitStructure;
   ADC_InitTypeDef ADC_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
 
   GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
   GPIO_Init(GPIOB,&GPIO_InitStructure);


   ADC_DeInit(ADC1);
   ADC_InitStructure.ADC_Mode              = ADC_Mode_Independent;  //����ģʽ
   ADC_InitStructure.ADC_ScanConvMode=DISABLE;
   ADC_InitStructure.ADC_ContinuousConvMode =DISABLE;      //����ת�� 
   ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_None; //ת������������ 
   ADC_InitStructure.ADC_DataAlign         =ADC_DataAlign_Right;   //�Ҷ���
   ADC_InitStructure.ADC_NbrOfChannel      =1;       //ɨ��ͨ���� 
   ADC_Init(ADC1,&ADC_InitStructure); 

   RCC_ADCCLKConfig(RCC_PCLK2_Div8);
   ADC_RegularChannelConfig(ADC1,ADC_Channel_8, 1,ADC_SampleTime_55Cycles5);
   ADC_Cmd(ADC1,ENABLE);  
  

   ADC_ResetCalibration(ADC1);
   while((ADC_GetResetCalibrationStatus(ADC1)));
   ADC_StartCalibration(ADC1);
   while((ADC_GetCalibrationStatus(ADC1)));
  
              


}

unsigned short get_voltage() 
{ 
   unsigned int adc; 
   unsigned char vol;
   ADC_ClearFlag(ADC1, ADC_FLAG_EOC); 
   ADC_SoftwareStartConvCmd(ADC1,ENABLE);
   while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==RESET); //����ƶ�ADC��־λ��1��� ADC_FLAG_EOC ת��������־λ 
   adc=ADC_GetConversionValue(ADC1);
   vol=(adc*330)>>12;
   return vol;//�������һ��ADCx�������ת����� 
}

