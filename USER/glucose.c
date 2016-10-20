#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"

const unsigned char  GLUCOSE_CMD[]={0x80,'\0',0x8B,0x11,0x20,0x13,0x24,0x15,0x24,'\0',0x8B,0x1E,0x22,0x10,0x20,0x10,0x28,'\0'};
const unsigned char *GLUCOSE_START=GLUCOSE_CMD;//���Ѫ�����Ƿ���������ָ��
const unsigned char *GLUCOSE_SNUMBER=&GLUCOSE_CMD[2];	//��ѯ���к������ָ��
const unsigned char *GLUCOSE_DATARRQST=&GLUCOSE_CMD[10];//��ѯ���������ָ��
unsigned char GLUCOSE_FLAG;	//����ָʾ�����Ƿ����
unsigned char GLUCOSE_BUF[200];	 //USART2����BUF

//��Ѫ���Ƿ�������
void g_send_cmds(const unsigned char *pdata)
{
	unsigned char *pdata1=(unsigned char*)pdata;
	putstr(USART2,pdata1);
}
//���Ѫ�����Ƿ���ڣ����ڷ���1�������ڷ���0
int Glucose_init()
{
   USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
   USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
   while(1)
      {
	    g_send_cmds(GLUCOSE_START);
		delay(10);
		if(GLUCOSE_FLAG==1)		//����յ����ݣ�USART2�жϻὫGLUCOSE_FLAG��1
		   	 break;
	  }
   if(GLUCOSE_BUF[0]==0x80)
      if(GLUCOSE_BUF[1]==0x10)
	     if(GLUCOSE_BUF[2]==0x20)
		 {
		     USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
             USART_ITConfig(USART2,USART_IT_IDLE,DISABLE);			  //���������жϵĿ����ǣ�ʹ����BUF����һ����֪��״̬
		    return 1;				 //���з��سɹ�
		  }
        
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
    USART_ITConfig(USART2,USART_IT_IDLE,DISABLE);
    return 0;
 		   
}
//����Ѫ���Ƿ��ص�����
void glu_parse()
{
   unsigned char n;
   for(n=0;n<8;n++)
    {
	  GLUCOSE_BUF[n]=(GLUCOSE_BUF[3*n+1]<<4)|(GLUCOSE_BUF[3*n+2]&0x0f);
	 
	}
	 GLUCOSE_BUF[8]='\0';
}
//��ȡѪ���ǻ����������ݣ����ݱ�ת����ʽ�󱣴���GLUCOSE_BUFǰ8���ֽ���
void Get_GluData()
{
      oled_show_str("reading data...");
	  while(1)		   //ѭ�����Ѫ�����Ƿ����
	  {
	     if(Glucose_init())
		    break;
		 delay(100);
		 
	  }
	  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
      USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);	  //ʹ�ܽ����жϺͿ����жϣ��������ݺ��ж�һ֡���ݵĽ���
      g_send_cmds(GLUCOSE_DATARRQST);
      GLUCOSE_FLAG=0;
      while(GLUCOSE_FLAG==0);
	  glu_parse();
	  oled_show_str("read OK!!!");
	  delay(1000);
	    
	 
  
}
