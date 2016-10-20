#ifndef _USART_H_
#define _USART_H_
#include "stm32f10x.h"

						 
void USART1_Config(void) ;
void USART2_Config(void);
unsigned char getc(USART_TypeDef* USARTx);
void putc(USART_TypeDef* USARTx,unsigned char c);
void putstr(USART_TypeDef* USARTx,unsigned char *c);
void puthexunit(USART_TypeDef* USARTx,unsigned char c);
#endif 
