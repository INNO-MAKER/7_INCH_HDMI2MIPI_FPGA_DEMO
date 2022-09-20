#ifndef _SSD2828_H_
#define _SSD2828_H_

#include "gpio.h"

//#define __DEBUG__
#ifdef __DEBUG__  
//#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)  
#define DEBUG(format,...) printf(""format"",##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif 


#define MIPI_1200_1920_60FPS

#ifdef MIPI_1200_1920_60FPS

#define LCD_VBPD		7 
#define LCD_VFPD	 	10 
#define LCD_VSPW		3

#define LCD_HBPD		60 
#define LCD_HFPD		60 
#define LCD_HSPW	  11

#define LCD_XSIZE_TFT    1200 
#define LCD_YSIZE_TFT    1920
#endif



void DIS_Delayms(u16 Num)
{
	DelayMs(Num);
}

void SPI_3W_SET_Cmd(u8 cmd)
{
	u8 kk;
	
	SSD_SPI_SDO_L;			//Set DC=0, for writing to Command register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
	
	SSD_SPI_SCLK_L;
	for(kk=0;kk<8;kk++)
	{
		if((cmd&0x80)==0x80) SSD_SPI_SDO_H;
		else         SSD_SPI_SDO_L;
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		cmd = cmd<<1;	
	}
}


void SPI_3W_SET_PAs(u8 value)
{
	u8 kk;

	SSD_SPI_SDO_H;			//Set DC=1, for writing to Data register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
//	delay_us(1);	
	SSD_SPI_SCLK_L;
	for(kk=0;kk<8;kk++)
	{
		if((value&0x80)==0x80) SSD_SPI_SDO_H;
		else         SSD_SPI_SDO_L;
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		value = value<<1;	
	}	
}
//-----------------------------------------------------
void Write_com(u16 vv)
{
	SSD_SPI_CS_L;
	SPI_3W_SET_Cmd(vv&0xff);	
}

void Write_register(u16 vv)
{
	SPI_3W_SET_PAs(vv&0xff);
	SPI_3W_SET_PAs((vv>>8)&0xff);	
}
//-----------------------------------------------------

void writec(u8 cmd)
{
	SSD_SPI_CS_L;
	SPI_3W_SET_Cmd(cmd);	
	SSD_SPI_CS_H;

}

void writed(u8 value)
{
	SSD_SPI_CS_L;
  SPI_3W_SET_PAs(value);
	SSD_SPI_CS_H;
}

void SPI_2828_WrCmd(u8 cmd)
{
	u8 i;
	
	SSD_SPI_CS_L;
	
	SSD_SPI_SDO_L;			//Set DC=0, for writing to Command register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
	

	SSD_SPI_SCLK_L;
	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0x80) 
			SSD_SPI_SDO_H;
		else       
			SSD_SPI_SDO_L;
		
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		cmd = cmd<<1;	
	}
	
	SSD_SPI_CS_H;	
}

//void SPI_2828_WrCmd(u8)
void SPI_2828_WrReg(u8 c,u16 value)
{
	SSD_SPI_CS_L;
	SPI_3W_SET_Cmd(c);
	SPI_3W_SET_PAs(value&0xff);
	SPI_3W_SET_PAs((value>>8)&0xff);	
	SSD_SPI_CS_H;	
}

u8 Read_PAs(void)
{
	u8 i;
	u8  rdValue;

//	SSD_SPI_SCLK_L;
	rdValue = 0;
	for(i=0;i<8;i++)
	{
		rdValue = rdValue<<1;
		
		SSD_SPI_SCLK_H;
		if(SSD_SPI_SDI==1)  rdValue |= 0x01;
		SSD_SPI_SCLK_L;
	}
//	Set_CSX(0);

	return rdValue;	
}

u16 SPI_READ(void)//SSD2828 ID Read 
{
	u8  cmd,rdT;
	u16 reValue;
	u8 i;
	
	SSD_SPI_CS_L;

	SSD_SPI_SDO_L;			//Set DC=0, for writing to Command register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
	

	cmd = 0xB0;
	SSD_SPI_SCLK_L;
	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0x80) SSD_SPI_SDO_H;
		else         SSD_SPI_SDO_L;
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		cmd = cmd<<1;	
	}
	
	SSD_SPI_SDO_L;			//Set DC=0, for writing to Command register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
	

	cmd = 0xFA;
	SSD_SPI_SCLK_L;
	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0x80) SSD_SPI_SDO_H;
		else         SSD_SPI_SDO_L;
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		cmd = cmd<<1;	
	}	
	
	rdT=0;
	for(i=0;i<8;i++)
	{
		rdT = rdT<<1;
		SSD_SPI_SCLK_H;
		if(SSD_SPI_SDI==0) 
                  rdT |= 0x00;
                else
                  rdT |= 0x01;
                
		SSD_SPI_SCLK_L;				
	}
	
	reValue = rdT;
	//reValue = (reValue<<8)&0xFF00;
	
	rdT=0;
	for(i=0;i<8;i++)
	{
		rdT = rdT<<1;
		SSD_SPI_SCLK_H;
		if(SSD_SPI_SDI==0) 
                  rdT |= 0x00;
                else
                  rdT |= 0x01;
                
		SSD_SPI_SCLK_L;				
	}
	
	reValue += (rdT<<8);
	
	SSD_SPI_CS_H;
	
	return reValue;			
}

