#include <stdint.h>
#include <stdio.h>
#include "stm32f0xx.h"


void set_gpio_value(uint8_t band, uint8_t pin, uint8_t val)
{
	GPIO_TypeDef *gpio = (band==0) ? GPIOA : 
											 (band==1) ? GPIOB : GPIOC;
	if(val)
		gpio->BSRR |= 1<<pin;
	else
		gpio->BSRR |= 1<<(pin+16);
}
	
uint8_t get_gpio_value(uint8_t band, uint8_t pin, uint8_t *val)
{
	GPIO_TypeDef *gpio = (band==0) ? GPIOA : 
											 (band==1) ? GPIOB : GPIOC;
	if (gpio->IDR & (1ul << pin)) {
    *val = 1;
  }
	else
		*val = 0;
	return *val;
}

void DelayMs(int ms)
{
	volatile int i;
	for(; ms>0; ms--)
		//for(i=3846; i>0; i--); //debug
		for(i=813; i>0; i--); //release;
}
