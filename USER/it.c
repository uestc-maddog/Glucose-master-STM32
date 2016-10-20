#include "stm32f10x.h"

//用于设置全局中断及其优先级
void IT_Config()
{
   NVIC_InitTypeDef  NVIC_InitStructure;
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
   NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;   
   NVIC_Init(&NVIC_InitStructure);

   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
   NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
   NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;   
   NVIC_Init(&NVIC_InitStructure);

			

}
//WFI指令，进入待机模式
//__asm void wfi()
//{
//   WFI;
//}
//设置各种待机模式前的准备条件
void go_suspend()
{
  	 SCB->SCR |=(1<<2);
	 RCC->APB1ENR|=(1<<28);
	 PWR->CR|=(1<<1)|(1<<2);
	 PWR->CSR|=(1<<8);
	 wfi();
}