u8   SPI_READ_ID(void)
{
	//SPI_2828_WrReg(0xd4, 0x00FA);?
	u16 Read_ID;
	u8 Flag;
	//Read_ID=0x2825;
	Read_ID=SPI_READ();
//	printf("The Chip ID is : 0x%x\n",Read_ID);
	if(Read_ID == 0x2828)
			 Flag=0x55;
		else
			 Flag=0;
	return Flag;
}

u16 SSD2828_READ(u8 reg)//SSD2828 Read Reg 
{
	u8  cmd,rdT;
	u16 reValue;
	u8 i;
	
	SSD_SPI_CS_L;

	SSD_SPI_SDO_L;			//Set DC=0, for writing to Command register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
	

	cmd = reg;//cmd = 0xB0;//read address
	SSD_SPI_SCLK_L;
	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0x80) SSD_SPI_SDO_H;
		else         SSD_SPI_SDO_L;
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		cmd = cmd<<1;	
	}
	
	SSD_SPI_SDO_L;			//Set DC=0, for writing to Command register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
	

	cmd = 0xFA;
	SSD_SPI_SCLK_L;
	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0x80) SSD_SPI_SDO_H;
		else         SSD_SPI_SDO_L;
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		cmd = cmd<<1;	
	}	
	
	rdT=0;
	for(i=0;i<8;i++)
	{
		rdT = rdT<<1;
		SSD_SPI_SCLK_H;
		
                if(SSD_SPI_SDI==0) 
                  rdT |= 0x00;
                else
                  rdT |= 0x01;
                
		SSD_SPI_SCLK_L;				
	}
	
	reValue = rdT;
	//reValue = (reValue<<8)&0xFF00;
	
	rdT=0;
	for(i=0;i<8;i++)
	{
		rdT = rdT<<1;
		SSD_SPI_SCLK_H;
		
                if(SSD_SPI_SDI==0) 
                  rdT |= 0x00;
                else
                  rdT |= 0x01;
                
		SSD_SPI_SCLK_L;				
	}
	
	reValue += (rdT<<8);
	
	SSD_SPI_CS_H;
	
	return reValue;			
}

u16   SSD2828_READ_Dat(u8 reg)// SSD2828 Read Reg 
{

	u16 Read_Dat;
	Read_Dat=SSD2828_READ(reg);
  return Read_Dat;
}

void SPI_WriteData(u8 value)
{
//	printf("-%2x",value);
	SSD_SPI_CS_L;
	SPI_3W_SET_PAs(value);
	SSD_SPI_CS_H;	
}

void GP_COMMAD_PA(u16 num)//New
{
      SPI_2828_WrReg(0xbc, num);//Oxbc-0xbd---num 
	  //  Write_com(0x00bd);
		//	SPI_2828_WrReg(0xbe, num);
     // Write_com(0x00bf);
	  writec(0xbf);
      SSD_SPI_CS_H;	
}

//////////////////////////////////////////////////////////////////////////
u8   SPI_READ_new(void)
{
	u8  rdT;
	u8 reValue;
	u8 kk;
	
	SSD_SPI_CS_L;

	rdT=0;
	for(kk=0;kk<8;kk++)
	{
		rdT = rdT<<1;
		SSD_SPI_SCLK_H;
		if(SSD_SPI_SDI&0x0800) rdT |= 0x01;
		SSD_SPI_SCLK_L;				
	}
	
	reValue = rdT;

	SSD_SPI_CS_H;
	
	return reValue;			
}

