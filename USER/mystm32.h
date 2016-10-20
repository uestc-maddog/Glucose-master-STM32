#ifndef _MYSTM32_H_
#define _MYSTM32_H_

#define uchar unsigned char 

/*
1位和0位：
        00:输入模式
	01：输出模式，10MHZ
	10：输出模式，2MHZ
	11：输出模式，50MHZ
3位和2位：
	输入模式下：
		00：模拟输入模式
		01：浮空输入模式
		10：上拉/下拉输入模式
	输出模式下：
		00：通用推挽输出模式
		01：通用开漏输出模式
		10：复用功能推挽输出模式
		11：复用功能开漏输出模式 	*/

//GPIO


#define mGPIO_BASE  0x40010800
#define mGPACRL   (*(volatile unsigned long *)(mGPIO_BASE+0x00))
#define mGPACRH   (*(volatile unsigned long *)(mGPIO_BASE+0x04))
#define mGPAIDR   (*(volatile unsigned short *)(mGPIO_BASE+0x08))
#define mGPAODR   (*(volatile unsigned short *)(mGPIO_BASE+0x0c))
#define mGPABSRR  (*(volatile unsigned long *)(mGPIO_BASE+0x10))
#define mGPABRR   (*(volatile unsigned short *)(mGPIO_BASE+0x14))
#define mGPALOCK  (*(volatile unsigned long   *)(mGPIO_BASE+0x18))

/*
RCC_AHBENR:
	16:以太网MAC接收
	15:以太网MAC发送
	14:以太网MAC
	12:全速USB OTG时钟
	 6:CRC
	 4:睡眠模式时闪存接口电路时钟
	 2:睡眠模式时SRAM时钟
	 1:DMA2
	 0:DMA1


RCC_APB2ENR:
	0关闭  1开启

        14:USART1
	12:SPI1
	11:TIM1
	10:ADC2
	9:ADC1
	6:GPIO E
	5:GPIO D
	4:GPIO C
	3:GPIO B
	2:GPIO A
	0:辅助功能IO时钟

RCC_APB1ENR:
	29:DAC
	28:电源接口时钟
	27:备份接口时钟
	26:CAN2
	25:CAN1
	22:I2C 2
	21:I2C 1
	20:UART5
	19:UART4
	18:USART3
	17:USART2
	15:SPI3
	14:SPI2
	11:窗口看门狗时钟
	 5:TIM7
	 4:TIM6
	 3:TIM5
	 2:TIM4
	 1:TIM3
	 0:TIM2
	 */
//RCC
#define mRCC_BASE  0x40021000

#define mRCC_CR    (*(volatile unsigned long   *)(mRCC_BASE + 0x00))
#define mRCC_CFGR  (*(volatile unsigned long   *)(mRCC_BASE + 0x04))
#define mRCC_CIR   (*(volatile unsigned long   *)(mRCC_BASE + 0x08))
#define mRCC_APB2RSTR  (*(volatile unsigned long   *)(mRCC_BASE + 0x0C))
#define mRCC_APB1RSTR  (*(volatile unsigned long   *)(mRCC_BASE + 0x10))
#define mRCC_AHBENR    (*(volatile unsigned long   *)(mRCC_BASE + 0x14))
#define mRCC_APB2ENR    (*(volatile unsigned long   *)(mRCC_BASE + 0x18))
#define mRCC_APB1ENR    (*(volatile unsigned long   *)(mRCC_BASE + 0x1C))
#define mRCC_BDCR    (*(volatile unsigned long   *)(mRCC_BASE + 0x20))
#define mRCC_CSR      (*(volatile unsigned long   *)(mRCC_BASE + 0x24))
//flash
#define mFLASH_BASE    0x40022000

#define mFLASH_ACR  (*(volatile unsigned long *)(mFLASH_BASE + 0x00))
#define mFLASH_KEYR  (*(volatile unsigned long *)(mFLASH_BASE + 0x04))
#define mFLASH_OPTKEYR  (*(volatile unsigned long *)(mFLASH_BASE + 0x08))
#define mFLASH_SR  (*(volatile unsigned long *)(mFLASH_BASE + 0x0C))
#define mFLASH_CR  (*(volatile unsigned long *)(mFLASH_BASE + 0x10))
#define mFLASH_AR  (*(volatile unsigned long *)(mFLASH_BASE + 0x14))
#define mFLASH_OBR  (*(volatile unsigned long *)(mFLASH_BASE + 0x18))
#define mFLASH_WRPR  (*(volatile unsigned long *)(mFLASH_BASE + 0x1C))
//system tick

