/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): possible defines select the used communication interface:
 *                        - USART2 interface  (default)
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm32f0xx.h"                  // Device header
#include "Serial.h"
#include "gpio.h"
#include "stdio.h"

/*----------------------------------------------------------------------------
 Define  USART
 *----------------------------------------------------------------------------*/
#define USARTx  USART1


/*----------------------------------------------------------------------------
 Define  Baudrate setting (BRR) for USART
 *----------------------------------------------------------------------------*/
#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

int CR1_ctrl;

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void SER_Initialize (void) {

	/* Configure PA3 to USART2_RX, PA2 to USART2_TX */
	RCC->AHBENR  |=  (   1ul << 17);         /* Enable GPIOA clock              */
  RCC->AHBENR  |=  (   1ul << 18);         /* Enable GPIOB clock              */
  //RCC->APB1ENR |=  (   1ul << 17);         /* Enable USART#2 clock            */
	RCC->APB2ENR |=  (   1ul << 14);         /* Enable USART#1 clock            */
	//RCC->APB1ENR |=  (   1ul << 20);         /* Enable USART#5 clock            */
	
#if 0
  /* Configure PA3 to USART2_RX, PA2 to USART2_TX */
  GPIOA->AFR[0] &= ~((15ul << 4* 2) | (15ul << 4* 3));
  GPIOA->AFR[0] |=  (( 1ul << 4* 2) | ( 1ul << 4* 3));
  GPIOA->MODER  &= ~(( 3ul << 2* 2) | ( 3ul << 2* 3));
  GPIOA->MODER  |=  (( 2ul << 2* 2) | ( 2ul << 2* 3));
#endif	
	/* Configure PA10 to USART1_RX, PA9 to USART1_TX */
  GPIOA->AFR[1] &= ~((15ul << 4* 2) | (15ul << 4* 1));
  GPIOA->AFR[1] |=  (( 1ul << 4* 2) | ( 1ul << 4* 1));
  GPIOA->MODER  &= ~(( 3ul << 2* 10) | ( 3ul << 2* 9));
  GPIOA->MODER  |=  (( 2ul << 2* 10) | ( 2ul << 2* 9));

  USARTx->BRR  = __USART_BRR(8000000ul, 9600ul);  /* 115200 baud @ 48MHz   */
  USARTx->CR3   = 0x0000;                  /* no flow control                 */
  USARTx->CR2   = 0x0000;                  /* 1 stop bit                      */
#if 0	
  USARTx->CR1   = ((   1ul <<  2) |        /* enable RX                       */
                   (   1ul <<  3) |        /* enable TX                       */
                   (   0ul << 12) |        /* 1 start bit, 8 data bits        */
                   (   1ul <<  0) );       /* enable USART                    */
#endif	
	CR1_ctrl =       (       /* enable RX                       */
                   (   1ul <<  3) |        /* enable TX                       */
                   (   0ul << 12) |        /* 1 start bit, 8 data bits        */
                   (   1ul <<  0) );       /* enable USART                    */
	
	USARTx->CR1 = CR1_ctrl;
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_PutChar (int ch) {

  while (!(USARTx->ISR & (1ul << 7)));
	//if((USARTx->ISR & (1ul << 7)))
		USARTx->TDR = (ch & 0x1FF);
	/*else {
		DelayMs(1);
		if((USARTx->ISR & (1ul << 7)))
			USARTx->TDR = (ch & 0x1FF);
	}*/
		
  return (ch);
}

/*----------------------------------------------------------------------------
  Read character from Serial Port
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) {
	volatile int rxc;
	//static int i;
  if (USARTx->ISR & 0x0020)
    return (USARTx->RDR);
  
	rxc = USARTx->ISR;
	//rxc &= 0x0f;
	USARTx->ICR = rxc;
	/*if(!(i%10000))
		printf("%08x\r\n", rxc);
	i++;*/
	USARTx->CR1 = CR1_ctrl | (   1ul <<  2);
  return (-1);
}
