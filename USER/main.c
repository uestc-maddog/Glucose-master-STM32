#include "stm32f10x.h"
#include <string.h>
#include "it.h"
//#include "lib.h"
#include "spi.h"
#include "mystm32.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include "usart.h"
#include "oled.h"
#include "MU609.h"
#include "interact.h"
#include "glucose.h"
#include "delay.h"
#include "adc.h"

//extern const unsigned char F6x8[92][6];


//全局初始化函数			 
void my_init()
{
   USART1_Config();
   USART2_Config();
   IT_Config();
   systicker_init();
   SPI1_Init();		  
   initial_oled();	   
   adc1_init();
   delay(100);
   while(1)
   {
   delay(500);
   show_voltage();
 }
   
}

int main()
{
    my_init();
/*	delay(100);
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)!=Bit_SET)
	    { oled_show_str("go suspend..");
	      go_suspend();
							}	  	 			
    Get_GluData();
	GSM_start();	   

   					*/
	
    while(1);
 }
