#ifndef _SPI_H_
#define _SPI_H_

void SPI1_Init(void);
void SPI1_SendByte(unsigned char dat);
unsigned char SPI1_RWByte(unsigned char dat);

#endif
