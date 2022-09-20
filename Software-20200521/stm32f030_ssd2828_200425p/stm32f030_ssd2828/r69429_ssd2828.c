

void SSD2828_WritePackageSize(uint8_t wr_len)
{
    SPI_2828_WrCmd(0xBC); //write len
    SPI_WriteData(wr_len);
    SPI_WriteData(0x00);
}

#define SPI_WriteCmd SPI_2828_WrCmd
void Initial_LCM()
{
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
SPI_WriteData(0x50);
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
		SPI_2828_WrReg(0xb6, 0x200B);				//Video mode and video pixel format //HS=H,VS=H,PCLK=L;	blj is ok	
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
}