void MIPI_7inch_1200x1920_LCD_Init(void)
{
	int ret;

		SPI_2828_WrCmd(0xb7);
		SPI_WriteData(0x50);//50=TX_CLK 70=PCLK，TX_CLK 24M，PLL=148.5M*24/3=
		SPI_WriteData(0x00);   //Configuration Register

		SPI_2828_WrCmd(0xb8);
		SPI_WriteData(0x00);
		SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

		SPI_2828_WrCmd(0xb9);
		SPI_WriteData(0x00);//1=PLL disable
		SPI_WriteData(0x00);
		//TX_CLK/MS should be between 5Mhz to100Mhz
		SPI_2828_WrCmd(0xBA);//PLL=(TX_CLK/MS)*NS 8228=480M 4428=240M  061E=120M 4214=240M 821E=360M 8219=300M
		//SPI_WriteData(0x06);//D7-0=NS(0x01 : NS=1) //12MHz xtal
		SPI_WriteData(0x03);//D7-0=NS(0x01 : NS=1) //24MHz xtal
		SPI_WriteData(0x80);//D15-14=PLL  00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)

		SPI_2828_WrCmd(0xBB);//LP Clock Divider LP clock = 400MHz / LPD / 8 = 240 / 8 / 4 = 7.5MHz
		SPI_WriteData(0x06);//D5-0=LPD=0x1 – Divide by 2
		SPI_WriteData(0x00);
		
		//DIS_Delayms(100);
		DIS_Delayms(80);

		SPI_2828_WrCmd(0xb9);
		SPI_WriteData(0x01);//1=PLL enable
		SPI_WriteData(0x00);
		
		//DIS_Delayms(50);
		DIS_Delayms(40);
		
		//MIPI lane configuration
		SPI_2828_WrCmd(0xDE);//通道数
		//SPI_WriteData(0x00);//11=4LANE 10=3LANE 01=2LANE 00=1LANE
		SPI_WriteData(0x03);//11=4LANE 10=3LANE 01=2LANE 00=1LANE tim.yep
		SPI_WriteData(0x00);

		SPI_2828_WrCmd(0xc9);
		SPI_WriteData(0x02);
		SPI_WriteData(0x23);   //p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
		
		DIS_Delayms(10);
 
///////////////////////////////////////////////////////////////
		SPI_2828_WrCmd(0xB7);	//Configuration Register
		//SPI_WriteData(0x50);	//DSC write	
		//SPI_WriteData(0x00);
    SPI_WriteData(0x60);	//generic write	
		SPI_WriteData(0x02);   		
		
		//DIS_Delayms(100);
		DIS_Delayms(80);
		
		SPI_2828_WrCmd(0xB7);	//Configuration Register
    SPI_WriteData(0x20);	//generic write	
		SPI_WriteData(0x02); 
		
		//DIS_Delayms(200);
		DIS_Delayms(160);
			//MCAP write
      SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF);
		  SPI_WriteData(0xB0);
      SPI_WriteData(0x00);
      DIS_Delayms(40);
			
			//Sequencer Timing Control for Pon(NVM load relative) ??? example code not same as pannel datasheet, tim.yep 
      SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF);
		  SPI_WriteData(0xD6);
      SPI_WriteData(0x01);
			DIS_Delayms(40);
			
			//Interface setting
      SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x06);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF);
		  SPI_WriteData(0xB3);
      SPI_WriteData(0x14);
			SPI_WriteData(0x08);
			SPI_WriteData(0x00);
			SPI_WriteData(0x22);
			SPI_WriteData(0x00);
			DIS_Delayms(80);
			
			//Interface ID setting
      SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF);
		  SPI_WriteData(0xB4);
      SPI_WriteData(0x0C);
			DIS_Delayms(40);
			
			SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
			SPI_WriteData(0xDE);
      SPI_WriteData(0x00);
			DIS_Delayms(40);
			
			//DSI control
      SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x03);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF);
		  SPI_WriteData(0xB6);
      SPI_WriteData(0x3A);
			SPI_WriteData(0xD3);
			DIS_Delayms(40);
			
			SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
			SPI_WriteData(0xc0);
      SPI_WriteData(0x00);
			
			SPI_2828_WrCmd(0xBC);
		  SPI_WriteData(0x25);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
			SPI_WriteData(0xc1);
      SPI_WriteData(0x0c);
			SPI_WriteData(0x61);
			SPI_WriteData(0x00);
			SPI_WriteData(0x20);
			SPI_WriteData(0x00);
			SPI_WriteData(0x01);
			SPI_WriteData(0x40);
			SPI_WriteData(0x01);
			SPI_WriteData(0x00);
			SPI_WriteData(0x1C);
			SPI_WriteData(0xF7);
			SPI_WriteData(0xE0);
			SPI_WriteData(0x62);
			SPI_WriteData(0xAD);
			SPI_WriteData(0xB1);
			SPI_WriteData(0x05);
			SPI_WriteData(0x9F);
			SPI_WriteData(0x3B);
			SPI_WriteData(0x00);
			SPI_WriteData(0x80);
			SPI_WriteData(0x02);
			SPI_WriteData(0x80);
			SPI_WriteData(0x00);
			SPI_WriteData(0x01);
			SPI_WriteData(0x00);
			SPI_WriteData(0x11);
			SPI_WriteData(0x11);
			SPI_WriteData(0x11);
			SPI_WriteData(0x11);
			SPI_WriteData(0x80);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x22);
			SPI_WriteData(0x00);
			SPI_WriteData(0x01);
			SPI_2828_WrCmd(0xBC); //write len
			SPI_WriteData(0x0A);
			SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF);
			SPI_WriteData(0xC2);
      DIS_Delayms(6);
			SPI_WriteData(0x31);
			SPI_WriteData(0xF7);
			SPI_WriteData(0x80);
			SPI_WriteData(0x00);
			SPI_WriteData(0x08);
			SPI_WriteData(0x00);
			SPI_WriteData(0x08);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x0D);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xC4);
			SPI_WriteData(0x70);
			SPI_WriteData(0x03);
			SPI_WriteData(0x03);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x08);
			SPI_WriteData(0x02);
			SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x15);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xC6); //153
			SPI_WriteData(0x72);
			SPI_WriteData(0x01);
			SPI_WriteData(0x45);
			SPI_WriteData(0x01);
			SPI_WriteData(0x66);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x01);
			SPI_WriteData(0x0B);
			SPI_WriteData(0x0A);
			SPI_WriteData(0x05);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x1F);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xC7); //178
			SPI_WriteData(0x00);
			SPI_WriteData(0x0F);
			SPI_WriteData(0x17);
			SPI_WriteData(0x21);
			SPI_WriteData(0x30);
			SPI_WriteData(0x3F);
			SPI_WriteData(0x49);
			SPI_WriteData(0x5A);
			SPI_WriteData(0x41);
			SPI_WriteData(0x49);
			SPI_WriteData(0x57);
			SPI_WriteData(0x6B);
			SPI_WriteData(0x72);
			SPI_WriteData(0x75);
			SPI_WriteData(0x76);
			SPI_WriteData(0x00);
			SPI_WriteData(0x0F);
			SPI_WriteData(0x17);
			SPI_WriteData(0x21);
			SPI_WriteData(0x30);
			SPI_WriteData(0x3F);
			SPI_WriteData(0x49);
			SPI_WriteData(0x5A);
			SPI_WriteData(0x41);
			SPI_WriteData(0x49);
			SPI_WriteData(0x57);
			SPI_WriteData(0x6B);
			SPI_WriteData(0x72);
			SPI_WriteData(0x75);
			SPI_WriteData(0x76);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x14);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xC8); //213
			SPI_WriteData(0x01);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0xFE);
			SPI_WriteData(0xFE);
			SPI_WriteData(0xFC);
			SPI_WriteData(0xF0);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0xFC);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0xFC);
			SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x05);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xC9); //237
			SPI_WriteData(0x00);
			SPI_WriteData(0x03);
			SPI_WriteData(0x01);
			SPI_WriteData(0x53);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x0D);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xCB); //246
			SPI_WriteData(0x6A);
			SPI_WriteData(0xFC);
			SPI_WriteData(0xFF);
			SPI_WriteData(0x58);
			SPI_WriteData(0x01);
			SPI_WriteData(0x04);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x80);
			SPI_WriteData(0x00);
			SPI_WriteData(0x3C);
			SPI_WriteData(0xCF);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xCC); 
			SPI_WriteData(0x07);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x0B);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xD0); //269
			SPI_WriteData(0x55);
			SPI_WriteData(0x81);
			SPI_WriteData(0xBB);
			SPI_WriteData(0x17);
			SPI_WriteData(0x0C);
			SPI_WriteData(0x4C);
			SPI_WriteData(0x19);
			SPI_WriteData(0x19);
			SPI_WriteData(0x0C);
			SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x1B);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xD3); //284
			SPI_WriteData(0x0B);
			SPI_WriteData(0x33);
			SPI_WriteData(0xBB);
			SPI_WriteData(0xBB); //288
			SPI_WriteData(0xB3);
			SPI_WriteData(0x33);
			SPI_WriteData(0x33);
			SPI_WriteData(0x33); 
			SPI_WriteData(0x00); //293
			SPI_WriteData(0x01);
			SPI_WriteData(0x00);
			SPI_WriteData(0xA0);
			SPI_WriteData(0xD8);
			SPI_WriteData(0xA0); //298
			SPI_WriteData(0x04);
			SPI_WriteData(0x43);
			SPI_WriteData(0x43);
			SPI_WriteData(0x33);
			SPI_WriteData(0x3B); //303
			SPI_WriteData(0x22);
			SPI_WriteData(0x72);
			SPI_WriteData(0x07);
			SPI_WriteData(0x3D);
			SPI_WriteData(0xBF);
			SPI_WriteData(0x99);
			SPI_WriteData(0x11);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x08);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xD5); //315
			SPI_WriteData(0x06);
			SPI_WriteData(0x00);
			SPI_WriteData(0x00);
			SPI_WriteData(0x01);
			SPI_WriteData(0x2C);
			SPI_WriteData(0x01);
			SPI_WriteData(0x2C);
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0xD6);
			SPI_WriteData(0x01);
			SPI_2828_WrCmd(0xB7); 
		  SPI_WriteData(0x60);
			SPI_WriteData(0x02);
			
			DIS_Delayms(40);
			
			//write display brightness
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
			SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0x51);
			SPI_WriteData(0xE0); //337
			
			DIS_Delayms(40);
						
			//write control display
      SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0x53);
			SPI_WriteData(0x04);
			
			DIS_Delayms(40);
				
			//set_pixel_format??? example code not same as pannel datasheet, tim.yep
      SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0x3A);
      SPI_WriteData(0x77);
			
			DIS_Delayms(40);
			
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0x35);
      SPI_WriteData(0x01);
			
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0x36);
      SPI_WriteData(0x48);
			
			SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0x29);
      SPI_WriteData(0x00);
			
			DIS_Delayms(100);
			
			//exit sleep mode  ??? example code not same as pannel datasheet, tim.yep
      SPI_2828_WrCmd(0xBC); //write len
		  SPI_WriteData(0x02);
      SPI_WriteData(0x00);
      SPI_2828_WrCmd(0xBF); 
		  SPI_WriteData(0x11);
			SPI_WriteData(0x00); //373
      DIS_Delayms(200);
			
