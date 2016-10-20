#include "stm32f10x.h"


//SPI1
//PA4 NSS
//PA5 SCK
//PA6 MISO
//PA7 MOSI


//PA4 DC PA8 RES


//初始化SPI1，以及复位引脚，，命令数据切换引脚 
void SPI1_Init()
{
   SPI_InitTypeDef  SPI_InitStructure;
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1,ENABLE);



  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;   //上拉输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_8;		 //cs
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);


   SPI_InitStructure.SPI_Direction   = 	SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode        =  SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize    =  SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL        =  SPI_CPOL_Low;
   SPI_InitStructure.SPI_CPHA        =  SPI_CPHA_1Edge;
   SPI_InitStructure.SPI_NSS		 =  SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler  = SPI_BaudRatePrescaler_256;//lowest
   SPI_InitStructure.SPI_FirstBit    =  SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial      = 7;
   
   SPI_Init(SPI1,&SPI_InitStructure);
   SPI_Cmd(SPI1,ENABLE);
 

}
//SPI1读取和发送数据
unsigned char SPI1_RWByte(unsigned char dat)
{
  
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
  SPI_I2S_SendData(SPI1,dat);
  while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
  return SPI_I2S_ReceiveData(SPI1);
}