#define mSTK_BASE   0xE000E010
#define mSTK_CTRL   (*(volatile unsigned long *)(mSTK_BASE + 0x00))
#define mSTK_LOAD   (*(volatile unsigned long *)(mSTK_BASE + 0x04))
#define mSTK_VAL    (*(volatile unsigned long *)(mSTK_BASE + 0x08))
#define mSTK_CALIB  (*(volatile unsigned long *)(mSTK_BASE + 0x0c))
//general timer
#define mTIM3_BASE  0x40000400


#define mTIM3_CR1   (*(volatile unsigned long *)(mTIM3_BASE + 0x00))
#define mTIM3_CR2   (*(volatile unsigned long *)(mTIM3_BASE + 0x04))
#define mTIM3_SMCR   (*(volatile unsigned long *)(mTIM3_BASE + 0x08))
#define mTIM3_DIER   (*(volatile unsigned long *)(mTIM3_BASE + 0x0C))   
#define mTIM3_SR   (*(volatile unsigned long *)(mTIM3_BASE + 0x10))
#define mTIM3_EGR   (*(volatile unsigned long *)(mTIM3_BASE + 0x14))
#define mTIM3_CCMR1   (*(volatile unsigned long *)(mTIM3_BASE + 0x18))
#define mTIM3_CCMR2 (*(volatile unsigned long *)(mTIM3_BASE + 0x1C))
#define mTIM3_CCER   (*(volatile unsigned long *)(mTIM3_BASE + 0x20))
#define mTIM3_CNT   (*(volatile unsigned long *)(mTIM3_BASE + 0x24))
#define mTIM3_PSC   (*(volatile unsigned long *)(mTIM3_BASE + 0x28))
#define mTIM3_ARR   (*(volatile unsigned long *)(mTIM3_BASE + 0x2C))
#define mTIM3_CCR1   (*(volatile unsigned long *)(mTIM3_BASE + 0x34))
#define mTIM3_CCR2   (*(volatile unsigned long *)(mTIM3_BASE + 0x38))
#define mTIM3_CCR3   (*(volatile unsigned long *)(mTIM3_BASE + 0x3C))
#define mTIM3_CCR4   (*(volatile unsigned long *)(mTIM3_BASE + 0x40))
#define mTIM3_DCR   (*(volatile unsigned long *)(mTIM3_BASE + 0x48))
#define mTIM3_DMAR   (*(volatile unsigned long *)(mTIM3_BASE + 0x4C))


#define mTIM6_BASE    0x40001000

#define mTIM6_CR1   (*(volatile unsigned long *)(mTIM6_BASE + 0x00))
#define mTIM6_CR2   (*(volatile unsigned long *)(mTIM6_BASE + 0x04))
#define mTIM6_DIER	(*(volatile unsigned long *)(mTIM6_BASE + 0x08))
#define mTIM6_SR   (*(volatile unsigned long *)(mTIM6_BASE + 0x10))
#define mTIM6_EGR   (*(volatile unsigned long *)(mTIM6_BASE + 0x14))   
#define mTIM6_CNT   (*(volatile unsigned long *)(mTIM6_BASE + 0x24))
#define mTIM6_PSC   (*(volatile unsigned long *)(mTIM6_BASE + 0x28))
#define mTIM6_ARR   (*(volatile unsigned long *)(mTIM6_BASE + 0x2C))

//SCB
#define mSCB_BASE    0xE000ED00

#define mSCB_CPUID   (*(volatile unsigned long *)(mSCB_BASE + 0x00))
#define mSCB_ICSR   (*(volatile unsigned long *)(mSCB_BASE + 0x04))
#define mSCB_VTOR   (*(volatile unsigned long *)(mSCB_BASE + 0x08))
#define mSCB_AIRCR   (*(volatile unsigned long *)(mSCB_BASE + 0x0C))
#define mSCB_SCR   (*(volatile unsigned long *)(mSCB_BASE + 0x10))
#define mSCB_CCR   (*(volatile unsigned long *)(mSCB_BASE + 0x14))
#define mSCB_SHPR1   (*(volatile unsigned long *)(mSCB_BASE + 0x18))
#define mSCB_SHPR2   (*(volatile unsigned long *)(mSCB_BASE + 0x1C))
#define mSCB_SHPR3   (*(volatile unsigned long *)(mSCB_BASE + 0x20))
#define mSCB_SHCRS   (*(volatile unsigned long *)(mSCB_BASE + 0x24))
#define mSCB_CFSR   (*(volatile unsigned long *)(mSCB_BASE + 0x28))
#define mSCB_HFSR   (*(volatile unsigned long *)(mSCB_BASE + 0x2C))
#define mSCB_MMAR   (*(volatile unsigned long *)(mSCB_BASE + 0x34))
#define mSCB_BFAR   (*(volatile unsigned long *)(mSCB_BASE + 0x38))
 //interrupt number