///////////////////////////////////////////////////////////////	

		//SSD2828_Initial
		SPI_2828_WrCmd(0xb7);
		SPI_WriteData(0x50);
		SPI_WriteData(0x00);   //Configuration Register

		SPI_2828_WrCmd(0xb8);
		SPI_WriteData(0x00);
		SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

		SPI_2828_WrCmd(0xb9);
		SPI_WriteData(0x00);//1=PLL disable
		SPI_WriteData(0x00);

	  SPI_2828_WrCmd(0xBA);//PLL=(TX_CLK/MS)*NS 8228=480M 4428=240M  061E=120M 4214=240M 821E=360M 8219=300M 8225=444M 8224=432
	

		SPI_WriteData(0x50);//D7-0=NS(0x01 : NS=1)   //0X28  57  
    //SPI_WriteData(0x9D);//12MHz xtal
		//SPI_WriteData(0x50);//24MHz xtal
		//SPI_WriteData(0x52);//D7-0=NS(0x01 : NS=1)   //0X28  57 
		//SPI_WriteData(0x50);//D7-0=NS(0x01 : NS=1)   //0X28  57 
		SPI_WriteData(0xC2);//D15-14=PLL范围 00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1) //0X82
		
		SPI_2828_WrCmd(0xb9);
		SPI_WriteData(0x01);//1=PLL enbale
		SPI_WriteData(0x00);

		SPI_2828_WrCmd(0xc9);
		SPI_WriteData(0x02); //0x02
		SPI_WriteData(0x23);   //p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue  0x23
		//DIS_Delayms(10);

		SPI_2828_WrCmd(0xCA);
		SPI_WriteData(0x01);//CLK Prepare //0x01
		SPI_WriteData(0x23);//Clk Zero   //0x23
		DIS_Delayms(8);

		SPI_2828_WrCmd(0xCB); //local_write_reg(addr=0xCB,data=0x0510)
		SPI_WriteData(0x10); //Clk Post  //0x10
		SPI_WriteData(0x05); //Clk Per    /0x05

		SPI_2828_WrCmd(0xCC); //local_write_reg(addr=0xCC,data=0x100A)
		SPI_WriteData(0x05); //HS Trail 05
		SPI_WriteData(0x10); //Clk Trail 10  //400
		//DIS_Delayms(10);

		//RGB interface configuration

		SPI_2828_WrReg(0xb1,(LCD_VSPW<<8)|LCD_HSPW);	//Vertical sync and horizontal sync active period 
		SPI_2828_WrReg(0xb2,(LCD_VBPD<<8)|LCD_HBPD);	//Vertical and horizontal back porch period  
		SPI_2828_WrReg(0xb3,(LCD_VFPD<<8)|LCD_HFPD);	//Vertical and horizontal front porch period 
		SPI_2828_WrReg(0xb4, LCD_XSIZE_TFT);		//Horizontal active period 
		SPI_2828_WrReg(0xb5, LCD_YSIZE_TFT);		//Vertical active period
	//	SPI_2828_WrReg(0xb6, 0x001B);				//Video mode and video pixel format //HS=L,VS=L,PCLK=L;
	//	SPI_2828_WrReg(0xb6, 0xC01B);				//Video mode and video pixel format //HS=H,VS=H,PCLK=L;
		SPI_2828_WrReg(0xb6, 0x001B);				//Video mode and video pixel format //HS=H,VS=H,PCLK=L;	blj is ok	
	//	SPI_2828_WrReg(0xb6, 0xe01B);				//Video mode and video pixel format //HS=H,VS=H,PCLK=L;				
		//MIPI lane configuration
		SPI_2828_WrCmd(0xDE);//通道数
		SPI_WriteData(0x03);//11=4LANE 10=3LANE 01=2LANE 00=1LANE
		SPI_WriteData(0x00);

		SPI_2828_WrCmd(0xD6);//  05=BGR  04=RGB
		SPI_WriteData(0x05);//D0=0=RGB 1:BGR D1=1=Most significant byte sent first
		SPI_WriteData(0x00);

		SPI_2828_WrCmd(0xB7);
		SPI_WriteData(0x4B); //0X4B
		SPI_WriteData(0x02);
		/*DIS_Delayms(100);
		
		printf("--%s\r\n", __FUNCTION__);*/
}

