/*
 * FileName:       
 * Author:     909220579qq.com      
 * Date: 2013-11-10
 * Description:  M25P80 驱动程序
 * Version:        
 * Function List:使用stm32上的spi与flash m25p80的数据读写，具体情况更改硬件连接即可  
 *      
 * History:            
 */
#include "stm32f10x.h" //加入ST标准外设库
#include "Spi.h"
#include "SpiFF.h" 


#define SPI_Flash_Read() SPI_ReadByte(SPI1)
#define	SPI_Flash_Write(x) SPI_WriteByte(SPI1,x)


void M25P80_Write_Enable(void)
{
	Select_Flash();	
	SPI_Flash_Write(WRITE_ENABLE);	
	NotSelect_Flash();
}

void M25P80_Write_Disable(void)
{
	Select_Flash();	
	SPI_Flash_Write(WRITE_DISABLE);	
	NotSelect_Flash();
}

void M25P80_Read_Id(u8 * id)
{
	u8 i;	
	Select_Flash();	
	SPI_Flash_Write(READ_ID);	
	for(i=0;i<20;i++)
	{
		id[i] = SPI_Flash_Read();	
	}	
	NotSelect_Flash();
}

u8 M25P80_Read_Status_Reg(void)
{
	u8 sta;
	Select_Flash();	
	SPI_Flash_Write(READ_STAUS_REG);	
	sta= SPI_Flash_Read();	
	NotSelect_Flash();	
	return sta;
}

void M25P80_Write_Status_Reg(u8 reg)
{
	Select_Flash();	
	SPI_Flash_Write(WRITE_STAUS_REG);	
	SPI_Flash_Write(reg);
	NotSelect_Flash();
}

//读数据，自动翻页,addr为字节地址
void M25P80_Read_Data(u32 addr,u32 len,u8 *buf)
{
	u32 i;	
	Select_Flash();	
	SPI_Flash_Write(READ_DATA);	
	SPI_Flash_Write((addr>>16) & 0xff);//扇区：16个
	SPI_Flash_Write((addr>>8) & 0xff); //页：256页
	SPI_Flash_Write(addr & 0xff);//页内地址：256字节
	
	for(i=0;i<len;i++)
	{
	  buf[i]=SPI_Flash_Read();
	}
	NotSelect_Flash();
}

//快速读数据
void M25P80_Fast_Read_Data(u32 addr,u32 len,u8 *buf)
{
	u32 i;

	Select_Flash();	
	SPI_Flash_Write(FAST_READ_DATA);	
	SPI_Flash_Write((addr>>16) & 0xff);
	SPI_Flash_Write((addr>>8) & 0xff);
	SPI_Flash_Write(addr & 0xff);
	SPI_Flash_Write(0);
	
	for(i=0;i<len;i++)
	{
		buf[i]=SPI_Flash_Read();
	}
	NotSelect_Flash();
}

//页编程函数，页编程前一定要进行区擦除!
void M25P80_Page_Program(u32 addr,u16 len,u8 *buf)
{
	u32 i;	
	M25P80_Write_Enable();
	Select_Flash();	
	SPI_Flash_Write(PAGE_PROGRAM);	
	SPI_Flash_Write((addr>>16) & 0xff);
	SPI_Flash_Write((addr>>8) & 0xff);
	SPI_Flash_Write(addr & 0xff);

	for(i=0;i<len;i++)
		SPI_Flash_Write(buf[i]);
	NotSelect_Flash();
	while(M25P80_Read_Status_Reg()&0x01);
    //NotSelect_Flash();	
}

void M25P80_Sector_Erase(u32 addr)
{
	M25P80_Write_Enable();
	Select_Flash();	
	SPI_Flash_Write(SECTOR_ERASE);	
	SPI_Flash_Write((addr>>16) & 0xff);
	SPI_Flash_Write((addr>>8) & 0xff);
	SPI_Flash_Write(addr & 0xff);
	NotSelect_Flash();
	while(M25P80_Read_Status_Reg()&0x01);
	//NotSelect_Flash();
}

void M25P80_Bulk_Erase(void)
{
	M25P80_Write_Enable();
	Select_Flash();	
	SPI_Flash_Write(BULK_ERASE);	
	NotSelect_Flash();
	while(M25P80_Read_Status_Reg()&0x01);
	//NotSelect_Flash();
}

void M25P80_Deep_Power_Down(void)
{
	Select_Flash();	
	SPI_Flash_Write(DEEP_POWER_DOWN);	
	NotSelect_Flash();
}

u8 M25P80_Wake_Up(void)
{
	u8 res;
	Select_Flash();	
	SPI_Flash_Write(WAKE_UP);	
	SPI_Flash_Write(0);	
	SPI_Flash_Write(0);	
	SPI_Flash_Write(0);	
	res=SPI_Flash_Read();	
	NotSelect_Flash();
	return res;
}

void M25P80_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	SPI1_Init();
	GPIOC->CRH &= 0xfffffff0;
	GPIOC->CRH |= 0x3;
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//GPIO_Init(GPIOC, &GPIO_InitStructure);//配置Flash片选管脚pc8
	//不选flash
	NotSelect_Flash();
}
