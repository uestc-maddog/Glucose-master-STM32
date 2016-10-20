#include "stm32f10x.h"

#define led_on(x)  GPIO_ResetBits(GPIOB,x)
#define led_off(x) GPIO_SetBits(GPIOB,x)


void interact_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);


}

void show_startup()
{
   led_on(GPIO_Pin_5);
}
void show_suspend()
{
  led_off(GPIO_Pin_5);
}