void SSD2828_WritePackageSize(uint16_t wr_len)
{
/*
    SPI_2828_WrCmd(0xBC); //write len
    SPI_WriteData(wr_len);
    SPI_WriteData(0x00);
*/
    SPI_2828_WrReg(0xBC, wr_len);
		SPI_2828_WrCmd(0xBF);	
}

#define SPI_WriteCmd SPI_2828_WrCmd
void r69429_ssd2828()
{
printf("++%s\r\n", __FUNCTION__);
	SSD_RST_L;  //复位
	DIS_Delayms(0x30);
	SSD_RST_H;
	DIS_Delayms(0x30);

SPI_WriteCmd(0xb7);
SPI_WriteData(0x50);//  LP mode(DCS mode & HS clk disable) 
SPI_WriteData(0x00);    


SPI_WriteCmd(0xb8);
SPI_WriteData(0x00);
SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

SPI_WriteCmd(0xb9);
SPI_WriteData(0x00);//1=PLL disable
SPI_WriteData(0x00);
DIS_Delayms(10);
//TX_CLK/MS should be between 5Mhz to100Mhz
SPI_WriteCmd(0xBA);
SPI_WriteData(0x14);
SPI_WriteData(0x42);
DIS_Delayms(10);
SPI_WriteCmd(0xBB);
SPI_WriteData(0x03);
SPI_WriteData(0x00);
DIS_Delayms(10);
SPI_WriteCmd(0xb9);
SPI_WriteData(0x01);//1=PLL disable
SPI_WriteData(0x00);
DIS_Delayms(100);

SPI_WriteCmd(0xDE);
SPI_WriteData(0x03);
SPI_WriteData(0x00);  
DIS_Delayms(10);

SPI_WriteCmd(0xc9);
SPI_WriteData(0x02);
SPI_WriteData(0x23);
DIS_Delayms(100);

//////////////////Initial  CODE///////////////////////




	SPI_WriteCmd(0xB7);
	SPI_WriteData(0x10);//10=TX_CLK 30=PCLK
	SPI_WriteData(0x02);


	SPI_WriteCmd(0xBD);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

    
SSD2828_WritePackageSize(2);
SPI_WriteData(0xb0); 
SPI_WriteData(0x00); 
 DIS_Delayms(40);
SSD2828_WritePackageSize(2);
SPI_WriteData(0xd6); 
SPI_WriteData(0x01);
 DIS_Delayms(40);

SSD2828_WritePackageSize(6);
SPI_WriteData(0xb3); 
SPI_WriteData(0x14); 
SPI_WriteData(0x08); 
SPI_WriteData(0x00); 
SPI_WriteData(0x22); 
SPI_WriteData(0x00); 
 DIS_Delayms(80);
SSD2828_WritePackageSize(2);
SPI_WriteData(0xb4); 
SPI_WriteData(0x0c); 
 DIS_Delayms(40);
SSD2828_WritePackageSize(2);
SPI_WriteData(0xDE); 
SPI_WriteData(0x00); 
 DIS_Delayms(40);

SSD2828_WritePackageSize(3);
SPI_WriteData(0xb6); 
SPI_WriteData(0x3a);
SPI_WriteData(0xd3);
 DIS_Delayms(40);



SSD2828_WritePackageSize(2);
SPI_WriteData(0xc0);
SPI_WriteData(0x00);


SSD2828_WritePackageSize(37);
SPI_WriteData(0xc1);
SPI_WriteData(0x0c);
SPI_WriteData(0x61);
SPI_WriteData(0x00);
SPI_WriteData(0x20);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteData(0x40);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x1c);
SPI_WriteData(0xf7);
SPI_WriteData(0xe0);
SPI_WriteData(0x62);
SPI_WriteData(0xad);
SPI_WriteData(0xb1);
SPI_WriteData(0x05);
SPI_WriteData(0x9f);
SPI_WriteData(0x3b);
SPI_WriteData(0x00);
SPI_WriteData(0x80);
SPI_WriteData(0x02);
SPI_WriteData(0x80);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x11);
SPI_WriteData(0x11);
SPI_WriteData(0x11);
SPI_WriteData(0x11);
SPI_WriteData(0x80);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x22);
SPI_WriteData(0x00);
SPI_WriteData(0x01);


