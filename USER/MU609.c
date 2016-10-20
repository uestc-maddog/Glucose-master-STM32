#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "it.h"

extern unsigned char RETURN_BUF[1500];	//接收返回的BUF
unsigned int  RETURN_COUNTER;
extern unsigned char GLUCOSE_BUF[200];
unsigned int GSM_TIMER_FLAG;
unsigned char embed_num=0;

enum GSM_STATE{WAIT_CARD_READY,CARD_FAILED,NET_INIT,NET_INIT_FAILED,LINK_TO_SERVER,LINK_ERROR,ENTRANSE,ENTRANSE_ERROR,SERVER_OK,SERVER_ERROR};

enum GSM_STATE MU609_STATE=WAIT_CARD_READY;
#define GSM_RST_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_1);
#define GSM_RST_LOW	   GPIO_ResetBits(GPIOA,GPIO_Pin_1);

//复位引脚初始化,PA1
void mu609_gpio_init()
{ 
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
//给GSM模块发送命令串
void GSM_start()
{   
    unsigned int i,n,j;			   //局部计数变量
	unsigned char flag;			   //局部标志
    mu609_gpio_init();
	//复位GSM
	GSM_RST_LOW;
	delay(1000);
	GSM_RST_HIGH;
	
	
	MU609_STATE=WAIT_CARD_READY;		 
	RETURN_COUNTER=0;	//清零返回BUF的指针，使接收数据能从头开始存储
	flag=0;		        //标志是否检测到OK
   //等待SIMST：1 这个字符串出现，即检测SIM卡
    oled_show_str("Searching SIM Card");
	GSM_TIMER_FLAG=20000;
	SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk;   
	while(1)
	{
	            delay(500);
            	n=RETURN_COUNTER;
             	for(i=0;i<n;i++)
	             {
	                 if(RETURN_BUF[i]=='S')
						   if(RETURN_BUF[i+2]=='M')
							    if(RETURN_BUF[i+4]=='T')
								   if(RETURN_BUF[i+7]=='1')
								     {  
								   	   flag=1;
									   break;
						       		  }
	     
	             }
				 if(flag==1)
				   break;
				if(GSM_TIMER_FLAG==0)
				   { 
				     MU609_STATE=CARD_FAILED;
				     oled_show_str("SIM Card Failed");
					 while(1);
				   }
     }	  //检查SIMST:1返回
	oled_show_str("SIM Card OK!");
	delay(1000);



	//初始化网络连接并检测返回的OK
	//发送命令后等待6秒，如果未返回ok则
	MU609_STATE=NET_INIT;
	oled_show_str("create net");			
    j=0;
	while(1)
	{
	j++;
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);  //发送命令时不允许接收，保证接收到的数据都是该命令的返回
    RETURN_COUNTER=0;	//清零返回BUF的指针，使接收数据能从头开始存储
	flag=0;		        //标志是否检测到OK
    putstr(USART1,"AT^IPINIT=\"1234\"\r\n");
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	GSM_TIMER_FLAG=6000;
	while(1)
        	{
	            delay(200);
	            n=RETURN_COUNTER;
	            for(i=0;i<n;i++)
	               {
		             if(RETURN_BUF[i]=='O')
		                 if(RETURN_BUF[i+1]=='K')
			                 {
			                   flag=1;
				               break;
			                  }
		            }
	            if(flag==1)
	                break;
				if(GSM_TIMER_FLAG==0)
				    break;
         	}
	 if(flag==1)  //接收到OK则退出
	     break;
	 if(j>2)  //重发次数超过两次
	    {
		   oled_show_str("create net failed");
		   delay(2000);
		   embed_num++;
		   if(embed_num>3)
		     { 
			   MU609_STATE=NET_INIT_FAILED; 
			   oled_show_str("ERROR UNKNOWN");	 //如果嵌套层数超过三，则停止，等待重启
		        while(1);
			 }
		   GSM_start();	   //重新执行这个函数
		   return;
		}
	 
	}

    

	//连接到服务器并检测返回的ok
	oled_show_str("link the server");
	MU609_STATE=LINK_TO_SERVER;
	j=0;
	while(1)
	{
	j++;
	RETURN_COUNTER=0;
	flag=0;
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
    putstr(USART1,"AT^IPOPEN=1,\"TCP\",\"choret.6655.la\",80\r\n");
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	GSM_TIMER_FLAG=6000;
	while(1)
        	{
	            delay(200);
	            n=RETURN_COUNTER;
	            for(i=0;i<n;i++)
	               {
		             if(RETURN_BUF[i]=='O')
		                 if(RETURN_BUF[i+1]=='K')
			                 {
			                   flag=1;
				               break;
			                  }
		            }
	            if(flag==1)
	                break;
         	}
	 if(flag==1)
	     break;
	 if(j>2)	//重发次数超过两次
	    {
		   oled_show_str("link server failed");
		   delay(2000);
		   embed_num++;
		   if(embed_num>3)
		     {  
			 	MU609_STATE=LINK_ERROR;
			    oled_show_str("ERROR UNKNOWN");	 //如果嵌套层数超过三，则停止，等待重启
		        while(1);
			 }
		   GSM_start();	   //重新执行这个函数
		   return;
		}
	}
	oled_show_str("link ok!");
	delay(100);




	//开启透传模式并检测返回的ok
	MU609_STATE=ENTRANSE;
	j=0;   //代表发送循环的次数
	while(1)
	{
	j++;
	RETURN_COUNTER=0;
	flag=0;
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
    putstr(USART1,"AT^IPENTRANS=1\r\n");
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	while(1)
        	{
	            delay(200);
	            n=RETURN_COUNTER;
	            for(i=0;i<n;i++)
	               {
		             if(RETURN_BUF[i]=='O')
		                 if(RETURN_BUF[i+1]=='K')
			                 {
			                   flag=1;
				               break;
			                  }
		            }
	            if(flag==1)
	                break;
         	}
	 if(flag==1)  //检测到OK即退出
	     break;
	 if(j>2)	//重发次数超过两次
	    {
		   oled_show_str("ENTRANS FAILED");
		   delay(2000);
		   embed_num++;
		   if(embed_num>3)
		     {  
			    MU609_STATE=ENTRANSE_ERROR;
				oled_show_str("ERROR UNKNOWN");	 //如果嵌套层数超过三，则停止，等待重启
		        while(1);
			 }
		   GSM_start();	   //重新执行这个函数
		   return;
		}

     }





    //发送携带数据的HTTP报文，并检测返回的OK
	oled_show_str("now,send!");

	RETURN_COUNTER=0;
	flag=0;
	USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);

    putstr(USART1,"GET /xgl/mu609.jsp?");
	putstr(USART1,"name=glucose&glucose=");
	for(i=0;i<8;i++)
	   putc(USART1,GLUCOSE_BUF[i]);
	putstr(USART1," HTTP/1.1\r\nHost: choret.6655.la\r\n\r\n");
	putstr(USART1,"+++");
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	GSM_TIMER_FLAG=10000;
	while(1)
        	{
	            delay(200);
	            n=RETURN_COUNTER;
	            for(i=0;i<n;i++)
	               {
		             if(RETURN_BUF[i]=='O')
		                 if(RETURN_BUF[i+1]=='K')
			                 {
			                   flag=1;
				               break;
			                  }
		            }
	            if(flag==1)
				  {
				    MU609_STATE=SERVER_OK;
	                break;
				  }
				if(GSM_TIMER_FLAG==0)
				   {
				     MU609_STATE=SERVER_ERROR;
                     oled_show_str("Server ERROR");
					 while(1);

				   }
         	}
	RETURN_COUNTER=0;
	flag=0;
	oled_show_str("send ok!");
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==Bit_SET);	//等待血糖仪拔出
	oled_show_str("go suspend..");
	delay(2000);
	go_suspend();
								  
   
	

	

	
}





