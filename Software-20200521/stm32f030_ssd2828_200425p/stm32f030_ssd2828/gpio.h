#ifndef __GPIO_H
#define __GPIO_H


#include <stdint.h>
#define BANKA					(0)
#define BANKB					(1)
#define BANKC					(2)

//band A
#define GPIO_PANEL_RESET		(2)
#define GPIO_PANEL_VEN			(1)
#define GPIO_SSD2828_RESET		(8)
#define MPROGRAM_B		 		(3)
#define FPGA_CS0				(4)
#define FPGA_SCK				(5)
#define FPGA_SDO				(6)
#define FPGA_SDI				(7)
#define FPGA_CS1				(15)
//band B
#define SSD2828_CS				(12)
#define SSD2828_SCK				(13)
#define SSD2828_SDI				(14) //input
#define SSD2828_SDO				(15) //output
#define	FPGA_GLB_RST			(1)
#define FPGA_INT          (0)


//band C
//#define PHY_RST	(13)

void set_gpio_value(uint8_t band, uint8_t pin, uint8_t val);
uint8_t get_gpio_value(uint8_t band, uint8_t pin, uint8_t *val);
uint8_t get_dip_val(void);
void DelayMs(int ms);

void panel_vs_en(uint32_t en);

#endif