SSD2828_WritePackageSize(10);
SPI_WriteData(0xc2);
SPI_WriteData(0x31);
SPI_WriteData(0xf7);
SPI_WriteData(0x80);
SPI_WriteData(0x00);
SPI_WriteData(0x08);
SPI_WriteData(0x00);
SPI_WriteData(0x08);
SPI_WriteData(0x00);
SPI_WriteData(0x00);


SSD2828_WritePackageSize(13);
SPI_WriteData(0xc4);
SPI_WriteData(0x70);
SPI_WriteData(0x03);
SPI_WriteData(0x03);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x08);
SPI_WriteData(0x02);
SPI_WriteData(0x00);


SSD2828_WritePackageSize(21);
SPI_WriteData(0xc6);
SPI_WriteData(0x72);
SPI_WriteData(0x01);
SPI_WriteData(0x45);
SPI_WriteData(0x01);
SPI_WriteData(0x66);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x01);
SPI_WriteData(0x0b);
SPI_WriteData(0x1a);
SPI_WriteData(0x05);


SSD2828_WritePackageSize(31);
SPI_WriteData(0xc7);
SPI_WriteData(0x00);
SPI_WriteData(0x0f);
SPI_WriteData(0x17);
SPI_WriteData(0x21);
SPI_WriteData(0x30);
SPI_WriteData(0x3f);
SPI_WriteData(0x49);
SPI_WriteData(0x5a);
SPI_WriteData(0x41);
SPI_WriteData(0x49);
SPI_WriteData(0x57);
SPI_WriteData(0x6B);
SPI_WriteData(0x72);
SPI_WriteData(0x75);
SPI_WriteData(0x76);
SPI_WriteData(0x00);
SPI_WriteData(0x0F);
SPI_WriteData(0x17);
SPI_WriteData(0x21);
SPI_WriteData(0x30);
SPI_WriteData(0x3F);
SPI_WriteData(0x49);
SPI_WriteData(0x5A);
SPI_WriteData(0x41);
SPI_WriteData(0x49);
SPI_WriteData(0x57);
SPI_WriteData(0x6B);
SPI_WriteData(0x72);
SPI_WriteData(0x75);
SPI_WriteData(0x76);


SSD2828_WritePackageSize(20);
SPI_WriteData(0xc8);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0xfe);
SPI_WriteData(0xfe);
SPI_WriteData(0xfc);
SPI_WriteData(0xf0);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0xfc);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0xfc);
SPI_WriteData(0x00);


SSD2828_WritePackageSize(5);
SPI_WriteData(0xc9);
SPI_WriteData(0x00);
SPI_WriteData(0x03);
SPI_WriteData(0x01);
SPI_WriteData(0x53);


SSD2828_WritePackageSize(13);
SPI_WriteData(0xcb);
SPI_WriteData(0x6a);
SPI_WriteData(0xfc);
SPI_WriteData(0xff);
SPI_WriteData(0x58);
SPI_WriteData(0x01);
SPI_WriteData(0x04);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x80);
SPI_WriteData(0x00);
SPI_WriteData(0x3c);
SPI_WriteData(0xcf);


SSD2828_WritePackageSize(2);
SPI_WriteData(0xcc);
SPI_WriteData(0x07);



SSD2828_WritePackageSize(11);
SPI_WriteData(0xd0);
SPI_WriteData(0x55);
SPI_WriteData(0x81);
SPI_WriteData(0xbb);
SPI_WriteData(0x17);
SPI_WriteData(0x0c);
SPI_WriteData(0x4c);
SPI_WriteData(0x19);
SPI_WriteData(0x19);
SPI_WriteData(0x0c);
SPI_WriteData(0x00);


