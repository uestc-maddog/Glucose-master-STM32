#include "stm32f10x.h"

unsigned int timer_flag;   //延时用的计数变量
 //SYSTICK初始化
void systicker_init()
{
   SysTick_Config(SystemCoreClock / 1000); //这个函数在core_cm3.h里，设置了其中断优先级为0，即最高  ，设置了基础计时时间为10ms
    SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;	//初始状态为关闭
	
}
//延时函数，计时单位为10ms
void delay(unsigned int ms)
{
   timer_flag=ms;
   SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk;
   while(timer_flag!=0);
   
   
}
