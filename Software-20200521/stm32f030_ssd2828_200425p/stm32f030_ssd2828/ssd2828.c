/*
 * ADS7846 based touchscreen and sensor driver
 *
 * Copyright (c) 2005 David Brownell
 * Copyright (c) 2006 Nokia Corporation
 * Various changes: Imre Deak <imre.deak@nokia.com>
 *
 * Using code from:
 *  - corgi_ts.c
 *	Copyright (C) 2004-2005 Richard Purdie
 *  - omap_ts.[hc], ads7846.h, ts_osk.c
 *	Copyright (C) 2002 MontaVista Software
 *	Copyright (C) 2004 Texas Instruments
 *	Copyright (C) 2005 Dirk Behme
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef uint16_t 	u16	;
typedef uint8_t		u8	;

uint8_t dummy_gpio_val;

#define SSD_SPI_SCLK_H  	set_gpio_value(BANKB, SSD2828_SCK, 1)
#define SSD_SPI_SCLK_L   	set_gpio_value(BANKB, SSD2828_SCK, 0)

#define SSD_SPI_SDO_H   	set_gpio_value(BANKB, SSD2828_SDO, 1)
#define SSD_SPI_SDO_L   	set_gpio_value(BANKB, SSD2828_SDO, 0)

#define SSD_SPI_CS_H   	 	set_gpio_value(BANKB, SSD2828_CS, 1)
#define SSD_SPI_CS_L    	set_gpio_value(BANKB, SSD2828_CS, 0)
	  			

#define SSD_RST_H   		set_gpio_value(BANKA, GPIO_SSD2828_RESET, 1)
#define SSD_RST_L   		set_gpio_value(BANKA, GPIO_SSD2828_RESET, 0)

#define LCD_RST_H   		set_gpio_value(BANKA, GPIO_PANEL_RESET, 0)
#define LCD_RST_L   		set_gpio_value(BANKA, GPIO_PANEL_RESET, 1)

#define SSD_SPI_SDI   		get_gpio_value(BANKB, SSD2828_SDI, &dummy_gpio_val)

#include "ssd2828.h"

void SSD2828_Init_io(void)
{
panel_vs_en(1);
SSD_RST_L;
LCD_RST_L;
SSD_SPI_CS_H;
SSD_SPI_SCLK_L;
SSD_SPI_SDO_L;
DIS_Delayms(120);
SSD_RST_H;
LCD_RST_H;
DIS_Delayms(120);
}

int  ssd2828_init(void)
{
	uint16_t ssd2828_id;
	SSD2828_Init_io();
	ssd2828_id = SSD2828_READ_Dat(0xB0);
	printf("ssd2828 id %x\r\n",ssd2828_id);

	MIPI_7inch_1200x1920_LCD_Init();

	//gpio_set_value(pdata->LCD_BK_EN,0);
	return 0;
}

