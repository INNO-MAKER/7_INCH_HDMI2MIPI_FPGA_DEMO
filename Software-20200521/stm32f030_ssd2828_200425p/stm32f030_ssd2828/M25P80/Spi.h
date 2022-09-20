#ifndef _SPI_H
#define _SPI_H

#define SPI_ReadByte(SPIx)	SPI_WriteByte(SPIx,0)
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 byte);
void SPI1_Init(void);

#endif
