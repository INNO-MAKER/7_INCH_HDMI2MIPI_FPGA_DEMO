/*
 * FileName:       
 * Author:     909220579qq.com      
 * Date: 2013-11-10
 * Description:  M25P80 ��������
 * Version:        
 * Function List:ʹ��stm32�ϵ�spi��flash m25p80�����ݶ�д�������������Ӳ�����Ӽ���  
 *      
 * History:            
 */
#include "Spi.h"
#include "SpiFF.h" 


#define SPI_Flash_Read() SPI_ReadByte()
#define	SPI_Flash_Write(x) SPI_WriteByte(x)


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

//�����ݣ��Զ���ҳ,addrΪ�ֽڵ�ַ
void M25P80_Read_Data(u32 addr,u32 len,u8 *buf)
{
	u32 i;	
	Select_Flash();	
	SPI_Flash_Write(READ_DATA);	
	SPI_Flash_Write((addr>>16) & 0xff);//������16��
	SPI_Flash_Write((addr>>8) & 0xff); //ҳ��256ҳ
	SPI_Flash_Write(addr & 0xff);//ҳ�ڵ�ַ��256�ֽ�
	
	for(i=0;i<len;i++)
	{
	  buf[i]=SPI_Flash_Read();
	}
	NotSelect_Flash();
}

//���ٶ�����
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

//ҳ��̺�����ҳ���ǰһ��Ҫ����������!
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
	NotSelect_Flash();
}
