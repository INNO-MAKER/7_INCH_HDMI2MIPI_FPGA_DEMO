#ifndef _SPI_H
#define _SPI_H

#include "stdint.h"

#define SPI_ReadByte()	SPI_WriteByte(0)
u8 SPI_WriteByte(u8 byte);

#define SPI_ReadWord()	SPI_WriteWord(0)
u32 SPI_WriteWord(u32 Word);
void SPI1_Init(void);

#endif
