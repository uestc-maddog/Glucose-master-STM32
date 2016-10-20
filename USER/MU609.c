#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "it.h"

extern unsigned char RETURN_BUF[1500];	//���շ��ص�BUF
unsigned int  RETURN_COUNTER;
extern unsigned char GLUCOSE_BUF[200];
unsigned int GSM_TIMER_FLAG;
unsigned char embed_num=0;

enum GSM_STATE{WAIT_CARD_READY,CARD_FAILED,NET_INIT,NET_INIT_FAILED,LINK_TO_SERVER,LINK_ERROR,ENTRANSE,ENTRANSE_ERROR,SERVER_OK,SERVER_ERROR};

enum GSM_STATE MU609_STATE=WAIT_CARD_READY;
#define GSM_RST_HIGH   GPIO_SetBits(GPIOA,GPIO_Pin_1);
#define GSM_RST_LOW	   GPIO_ResetBits(GPIOA,GPIO_Pin_1);

//��λ���ų�ʼ��,PA1
void mu609_gpio_init()
{ 
  
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
}
//��GSMģ�鷢�����
void GSM_start()
{   
    unsigned int i,n,j;			   //�ֲ���������
	unsigned char flag;			   //�ֲ���־
    mu609_gpio_init();
	//��λGSM
	GSM_RST_LOW;
	delay(1000);
	GSM_RST_HIGH;
	
	
	MU609_STATE=WAIT_CARD_READY;		 
	RETURN_COUNTER=0;	//���㷵��BUF��ָ�룬ʹ���������ܴ�ͷ��ʼ�洢
	flag=0;		        //��־�Ƿ��⵽OK
   //�ȴ�SIMST��1 ����ַ������֣������SIM��
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
     }	  //���SIMST:1����
	oled_show_str("SIM Card OK!");
	delay(1000);



	//��ʼ���������Ӳ���ⷵ�ص�OK
	//���������ȴ�6�룬���δ����ok��
	MU609_STATE=NET_INIT;
	oled_show_str("create net");			
    j=0;
	while(1)
	{
	j++;
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);  //��������ʱ��������գ���֤���յ������ݶ��Ǹ�����ķ���
    RETURN_COUNTER=0;	//���㷵��BUF��ָ�룬ʹ���������ܴ�ͷ��ʼ�洢
	flag=0;		        //��־�Ƿ��⵽OK
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
	 if(flag==1)  //���յ�OK���˳�
	     break;
	 if(j>2)  //�ط�������������
	    {
		   oled_show_str("create net failed");
		   delay(2000);
		   embed_num++;
		   if(embed_num>3)
		     { 
			   MU609_STATE=NET_INIT_FAILED; 
			   oled_show_str("ERROR UNKNOWN");	 //���Ƕ�ײ�������������ֹͣ���ȴ�����
		        while(1);
			 }
		   GSM_start();	   //����ִ���������
		   return;
		}
	 
	}

    

	//���ӵ�����������ⷵ�ص�ok
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
	 if(j>2)	//�ط�������������
	    {
		   oled_show_str("link server failed");
		   delay(2000);
		   embed_num++;
		   if(embed_num>3)
		     {  
			 	MU609_STATE=LINK_ERROR;
			    oled_show_str("ERROR UNKNOWN");	 //���Ƕ�ײ�������������ֹͣ���ȴ�����
		        while(1);
			 }
		   GSM_start();	   //����ִ���������
		   return;
		}
	}
	oled_show_str("link ok!");
	delay(100);




	//����͸��ģʽ����ⷵ�ص�ok
	MU609_STATE=ENTRANSE;
	j=0;   //������ѭ���Ĵ���
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
	 if(flag==1)  //��⵽OK���˳�
	     break;
	 if(j>2)	//�ط�������������
	    {
		   oled_show_str("ENTRANS FAILED");
		   delay(2000);
		   embed_num++;
		   if(embed_num>3)
		     {  
			    MU609_STATE=ENTRANSE_ERROR;
				oled_show_str("ERROR UNKNOWN");	 //���Ƕ�ײ�������������ֹͣ���ȴ�����
		        while(1);
			 }
		   GSM_start();	   //����ִ���������
		   return;
		}

     }





    //����Я�����ݵ�HTTP���ģ�����ⷵ�ص�OK
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
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==Bit_SET);	//�ȴ�Ѫ���ǰγ�
	oled_show_str("go suspend..");
	delay(2000);
	go_suspend();
								  
   
	

	

	
}





