#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"

const unsigned char  GLUCOSE_CMD[]={0x80,'\0',0x8B,0x11,0x20,0x13,0x24,0x15,0x24,'\0',0x8B,0x1E,0x22,0x10,0x20,0x10,0x28,'\0'};
const unsigned char *GLUCOSE_START=GLUCOSE_CMD;//检查血糖仪是否存在命令的指针
const unsigned char *GLUCOSE_SNUMBER=&GLUCOSE_CMD[2];	//查询序列号命令的指针
const unsigned char *GLUCOSE_DATARRQST=&GLUCOSE_CMD[10];//查询数据命令的指针
unsigned char GLUCOSE_FLAG;	//用于指示接收是否完成
unsigned char GLUCOSE_BUF[200];	 //USART2接收BUF

//向血糖仪发送命令
void g_send_cmds(const unsigned char *pdata)
{
	unsigned char *pdata1=(unsigned char*)pdata;
	putstr(USART2,pdata1);
}
//检测血糖仪是否存在，存在返回1，不存在返回0
int Glucose_init()
{
   USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
   USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
   while(1)
      {
	    g_send_cmds(GLUCOSE_START);
		delay(10);
		if(GLUCOSE_FLAG==1)		//如接收到数据，USART2中断会将GLUCOSE_FLAG置1
		   	 break;
	  }
   if(GLUCOSE_BUF[0]==0x80)
      if(GLUCOSE_BUF[1]==0x10)
	     if(GLUCOSE_BUF[2]==0x20)
		 {
		     USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
             USART_ITConfig(USART2,USART_IT_IDLE,DISABLE);			  //反复开断中断的考虑是，使接收BUF处于一个可知的状态
		    return 1;				 //序列返回成功
		  }
        
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
    USART_ITConfig(USART2,USART_IT_IDLE,DISABLE);
    return 0;
 		   
}
//解析血糖仪返回的数据
void glu_parse()
{
   unsigned char n;
   for(n=0;n<8;n++)
    {
	  GLUCOSE_BUF[n]=(GLUCOSE_BUF[3*n+1]<<4)|(GLUCOSE_BUF[3*n+2]&0x0f);
	 
	}
	 GLUCOSE_BUF[8]='\0';
}
//读取血糖仪缓冲区的数据，数据被转换格式后保存在GLUCOSE_BUF前8个字节中
void Get_GluData()
{
      oled_show_str("reading data...");
	  while(1)		   //循环检测血糖仪是否存在
	  {
	     if(Glucose_init())
		    break;
		 delay(100);
		 
	  }
	  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
      USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);	  //使能接收中断和空闲中断，接收数据和判断一帧数据的结束
      g_send_cmds(GLUCOSE_DATARRQST);
      GLUCOSE_FLAG=0;
      while(GLUCOSE_FLAG==0);
	  glu_parse();
	  oled_show_str("read OK!!!");
	  delay(1000);
	    
	 
  
}
