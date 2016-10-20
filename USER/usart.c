#include "stm32f10x.h"
#include "usart.h"

unsigned char RETURN_BUF[1500];//接收GSM返回数据的BUF

//初始化USART1，关闭中断，接收发送使能 ，使用DMA1通道4作为发送通道，DMA1通道5作为接收通道，通道4状态关闭，通道5状态开启
void USART1_Config()
{
 USART_InitTypeDef USART_InitStructure;

 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

 USART_InitStructure.USART_BaudRate=115200;
 USART_InitStructure.USART_StopBits=USART_StopBits_1;
 USART_InitStructure.USART_WordLength=USART_WordLength_8b;
 USART_InitStructure.USART_Parity=USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
 USART_Init(USART1,&USART_InitStructure);
 USART_Cmd(USART1,ENABLE);
 USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);




 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOA,&GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOA,&GPIO_InitStructure);


 
					

}
//PA2 USART2 TX
//PA3 USART2 RX
//PA0,作为待机模式的唤醒引脚
//初始化USART2及其引脚，关闭其中断	，接收发送使能
void USART2_Config()
{
 USART_InitTypeDef USART_InitStructure;
 GPIO_InitTypeDef GPIO_InitStructure;


 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);



 USART_InitStructure.USART_BaudRate=9600;
 USART_InitStructure.USART_StopBits=USART_StopBits_1;
 USART_InitStructure.USART_WordLength=USART_WordLength_8b;
 USART_InitStructure.USART_Parity=USART_Parity_No;
 USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
 USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
 USART_Init(USART2,&USART_InitStructure);
 USART_Cmd(USART2,ENABLE);

  
 

 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOA,&GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_0;				 
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 
					

}
//发送一个字符
void putc(USART_TypeDef* USARTx,unsigned char c)
{ 
    USART_SendData(USARTx,c);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)!=SET);

}
//接收一个字符
unsigned char getc(USART_TypeDef* USARTx)
{  
   while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE)==RESET);
   return USART_ReceiveData(USARTx);
   
}
//发送一个字符串到串口
void putstr(USART_TypeDef* USARTx,unsigned char *c)
{	
   	unsigned char *pdata=c;
    while(*pdata!='\0')
     {
	  	  USART_SendData(USARTx,*pdata++);
        while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)!=SET);
		 
	  }
}
//将一个8位无符号数以16进制发送到串口
void puthex(USART_TypeDef* USARTx,unsigned char c)
{
   putstr(USARTx,"0x"); 
   puthexunit(USARTx,c>>4);
   puthexunit(USARTx,c);
	      
}
//将一个4位无符号数以16进制发送到串口
void puthexunit(USART_TypeDef* USARTx,unsigned char c)
{
  switch(c & 0x0f)
    {
	  case 0x00:
	      putc(USARTx,'0');
		  break;
	  case 0x01:
	      putc(USARTx,'1');
		  break;
	  case 0x02:
	      putc(USARTx,'2');
		  break;
	  case 0x03:
	      putc(USARTx,'3');
		  break;
	  case 0x04:
	      putc(USARTx,'4');
		  break;
	   case 0x05:
	      putc(USARTx,'5');
		  break;
	    case 0x06:
	      putc(USARTx,'6');
		  break;
		case 0x07:
	      putc(USARTx,'7');
		  break;
		case 0x08:
	      putc(USARTx,'8');
		  break;
		case 0x09:
	      putc(USARTx,'9');
		  break;
		case 0x0A:
	      putc(USARTx,'A');
		  break;
		case 0x0B:
	      putc(USARTx,'B');
		  break;
		case 0x0C:
	      putc(USARTx,'C');
		  break;
		case 0x0D:
	      putc(USARTx,'D');
		  break;
		case 0x0E:
	      putc(USARTx,'E');
		  break;
		case 0x0F:
	      putc(USARTx,'F');
		  break;
		default:
		  break;

	}
}




