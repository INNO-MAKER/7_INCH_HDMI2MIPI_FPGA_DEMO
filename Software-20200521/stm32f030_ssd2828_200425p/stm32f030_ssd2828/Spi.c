#include "Spi.h"       
#include "stdint.h"
#include "gpio.h"
#include "perh.h"

u8 SPI_WriteByte(u8 Byte)
{
    u8 gpio_value;
	  int bit_count;
		u8 miso;
    // Number of bytes we are writing.
    //uint8_t num_bytes = 3;  //register address = 2 bytes, data = 1 byte

    //set_gpio_value(1, GPIO_FX3_CE2, 0);

		
				miso = 0x0;
        for(bit_count = 7; bit_count >= 0; bit_count--) {
						set_gpio_value(BANKA, FPGA_SDO, ((Byte >> bit_count) & 0x01));
            set_gpio_value(BANKA, FPGA_SCK, 1);
            //CyU3PGpioSetValue(GPIO_FX3_MOSI, ((mode >> bit_count) & 0x01));
					  get_gpio_value(BANKA, FPGA_SDI, &gpio_value);
            set_gpio_value(BANKA, FPGA_SCK, 0);

            //CyU3PGpioGetValue(GPIO_FX3_MISO, &gpio_value);
            if(gpio_value) {
                miso |= (1 << bit_count);
            }
        }
        // FIXME: Determine what to do with miso value;

    set_gpio_value(BANKA, FPGA_SDO, 0);
    //set_gpio_value(1, GPIO_FX3_CE2, 1);
		
		return miso;
}	


u32 SPI_WriteWord(u32 Word)
{
    u8 gpio_value;
	  int bit_count;
		u32 miso;
    // Number of bytes we are writing.
    //uint8_t num_bytes = 3;  //register address = 2 bytes, data = 1 byte

    //set_gpio_value(1, GPIO_FX3_CE2, 0);

		
				miso = 0x0;
        for(bit_count = 31; bit_count >= 0; bit_count--) {
						set_gpio_value(BANKA, FPGA_SDO, ((Word >> bit_count) & 0x01));
            set_gpio_value(BANKA, FPGA_SCK, 1);
            //CyU3PGpioSetValue(GPIO_FX3_MOSI, ((mode >> bit_count) & 0x01));
					  get_gpio_value(BANKA, FPGA_SDI, &gpio_value);
            set_gpio_value(BANKA, FPGA_SCK, 0);

            //CyU3PGpioGetValue(GPIO_FX3_MISO, &gpio_value);
            if(gpio_value) {
                miso |= (1 << bit_count);
            }
        }
        // FIXME: Determine what to do with miso value;

    set_gpio_value(BANKA, FPGA_SDO, 0);
    //set_gpio_value(1, GPIO_FX3_CE2, 1);
		
		return miso;
}	
