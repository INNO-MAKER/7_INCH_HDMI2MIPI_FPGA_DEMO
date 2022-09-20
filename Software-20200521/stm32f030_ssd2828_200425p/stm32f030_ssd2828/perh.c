/*!
    LTC6948: Ultralow Noise and Spurious 0.37GHz to 6.39GHz FracN Synthesizer with Integrated VCO

@verbatim

The LTC?6948 is a high performance, low noise, 6.39GHz
phase-locked loop (PLL) with a fully integrated VCO,
including a reference divider, phase-frequency detector
(PFD), ultralow noise charge pump, fractional feedback
divider, and VCO output divider.

The fractional divider uses an advanced, 4th order ??
modulator which provides exceptionally low spurious
levels. This allows wide loop bandwidths, producing extremely
low integrated phase noise values.

The programmable VCO output divider, with a range of 1
through 6, extends the output frequency range.

@endverbatim


http://www.linear.com/product/LTC6948

http://www.linear.com/product/LTC6948#demoboards

REVISION HISTORY
$Revision: 3018 $
$Date: 2014-12-01 15:53:20 -0800 (Mon, 01 Dec 2014) $

Copyright (c) 2013, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.
*/

//! @defgroup LTC6948 LTC6948: Ultralow Noise and Spurious 0.37GHz to 6.39GHz FracN Synthesizer with Integrated VCO

/*! @file
    @ingroup LTC6948
    Library for LTC6948: Ultralow Noise and Spurious 0.37GHz to 6.39GHz FracN Synthesizer with Integrated VCO
*/

#include <stdint.h>
#include <stdio.h>

#include "perh.h"
#include "stm32f0xx.h"                  // Device header
#include "gpio.h"

void ssd2828_reset(void)
{
	set_gpio_value(BANKA, GPIO_SSD2828_RESET, 0);
	DelayMs(10);
	set_gpio_value(BANKA, GPIO_SSD2828_RESET, 1);
}	

void panel_reset(void)
{
	set_gpio_value(BANKA, GPIO_PANEL_RESET, 1);
	DelayMs(10);
	set_gpio_value(BANKA, GPIO_PANEL_RESET, 0);
	DelayMs(10);
}	

void panel_vs_en(uint32_t en)
{
	set_gpio_value(BANKA, GPIO_PANEL_VEN, en);
}

void fpga_reset(void)
{
	set_gpio_value(BANKB, FPGA_GLB_RST, 1);
	DelayMs(1);
	set_gpio_value(BANKB, FPGA_GLB_RST, 0);
	DelayMs(1);
}

enum DIR{INPUT = 0, OUTPUT};

void gpio_set_pinmux(int BANK, enum DIR dir, int pin)
{
	GPIO_TypeDef *h;
	h = (BANK == BANKA) ? GPIOA : 
	    (BANK == BANKB) ? GPIOB : GPIOC;
	
	h->MODER &= ~((3ul << 2*pin));
	if(dir == OUTPUT) {
      h->MODER |=  ((1ul << 2*pin));
	}	
  h->OTYPER  &= ~((1ul <<   pin));
  h->OSPEEDR &= ~((3ul << 2*pin));
  h->OSPEEDR |=  ((1ul << 2*pin));
  h->PUPDR   &= ~((3ul << 2*pin));
}

