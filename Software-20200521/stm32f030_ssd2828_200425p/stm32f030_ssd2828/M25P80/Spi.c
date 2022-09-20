#include "stm32f10x.h" //加入ST标准外设库
#include "stm32f10x_spi.h"
#include "Spi.h"       


//spi 写一个字节
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
{
	u8 spi_data = 0;
	while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);		//等待发送区空	  
	SPIx->DR=Byte;	 	//发送一个byte   
	while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte
	//while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);
	spi_data = SPIx->DR;  
	return spi_data;          	     //返回收到的数据			
} 
//初始化spi1
void SPI1_Init(void) //spi flash
{
  	//SPI_InitTypeDef  SPI_InitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;																			   
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	GPIOA->CRL&=0x000fffff;					 
	GPIOA->CRL|=0xbbb00000;//PA5,6,7复用推挽输出，50MHz
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);
	//RCC->APB2ENR |= 1<<12;
	//SPI_Cmd(SPI1,DISABLE);
	SPI1->CR1&=~(1<<6);//disable Spi1
	/*SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);*/
	SPI1->CR1&=~(1<<10);
	SPI1->CR1|=1<<2;
    SPI1->CR1&=~(1<<11);
	SPI1->CR1|=1<<1;
	SPI1->CR1|=1;
	SPI1->CR1|=1<<9;
	SPI1->CR1|=1<<8;
	SPI1->CR1&=~(7<<3);//清空 5：3
	SPI1->CR1|=4<<3;//比特率：000 Fpclk/2,010 Fpclk/8 011 Fpclk/16 111 Fpclk/256
	SPI1->CR1&=~(1<<7);
	//SPI_Cmd(SPI1, ENABLE);
	SPI1->CR1|=1<<6;//enable Spi1  
}
