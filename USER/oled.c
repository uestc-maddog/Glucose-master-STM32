#include "stm32f10x.h"
#include "lib.h"
#include "spi.h"
#include "delay.h"
#include "adc.h"


#define OLED_DATA()   GPIO_SetBits(GPIOA,GPIO_Pin_4)  //数据发送使能
#define OLED_CMD()    GPIO_ResetBits(GPIOA,GPIO_Pin_4)		//命令发送使能
#define RST_HIGH()  GPIO_SetBits(GPIOA,GPIO_Pin_8)		  //复位引脚高
#define RST_LOW()   GPIO_ResetBits(GPIOA,GPIO_Pin_8)		 //复位引脚低
//发送一个数据到OLED的RAM缓冲区
void oled_send_data(unsigned char c)
{
   unsigned char x=c;
   OLED_DATA();
   SPI1_RWByte(x);

}
//发送一个命令到OLED
void oled_send_cmd(unsigned char c)
{
    unsigned char x=c;
	OLED_CMD();
	SPI1_RWByte(x);
}
//确定显示位置，page是行，范围0-3,，colum是列，范围0-127
void oled_address(unsigned char page,unsigned char column)
{
	column=column+2;  		//根据实际情况测试出来的初始值加上偏移					
	page=page+4;			 //根据实际情况测试出来的初始值加上偏移
	oled_send_cmd(0xb0+page);   			
	oled_send_cmd(((column>>4)&0x0f)+0x10);	
	oled_send_cmd(column&0x0f);				
}
//清屏
void clear_screen()
{
	unsigned char i,j;
 	for(j=0;j<4;j++)
	{
		oled_address(j,0);
		for(i=0;i<128;i++)
		{
			oled_send_data(0x00);
		}
	}
}
//清空某一行
void clear_page(unsigned char page)
{

   unsigned char i;
   oled_address(page,0);
   for(i=0;i<128;i++)
     {
	   oled_send_data(0x00);
	 }
}

//初始化OLED模块
olp;void initial_oled()
{
  	RST_LOW();        	//低电平复位
  	delay(100);
  	RST_HIGH();		   	//复位完毕
  	delay(200); 
	       
	oled_send_cmd(0xae);	//关显示

	oled_send_cmd(0xd5);	//晶振频率
	oled_send_cmd(0x80);

	oled_send_cmd(0xa8);	//duty设置
	oled_send_cmd(0x3f);	//duty=1/64

	oled_send_cmd(0xd3);	//显示偏移
	oled_send_cmd(0x00);	

	oled_send_cmd(0x40);	//起始行

	oled_send_cmd(0x8d);	//升压允许
	oled_send_cmd(0x14);	

	oled_send_cmd(0x20);	//page address mode
	oled_send_cmd(0x02);

	oled_send_cmd(0xc8); //行扫描顺序：从上到下	//c1
	oled_send_cmd(0xa1); //列扫描顺序：从左到右	//a0

	oled_send_cmd(0xda); //sequential configuration
	oled_send_cmd(0x12);


	oled_send_cmd(0x81); //微调对比度,本指令的0x81不要改动，改下面的值
	oled_send_cmd(0xcf); //微调对比度的值，可设置范围0x00～0xff


	oled_send_cmd(0xd9); //Set Pre-Charge Period
	oled_send_cmd(0xf1);

	oled_send_cmd(0xdb); //Set VCOMH Deselect Level
	oled_send_cmd(0x40);

	oled_send_cmd(0xaf); //开显示
	clear_screen();
}




//在屏幕上一个固定位置显示一个字符串，长度在18个字符以内
void oled_show_str(unsigned char *c)
{  
   unsigned char i;
   unsigned char *temp=c;
   clear_page(1);
   oled_address(1,6);
   while(*temp!='\0')
     {
	   for(i=0;i<6;i++)
	      oled_send_data(F6x8[*temp-0x20][i]);
	   temp++;
	      
	 }
   
}

void show_voltage()
{
    unsigned short vol;
	unsigned char i,x,y,z;
	vol=get_voltage();
	x=vol%10;
	y=(vol/10)%10;
	z=vol/100;
	clear_page(0);
    oled_address(0,6);
    for(i=0;i<6;i++)
	      oled_send_data(F6x8[z+16][i]);
	for(i=0;i<6;i++)
	      oled_send_data(F6x8[14][i]);
	for(i=0;i<6;i++)
	      oled_send_data(F6x8[y+16][i]);
	for(i=0;i<6;i++)
	      oled_send_data(F6x8[x+16][i]);
	for(i=0;i<6;i++)
	      oled_send_data(F6x8[27][i]);
}
  