SSD2828_WritePackageSize(27);
SPI_WriteData(0xd3);
SPI_WriteData(0x0b);
SPI_WriteData(0x33);
SPI_WriteData(0xbb);
SPI_WriteData(0xbb);
SPI_WriteData(0xb3);
SPI_WriteData(0x33);
SPI_WriteData(0x33);
SPI_WriteData(0x33);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteData(0x00);
SPI_WriteData(0xa0);
SPI_WriteData(0xd8);
SPI_WriteData(0xa0);
SPI_WriteData(0x04);
SPI_WriteData(0x43);
SPI_WriteData(0x43);
SPI_WriteData(0x33);
SPI_WriteData(0x3b);
SPI_WriteData(0x22);
SPI_WriteData(0x72);
SPI_WriteData(0x07);
SPI_WriteData(0x3d);
SPI_WriteData(0xbf);
SPI_WriteData(0x99);
SPI_WriteData(0x11);


SSD2828_WritePackageSize(8);
SPI_WriteData(0xd5);
SPI_WriteData(0x06);
SPI_WriteData(0x00);
SPI_WriteData(0x00);
SPI_WriteData(0x01);
SPI_WriteData(0x2c);
SPI_WriteData(0x01);
SPI_WriteData(0x2c);

SSD2828_WritePackageSize(2);
SPI_WriteData(0xd6);
SPI_WriteData(0x01);




SPI_WriteCmd(0xB7);
SPI_WriteData(0x60);
SPI_WriteData(0x02);
DIS_Delayms(40);

SSD2828_WritePackageSize(2);
SPI_WriteData(0x51); 
SPI_WriteData(0xE0); 
DIS_Delayms(40);
SSD2828_WritePackageSize(2);
SPI_WriteData(0x53); 
SPI_WriteData(0x04); 
DIS_Delayms(40);
SSD2828_WritePackageSize(2);
SPI_WriteData(0x3a); 
SPI_WriteData(0x77); 
DIS_Delayms(40);

SSD2828_WritePackageSize(2);
SPI_WriteData(0x35); 
SPI_WriteData(0x01);

SSD2828_WritePackageSize(5);
SPI_WriteData(0x2A); 
SPI_WriteData(0x00); 
SPI_WriteData(0x00); 
SPI_WriteData(0x04); 
SPI_WriteData(0xAF); 
DIS_Delayms(40);
SSD2828_WritePackageSize(5);
SPI_WriteData(0x2B); 
SPI_WriteData(0x00); 
SPI_WriteData(0x00); 
SPI_WriteData(0x07); 
SPI_WriteData(0x7F); 


DIS_Delayms(100);
 


SSD2828_WritePackageSize(1);
SPI_WriteData(0x29);
 DIS_Delayms(150);

        
SSD2828_WritePackageSize(1);
SPI_WriteData(0x11); 
DIS_Delayms(300);




////////////////////Initial  CODE/////////////////////



//SSD2828_Initial
SPI_WriteCmd(0xb7);
SPI_WriteData(0x50);
SPI_WriteData(0x00);  
 
SPI_WriteCmd(0xb8);
SPI_WriteData(0x00);
SPI_WriteData(0x00);   

SPI_WriteCmd(0xb9);
SPI_WriteData(0x00);
SPI_WriteData(0x00);

SPI_WriteCmd(0xBA);
SPI_WriteData(0x52);
SPI_WriteData(0xc2);

SPI_WriteCmd(0xBB);
SPI_WriteData(0x07);
SPI_WriteData(0x00);

SPI_WriteCmd(0xb9);
SPI_WriteData(0x01);
SPI_WriteData(0x00);

SPI_WriteCmd(0xc9);
SPI_WriteData(0x02);
SPI_WriteData(0x23); 
DIS_Delayms(5);

SPI_WriteCmd(0xCA);
SPI_WriteData(0x01);
SPI_WriteData(0x23);

SPI_WriteCmd(0xCB); 
SPI_WriteData(0x10);
SPI_WriteData(0x05);

SPI_WriteCmd(0xCC); 
SPI_WriteData(0x05);
SPI_WriteData(0x10);
DIS_Delayms(5);

SPI_WriteCmd(0xD0); 
SPI_WriteData(0x00);
SPI_WriteData(0x00);
DIS_Delayms(5);

/*
LoadLcdParameterConfig();   	
*/
//RGB interface configuration

		SPI_2828_WrReg(0xb1,(LCD_VSPW<<8)|LCD_HSPW);	//Vertical sync and horizontal sync active period 
		SPI_2828_WrReg(0xb2,(LCD_VBPD<<8)|LCD_HBPD);	//Vertical and horizontal back porch period  
		SPI_2828_WrReg(0xb3,(LCD_VFPD<<8)|LCD_HFPD);	//Vertical and horizontal front porch period 
		SPI_2828_WrReg(0xb4, LCD_XSIZE_TFT);		//Horizontal active period 
		SPI_2828_WrReg(0xb5, LCD_YSIZE_TFT);		//Vertical active period
	//	SPI_2828_WrReg(0xb6, 0x001B);				//Video mode and video pixel format //HS=L,VS=L,PCLK=L;
	//	SPI_2828_WrReg(0xb6, 0xC01B);				//Video mode and video pixel format //HS=H,VS=H,PCLK=L;
	//	SPI_2828_WrReg(0xb6, 0x200B);				//Video mode and video pixel format //HS=H,VS=H,PCLK=L;	blj is ok	
	//	SPI_2828_WrReg(0xb6, 0xe01B);				//Video mode and video pixel format //HS=H,VS=H,PCLK=L;				
		//MIPI lane configuration