int32_t bitbangSPI_Initialize (void) 
{
  uint8_t ret;
	//uint8_t i;
	
  RCC->AHBENR |=  (1ul << 17);                  /* Enable GPIOA clock         */
	RCC->AHBENR |=  (1ul << 18);                  /* Enable GPIOB clock         */
	RCC->AHBENR |=  (1ul << 19);                  /* Enable GPIOC	clock         */

	/* Configure PA2 to RESET_MIPI#(panel), PA1 to MIPIV_EN(panel) */
  GPIOA->MODER  &= ~(( 3ul << 2* GPIO_PANEL_RESET) | ( 3ul << 2* GPIO_PANEL_VEN)); 
  GPIOA->MODER  |=  (( 1ul << 2* GPIO_PANEL_RESET) | ( 1ul << 2* GPIO_PANEL_VEN)); // PA1 & PA2 as gpio output
	GPIOA->OTYPER  &= ~((1ul << 	GPIO_PANEL_RESET		) | (1 << GPIO_PANEL_VEN));					 // push-pull mode
  GPIOA->OSPEEDR &= ~((3ul << 2*GPIO_PANEL_RESET) | (3ul << 2*GPIO_PANEL_VEN));		 // output speed slow
  GPIOA->PUPDR   &= ~((3ul << 2*GPIO_PANEL_RESET) | (3ul << 2*GPIO_PANEL_VEN));		 // pull null
	//config PA8 to SSD2828 reset
	GPIOA->MODER   &= ~((3ul << 2*GPIO_SSD2828_RESET));
  GPIOA->MODER   |=  ((1ul << 2*GPIO_SSD2828_RESET));
  GPIOA->OTYPER  &= ~((1ul <<   GPIO_SSD2828_RESET));
  GPIOA->OSPEEDR &= ~((3ul << 2*GPIO_SSD2828_RESET));
  GPIOA->OSPEEDR |=  ((1ul << 2*GPIO_SSD2828_RESET));
  GPIOA->PUPDR   &= ~((3ul << 2*GPIO_SSD2828_RESET));
	//SPI config
	gpio_set_pinmux(BANKA, OUTPUT, FPGA_CS0);
	gpio_set_pinmux(BANKA, OUTPUT, FPGA_CS1);
	gpio_set_pinmux(BANKA, OUTPUT, FPGA_SDO);
	gpio_set_pinmux(BANKA, INPUT,  FPGA_SDI);
	gpio_set_pinmux(BANKA, OUTPUT, FPGA_SCK);
	set_gpio_value(BANKA, FPGA_CS0, 1);
	set_gpio_value(BANKA, FPGA_CS1, 1);
	set_gpio_value(BANKA, FPGA_SDO, 0);
	set_gpio_value(BANKA, FPGA_SCK, 0);
	//config PB1 to fpga reset
	GPIOB->MODER   &= ~((3ul << 2*FPGA_GLB_RST));
  GPIOB->MODER   |=  ((1ul << 2*FPGA_GLB_RST));
  GPIOB->OTYPER  &= ~((1ul <<   FPGA_GLB_RST));
  GPIOB->OSPEEDR &= ~((3ul << 2*FPGA_GLB_RST));
  GPIOB->OSPEEDR |=  ((1ul << 2*FPGA_GLB_RST));
  GPIOB->PUPDR   &= ~((3ul << 2*FPGA_GLB_RST));
	//config PB12~15 to SSD2828 CS,SCK,SDO,SDI
	GPIOB->MODER   &= ~((3ul << 2*SSD2828_CS));
  GPIOB->MODER   |=  ((1ul << 2*SSD2828_CS));
  GPIOB->OTYPER  &= ~((1ul <<   SSD2828_CS));
  GPIOB->OSPEEDR &= ~((3ul << 2*SSD2828_CS));
  GPIOB->OSPEEDR |=  ((1ul << 2*SSD2828_CS));
  GPIOB->PUPDR   &= ~((3ul << 2*SSD2828_CS));
	GPIOB->MODER   &= ~((3ul << 2*SSD2828_SCK));
  GPIOB->MODER   |=  ((1ul << 2*SSD2828_SCK));
  GPIOB->OTYPER  &= ~((1ul <<   SSD2828_SCK));
  GPIOB->OSPEEDR &= ~((3ul << 2*SSD2828_SCK));
  GPIOB->OSPEEDR |=  ((1ul << 2*SSD2828_SCK));
  GPIOB->PUPDR   &= ~((3ul << 2*SSD2828_SCK));
	GPIOB->MODER   &= ~((3ul << 2*SSD2828_SDO));
  GPIOB->MODER   |=  ((1ul << 2*SSD2828_SDO));
  GPIOB->OTYPER  &= ~((1ul <<   SSD2828_SDO));
  GPIOB->OSPEEDR &= ~((3ul << 2*SSD2828_SDO));
  GPIOB->OSPEEDR |=  ((1ul << 2*SSD2828_SDO));
  GPIOB->PUPDR   &= ~((3ul << 2*SSD2828_SDO));
	GPIOB->MODER   &= ~((3ul << 2*SSD2828_SDI));
  //GPIOB->MODER   |=  ((1ul << 2*SSD2828_SDI));
  GPIOB->OTYPER  &= ~((1ul <<   SSD2828_SDI));
  GPIOB->OSPEEDR &= ~((3ul << 2*SSD2828_SDI));
  GPIOB->OSPEEDR |=  ((1ul << 2*SSD2828_SDI));
  GPIOB->PUPDR   &= ~((3ul << 2*SSD2828_SDI));
	set_gpio_value(BANKB, SSD2828_CS, 1);
	set_gpio_value(BANKB, SSD2828_SDO, 0);
	set_gpio_value(BANKB, SSD2828_SCK, 0);
/*	
	GPIOA->MODER   &= ~((3ul << 2*MPROGRAM_B));
  GPIOA->MODER   |=  ((1ul << 2*MPROGRAM_B));
  GPIOA->OTYPER  &= ~((1ul <<   MPROGRAM_B));
  GPIOA->OSPEEDR &= ~((3ul << 2*MPROGRAM_B));
  GPIOA->OSPEEDR |=  ((1ul << 2*MPROGRAM_B));
  GPIOA->PUPDR   &= ~((3ul << 2*MPROGRAM_B));
	set_gpio_value(BANKA, MPROGRAM_B, 1);
*/
	set_gpio_value(BANKB, FPGA_GLB_RST, 1);
	DelayMs(10);
	set_gpio_value(BANKB, FPGA_GLB_RST, 0);
	
	ssd2828_reset();
	panel_vs_en(1);
	DelayMs(10);
	panel_reset();
	
  return (0);
}