#define WWDG_IRQn        0
#define PVD_IRQn		 1
#define TAMPER_IRQn		 2
#define RTC_IRQn		 3
#define Flash_IRQn		 4
#define RCC_IRQn		 5
#define EXTI0_IRQn		 6
#define EXTI1_IRQn		 7
#define EXTI2_IRQn		 8
#define EXTI3_IRQn		 9
#define EXTI4_IRQn		 10
#define DMA1_1_IRQn		 11
#define DMA1_2_IRQn		 12
#define DMA1_3_IRQn		 13
#define DMA1_4_IRQn		 14
#define DMA1_5_IRQn		 15
#define DMA1_6_IRQn		 16
#define DMA1_7_IRQn		 17
#define ADC1_2_IRQn		 18
#define USB_HP_CAN_TX_IRQn	  19
#define USB_LP_CAN_RX0_IRQn	  20
#define CAN_RX1_IRQn		  21
#define CAN_SCE_IRQn		  22
#define EXTI9_5_IRQn		  23
#define TIM1_BRK_IRQn		  24
#define TIM1_UP_IRQn		  25
#define TIM1_TRG_COM_IRQn	  26
#define TIM1_CC_IRQn		  27
#define TIM2_IRQn			  28
#define TIM3_IRQn			  29
#define TIM4_IRQn			  30
#define I2C1_EV_IRQn		  31
#define I2C1_ER_IRQn		  32
#define I2C2_EV_IRQn		  33
#define I2C2_ER_IRQn		  34
#define SPI1_IRQn			  35
#define SPI2_IRQn			  36
#define USART1_IRQn			  37
#define USART2_IRQn			  38
#define USART3_IRQn			  39
#define EXTI15_10_IRQn		  40
#define RTCAlarm_IRQn		  41
#define USB_Wakeup_IRQn		  42
#define TIM8_BRK_IRQn		  43
#define TIM8_UP_IRQn		  44
#define TIM8_TRG_COM_IRQn	  45
#define TIM8_CC_IRQn		  46
#define ADC3_IRQn			  47
#define FSMC_IRQn			  48
#define SDIO_IRQn			  49
#define TIM5_IRQn			  50
#define SPI3_IRQn			  51
#define UART4_IRQn			  52
#define UART5_IRQn			  53
#define TIM6_IRQn			  54
#define TIM7_IRQn			  55
#define DMA2_1_IRQn			  56
#define DMA2_2_IRQn			  57
#define DMA2_3_IRQn			  58
#define DMA2_4_5_IRQn		  59
							  
//NVIC

#define mNVIC_BASE    0xE000E100

typedef struct
{
    volatile  unsigned long ISER[8];                 /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register           */
       unsigned long RESERVED0[24];
    volatile unsigned long ICER[8];                 /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register         */
       unsigned long RSERVED1[24];
    volatile unsigned  long ISPR[8];                 /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register          */
       unsigned long RESERVED2[24];
    volatile unsigned long ICPR[8];                 /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register        */
       unsigned long RESERVED3[24];
    volatile unsigned long IABR[8];                 /*!< Offset: 0x200 (R/W)  Interrupt Active bit Register           */
       unsigned long RESERVED4[56];
    volatile unsigned char   IPR[240];                 /*!< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
       unsigned long RESERVED5[644];
    volatile unsigned long  STIR;                    /*!< Offset: 0xE00 ( /W)  Software Trigger Interrupt Register     */
}mNVIC_Type;
//#define NVIC     ((mNVIC_Type *)mNVIC_BASE)

//USART

//#define USART1_BASE   0x40013800


#define mUSART1_SR   (*(volatile unsigned long *)(USART1_BASE +0x00))
#define mUSART1_DR   (*(volatile unsigned long *)(USART1_BASE +0x04))
#define mUSART1_BRR   (*(volatile unsigned long *)(USART1_BASE +0x08))
#define mUSART1_CR1   (*(volatile unsigned long *)(USART1_BASE +0x0C))
#define mUSART1_CR2   (*(volatile unsigned long *)(USART1_BASE +0x10))
#define mUSART1_CR3   (*(volatile unsigned long *)(USART1_BASE +0x14))
#define mUSART1_GTPR   (*(volatile unsigned long *)(USART1_BASE +0x18))



#endif

