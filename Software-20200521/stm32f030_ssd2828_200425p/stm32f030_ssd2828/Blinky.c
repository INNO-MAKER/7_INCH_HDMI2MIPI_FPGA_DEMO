/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2015 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>

//#include "Board_LED.h"                  // ::Board Support:LED
//#include "Board_Buttons.h"              // ::Board Support:Buttons

#include "stm32f0xx.h"                  // Device header

#include "Serial.h"
#include "perh.h"
#include "gpio.h"

#include "SpiFF.h" 
#include "spi.h"

extern int  ssd2828_init(void);
extern void fpga_reset(void);


//#define __DEBUG__ 

#ifdef __DEBUG__  
//#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)  
#define DEBUG(format,...) printf(""format"",##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif 

volatile uint32_t msTicks;                                 // counts 1ms timeTicks
/*----------------------------------------------------------------------------
 * SysTick_Handler:
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
  msTicks++;
}

/*----------------------------------------------------------------------------
 * Delay: delays a number of Systicks
 *----------------------------------------------------------------------------*/
void Delay (uint32_t dlyTicks) {
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) { __NOP(); }
}

/*----------------------------------------------------------------------------
 * SystemCoreClockConfigure: configure SystemCoreClock using HSI
                             (HSE is not populated on Nucleo board)
 *----------------------------------------------------------------------------*/
void SystemCoreClockConfigure(void) {

  RCC->CR |= ((uint32_t)RCC_CR_HSION);                     // Enable HSI
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  // Wait for HSI Ready

  RCC->CFGR = RCC_CFGR_SW_HSI;                             // HSI is system clock
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  // Wait for HSI used as system clock

  FLASH->ACR  = FLASH_ACR_PRFTBE;                          // Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_LATENCY;                         // Flash 1 wait state

  RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         // HCLK = SYSCLK
  RCC->CFGR |= RCC_CFGR_PPRE_DIV1;                         // PCLK = HCLK

  RCC->CR &= ~RCC_CR_PLLON;                                // Disable PLL
/*
  //  PLL configuration:  = HSI/2 * 12 = 48 MHz
  RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
  RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL12);

  RCC->CR |= RCC_CR_PLLON;                                 // Enable PLL
  while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           // Wait till PLL is ready

  RCC->CFGR &= ~RCC_CFGR_SW;                               // Select PLL as system clock source
  RCC->CFGR |=  RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  // Wait till PLL is system clock src
*/
}

#define FRST_CH_8MHz	(2414 - 250)
#define FRST_CH_6MHz	(2424 - 250)

/*----------------------------------------------------------------------------
 * main: blink LED and check button state
 *----------------------------------------------------------------------------*/

int main (void) {
	int ret;
	//unsigned int mode;
	int rx_ch;
	int i;
	uint8_t gpio_val;
	uint32_t rd_ret;
	
	unsigned int mode;
	u8 sf_id[20];
	
  SystemCoreClockConfigure();                              // configure HSI as System Clock
  SystemCoreClockUpdate();

  //LED_Initialize();
  //Buttons_Initialize();
  SER_Initialize();

  SysTick_Config(SystemCoreClock / 1000);                  // SysTick 1 msec interrupts
//while(1) {	
	SER_PutChar('+');
	bitbangSPI_Initialize ();
	//M25P80_Init();
	
	//wait fpga config ok
	/*set_gpio_value(0, MPROGRAM_B, 0);
	DelayMs(30);
	set_gpio_value(0, MPROGRAM_B, 1);
  DelayMs(2000);*/
	DelayMs(50);
	//while(1)
	printf("STM32 FPGA MIPI rotate 90 degree board.\r\n");
	ssd2828_init();
	fpga_reset();
	do{
		ret = get_gpio_value(1, FPGA_INT, &gpio_val);
		if(gpio_val) {
			fpga_reset();
			printf("X...\r\n");
		}	
		DelayMs(1000);
	}
	while(1);
	

}
