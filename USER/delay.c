#include "stm32f10x.h"

unsigned int timer_flag;   //��ʱ�õļ�������
 //SYSTICK��ʼ��
void systicker_init()
{
   SysTick_Config(SystemCoreClock / 1000); //���������core_cm3.h����������ж����ȼ�Ϊ0�������  �������˻�����ʱʱ��Ϊ10ms
    SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;	//��ʼ״̬Ϊ�ر�
	
}
//��ʱ��������ʱ��λΪ10ms
void delay(unsigned int ms)
{
   timer_flag=ms;
   SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk;
   while(timer_flag!=0);
   
   
}