SPI_WriteCmd(0xb6);
SPI_WriteData(0x03);
SPI_WriteData(0x00);


SPI_WriteCmd(0xDE);
SPI_WriteData(0x03);  //01 2LINE  02  3LINE  03 4LINE
SPI_WriteData(0x00);

SPI_WriteCmd(0xD6);
SPI_WriteData(0x04);
SPI_WriteData(0x00);

SPI_WriteCmd(0xB7);
SPI_WriteData(0x4B);
SPI_WriteData(0x02);DIS_Delayms(100);

SPI_WriteCmd(0x2C);
printf("--%s\r\n", __FUNCTION__);
}


void MIPI_Read_Init(void)
{
  	DIS_Delayms(200);
	SSD_RST_L;// ( rGPFDAT &= (~(1<<3))) ;
	//LCD_RST_L;
	DIS_Delayms(50);
	SSD_RST_H;//  ( rGPFDAT |= (1<<3) ) ;
	//LCD_RST_H;
	DIS_Delayms(150);
  
	SPI_2828_WrCmd(0xb7);
		SPI_WriteData(0x50);//50=TX_CLK 70=PCLK
		SPI_WriteData(0x00);   //Configuration Register
 
		SPI_2828_WrCmd(0xb8);
		SPI_WriteData(0x00);
		SPI_WriteData(0x00);   //VC(Virtual ChannelID) Control Register

		SPI_2828_WrCmd(0xb9);
		SPI_WriteData(0x00);//1=PLL disable
		SPI_WriteData(0x00);
	 
    SPI_2828_WrCmd(0xBA);//PLL=(TX_CLK/MS)*NS 8228=480M 4428=240M  061E=120M 4214=240M 821E=360M 8219=300M
		SPI_WriteData(0x07);//14//D7-0=NS(0x01 : NS=1)//11
		SPI_WriteData(0xC0);//42//D15-14=PLL?? 00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)
		
 //   SPI_2828_WrCmd(0xBA);//PLL=(TX_CLK/MS)*NS 8228=480M 4428=240M  061E=120M 4214=240M 821E=360M 8219=300M
//		SPI_WriteData(0x14);//14//D7-0=NS(0x01 : NS=1)//11
//		SPI_WriteData(0x02);//42//D15-14=PLL?? 00=62.5-125 01=126-250 10=251-500 11=501-1000  DB12-8=MS(01:MS=1)
		
		SPI_2828_WrCmd(0xBB);//LP Clock Divider LP clock = 400MHz / LPD / 8 = 240 / 8 / 4 = 7.5MHz
		SPI_WriteData(0x03);//D5-0=LPD=0x1 – Divide by 2  //12
		SPI_WriteData(0x00);
		
		SPI_2828_WrCmd(0xb9);
		SPI_WriteData(0x01);//1=PLL disable
		SPI_WriteData(0x00);
	 
		//MIPI lane configuration
		SPI_2828_WrCmd(0xDE);//???
		SPI_WriteData(0x03);//11=4LANE 10=3LANE 01=2LANE 00=1LANE
		SPI_WriteData(0x00);

		SPI_2828_WrCmd(0xc9);
		SPI_WriteData(0x02);
		SPI_WriteData(0x23);   //p1: HS-Data-zero  p2: HS-Data- prepare  --> 8031 issue
		DIS_Delayms(100);
 
	SPI_2828_WrCmd(0xB7);
	SPI_WriteData(0x10);//10=TX_CLK 30=PCLK
	SPI_WriteData(0x02);


	SPI_2828_WrCmd(0xBD);
	SPI_WriteData(0x00);
	SPI_WriteData(0x00);

}



u16 SSD2828_READ_MIPI(void)//SSD2828 Read Mipi
{
	u8  cmd,rdT;
	u16 reValue;
	u8 i;
	
	SSD_SPI_CS_L;

	SSD_SPI_SDO_L;			//Set DC=0, for writing to Command register
	SSD_SPI_SCLK_L;
	SSD_SPI_SCLK_H;
	
	cmd = 0xFA;
	SSD_SPI_SCLK_L;
	for(i=0;i<8;i++)
	{
		if((cmd&0x80)==0x80) SSD_SPI_SDO_H;
		else         SSD_SPI_SDO_L;
		SSD_SPI_SCLK_H;
		SSD_SPI_SCLK_L;
		cmd = cmd<<1;	
	}	
	
	rdT=0;
	for(i=0;i<8;i++)
	{
		rdT = rdT<<1;
		SSD_SPI_SCLK_H;
                
   if(SSD_SPI_SDI==0) 
                  rdT |= 0x00;
                else
                  rdT |= 0x01;
                
                
		SSD_SPI_SCLK_L;				
	}
	
	reValue = rdT;
	//reValue = (reValue<<8)&0xFF00;
	
	rdT=0;
	for(i=0;i<8;i++)
	{
		rdT = rdT<<1;
		SSD_SPI_SCLK_H;
                
		   if(SSD_SPI_SDI==0) 
                  rdT |= 0x00;
                else
                  rdT |= 0x01;
                
		SSD_SPI_SCLK_L;				
	}
	
	reValue += (rdT<<8);
	
	SSD_SPI_CS_H;
	
	return reValue;			
}

#endif
