#include "stm32f10x.h"
#include "lib.h"
#include "spi.h"
#include "delay.h"
#include "adc.h"


#define OLED_DATA()   GPIO_SetBits(GPIOA,GPIO_Pin_4)  //���ݷ���ʹ��
#define OLED_CMD()    GPIO_ResetBits(GPIOA,GPIO_Pin_4)		//�����ʹ��
#define RST_HIGH()  GPIO_SetBits(GPIOA,GPIO_Pin_8)		  //��λ���Ÿ�
#define RST_LOW()   GPIO_ResetBits(GPIOA,GPIO_Pin_8)		 //��λ���ŵ�
//����һ�����ݵ�OLED��RAM������
void oled_send_data(unsigned char c)
{
   unsigned char x=c;
   OLED_DATA();
   SPI1_RWByte(x);

}
//����һ�����OLED
void oled_send_cmd(unsigned char c)
{
    unsigned char x=c;
	OLED_CMD();
	SPI1_RWByte(x);
}
//ȷ����ʾλ�ã�page���У���Χ0-3,��colum���У���Χ0-127
void oled_address(unsigned char page,unsigned char column)
{
	column=column+2;  		//����ʵ��������Գ����ĳ�ʼֵ����ƫ��					
	page=page+4;			 //����ʵ��������Գ����ĳ�ʼֵ����ƫ��
	oled_send_cmd(0xb0+page);   			
	oled_send_cmd(((column>>4)&0x0f)+0x10);	
	oled_send_cmd(column&0x0f);				
}
//����
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
//���ĳһ��
void clear_page(unsigned char page)
{

   unsigned char i;
   oled_address(page,0);
   for(i=0;i<128;i++)
     {
	   oled_send_data(0x00);
	 }
}

//��ʼ��OLEDģ��
olp;void initial_oled()
{
  	RST_LOW();        	//�͵�ƽ��λ
  	delay(100);
  	RST_HIGH();		   	//��λ���
  	delay(200); 
	       
	oled_send_cmd(0xae);	//����ʾ

	oled_send_cmd(0xd5);	//����Ƶ��
	oled_send_cmd(0x80);

	oled_send_cmd(0xa8);	//duty����
	oled_send_cmd(0x3f);	//duty=1/64

	oled_send_cmd(0xd3);	//��ʾƫ��
	oled_send_cmd(0x00);	

	oled_send_cmd(0x40);	//��ʼ��

	oled_send_cmd(0x8d);	//��ѹ����
	oled_send_cmd(0x14);	

	oled_send_cmd(0x20);	//page address mode
	oled_send_cmd(0x02);

	oled_send_cmd(0xc8); //��ɨ��˳�򣺴��ϵ���	//c1
	oled_send_cmd(0xa1); //��ɨ��˳�򣺴�����	//a0

	oled_send_cmd(0xda); //sequential configuration
	oled_send_cmd(0x12);


	oled_send_cmd(0x81); //΢���Աȶ�,��ָ���0x81��Ҫ�Ķ����������ֵ
	oled_send_cmd(0xcf); //΢���Աȶȵ�ֵ�������÷�Χ0x00��0xff


	oled_send_cmd(0xd9); //Set Pre-Charge Period
	oled_send_cmd(0xf1);

	oled_send_cmd(0xdb); //Set VCOMH Deselect Level
	oled_send_cmd(0x40);

	oled_send_cmd(0xaf); //����ʾ
	clear_screen();
}




//����Ļ��һ���̶�λ����ʾһ���ַ�����������18���ַ�����
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
  
