/*
 * (C) Copyright 2010 by Tekkaman Ninja.
 * Author: Tekkaman Ninja <tekkamanninja@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>

#include <video_fb.h>

#include <asm/io.h>
#include <asm/types.h>

#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))
#define Delay(_a)		udelay(_a*1000)

#define LCD_WIDTH 	CONFIG_LCD_WIDTH
#define LCD_HEIGHT	CONFIG_LCD_HEIGHT

/*
 * Export Graphic Device
 */
GraphicDevice smi;

void tiny210_lcd_init(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;
	u32* pBuffer = (u32*)uFbAddr;

	Outp32(0xe0100204,0x700000); 	//CLK_SRC1-Clock Source Control Registers
					//FIMD_SEL([23:20]--> 0111: SCLKEPLL)
	Outp32(0xe0200120, 0x22222222);		//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe0200128,0x0);			//set pull-up,down disable
	Outp32(0xe0200140, 0x22222222);		//set GPF1 as VD[11:4]
	Outp32(0xe0200148,0x0);			//set pull-up,down disable
	Outp32(0xe0200160, 0x22222222);		//set GPF2 as VD[19:12]
	Outp32(0xe0200168,0x0);			//set pull-up,down disable
	Outp32(0xe0200180, 0x00002222);		//set GPF3 as VD[23:20]
	Outp32(0xe0200188,0x0);			//set pull-up,down disable

//--------- S5PC110 EVT0 needs MAX drive strength	---------//
	Outp32(0xe020012c,0xffffffff);		//set GPF0 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020014c,0xffffffff);		//set GPF1 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020016c,0xffffffff);		//set GPF2 drive strength max by WJ.KIM(09.07.17)
	Outp32(0xe020018c,0x3ff);		//set GPF3 drive strength max by WJ.KIM(09.07.17)

	Outp32(0xf8000130, 0x20);	//Set VIDINTCON0[5:0] to enable Window 0 FIFO Interrupt
	Outp32(0xf8000020, 0x0);	//WINCON0-Window 0 Control Register
	Outp32(0xf8000024, 0x0);	//WINCON1-Window 1 Control Register
	Outp32(0xf8000028, 0x0);	//WINCON2-Window 2 Control Register
	Outp32(0xf800002c, 0x0);	//WINCON3-Window 3 Control Register
	Outp32(0xf8000030, 0x0);	//WINCON4-Window 4 Control Register
	Outp32(0xf8000034, 0x0);	//SHODOWCON-Window Shadow Control Register
	Outp32(0xf8000180, 0x0);	//WIN0MAP 
	Outp32(0xf8000184, 0x0);	//WIN1MAP 
	Outp32(0xf8000188, 0x0);	//WIN2MAP 
	Outp32(0xf800018c, 0x0);	//WIN3MAP 
	Outp32(0xf8000190, 0x0);	//WIN4MAP 
	Outp32(0xf8000140, 0x0);	//W1KEYCON0 
	Outp32(0xf8000148, 0x0);	//W2KEYCON0 
	Outp32(0xf8000150, 0x0);	//W3KEYCON0 
	Outp32(0xf8000158, 0x0);	//W4KEYCON0 
	Outp32(0xf8000058, 0x0);	//VIDOSD1C-Window 1 Position Control C Register
	Outp32(0xf8000208, 0x0);	//VIDW1ALPHA0 set RGB alpha lower value,case AEN == 0
	Outp32(0xf800020c, 0x0);	//VIDW1ALPHA1 set RGB alpha lower value,case AEN == 1
	Outp32(0xf8000068, 0x0);	//VIDOSD2C-Window 2 Position Control C Register
	Outp32(0xf8000210, 0x0);	//(VIDW0ALPHA0-Window 2 Alpha0 Control Register
	Outp32(0xf8000214, 0x0);	//VIDW2ALPHA1-Window 2 Alpha1 control Register 
	Outp32(0xf8000078, 0x0);	//VIDOSD3C-Window 3 Position Control C Register
	Outp32(0xf8000218, 0x0);	//VIDW0ALPHA0-Window 3 Alpha0 Control Register
	Outp32(0xf800021c, 0x0);	//VIDW3ALPHA1-Window 3 Alpha1 Control Register
	Outp32(0xf8000088, 0x0);	//VIDOSD4C-Window 4 Position Control C Register
	Outp32(0xf8000220, 0x0);	//VIDW4ALPHA0-Window 4 Alpha0 Control Register
	Outp32(0xf8000224, 0x0);	//VIDW0ALPHA1-Window 4 Alpha1 Control Register
	Outp32(0xf8000260, 0x1);	//BLENDCON-Blending Equation Control Register
					//set to 8-bit alpha value
	Outp32(0xf8000048, 0x4b000);	//VIDOSD0C-Window 0 Position Control C Register
					//VIDOSD0C-[23:0] Specifies the Window Size 
	Outp32(0xf8000200, 0xffffff);	//Hue Control Registers
	Outp32(0xf8000204, 0xffffff);	//VIDW0ALPHA1-Window 0 Alpha1 control Register
	Outp32(0xf8000034, 0x0);	//SHODOWCON-Window Shadow Control Register
					//set WINCON0[15:12]=0x8 for Half-Word swep enabled
					//and double buffering and RGB source image
	Outp32(0xf80000a0, uFbAddr + 0x00000000); 	//VIDW00ADD0B0-Specifies window 0’s 
							//buffer start address register, buffer 0. 

	Outp32(0xf80000d0, uFbAddr + 0x00400800);	//VIDW00ADD1B0-Specifies window 0’s
							//buffer end address register, buffer 0. 

	Outp32(0xf80000a4, uFbAddr + 0x00000000);	//VIDW00ADD0B1-Specifies window 0’s
							//buffer start address register, buffer 1. 

	Outp32(0xf80000d4, uFbAddr + 0x00400800);	//VIDW00ADD1B1-Specifies window 0’s
							//buffer end address register, buffer 1. 

	Outp32(0xf80020a0, uFbAddr + 0x00000000);	//VIDW00ADD0B2-Specifies window 0’s
							//buffer start address register, buffer 2.
	Outp32(0xf80020d0, uFbAddr + 0x00400800);	//VIDW00ADD1B2-Specifies window 0’s
							//buffer end address register, buffer 2. 

/////////////  EDIT BY WEI  /////////////

	Outp32(0xf8000000, 0xd7);//153);	//VIDCON0-Video Main Control 0 Register
	Outp32(0xf8000004, 0x60);

	Outp32(0xf8000010,0x060400); 		//For VBPD:[0x01] VFPD:[0x08] VSPW:[0x02]
	Outp32(0xf8000014,0xc7702); 		//For HBPD:[0x05] HFPD:[0x28] HSPW:[0x02]

	Outp32(0xf8000170, 0x4c); 	//Disable dithering[bit 0];Set RGB Dither bit to 8-bit 
 	Outp32(0xf8000018, 0x13f9df);	//set	LINEVAL[21:11]=640-1=11b01001111111;
										//		HOZVAL [10: 0]=480-1=11b00111011111

	Outp32(0xf8000100, 0x3c0);	//Virtual screen page width:480*2=0x3c0
	Outp32(0xf8000040, 0x0);	//VIDOSD0A-Window 0 Position Control A Register
	Outp32(0xf8000044, 0xEFA7F);	//VIDOSD0B-Window 0 Position Control B Register
										// 0x1df=479 0x27f=639
	Outp32(0xf8000020, 0x10015);	//WINCON0-Window 0 Control Register
	Outp32(0xf8000034, 0x1);	//SHODOWCON-Window Shadow Control Register
	//Outp32(0xf8000000, 0x153);	//VIDCON0-Video Main Control 0 Register
										//Select HCLK as source and 5 as devided value
	Outp32(0xf8000134, 0x9021);
	Outp32(0xf8000134, 0x0);
	Outp32(0xf80001a4, 0x3);	//TRIGCON-I80 / RGB Trigger Control Register
	Outp32(0xe0107008,0x2); 	/* 
						DISPLAY_CONTROL-Display path selection
						DISPLAY_PATH_SEL([1:0]-->10:RGB=FIMD I80=FIMD ITU=FIMD)
					*/

/////////////  END OF EDIT  /////////////

}

void backlight_on(void)
{
	printf("Backlight on\n");
	Outp32(0xE02000A0, 0x00000010);		//set GPD0_1 as Output
	Outp32(0xe02000a4,0x2);			//set output value as 1
}

void power_enable(void)
{
	Outp32(0xE0200c20,0x0);
	Outp32(0xE0200c20,0x00000100);
	Outp32(0xe0200c24,(0x1<<2));
}

/*******************************************************************************
 *
 * Init video chip with common Linux graphic modes (lilo)
 */

void *video_hw_init (void)
{

	GraphicDevice *pGD = (GraphicDevice *)&smi;
	int bits_per_pixel = BPP;

	printf("Video: ");

	tiny210_lcd_init();
	power_enable();

	pGD->winSizeX = LCD_WIDTH; 
	pGD->winSizeY = LCD_HEIGHT;
	pGD->plnSizeX = LCD_WIDTH;
	pGD->plnSizeY = LCD_HEIGHT;

	printf ("WEI: video_hw_init: <%d, %d, %d, %d>\r\n",
				pGD->winSizeX, pGD->winSizeY,
				pGD->plnSizeX, pGD->plnSizeY);

	switch (bits_per_pixel) {
	case 8:
		pGD->gdfBytesPP = 1;
		pGD->gdfIndex = GDF__8BIT_INDEX;
		break;
	case 15:
		pGD->gdfBytesPP = 2;
		pGD->gdfIndex = GDF_15BIT_555RGB;
		break;
	case 16:
		pGD->gdfBytesPP = 2;
		pGD->gdfIndex = GDF_16BIT_565RGB;
		break;
	case 24:
		pGD->gdfBytesPP = 3;
		pGD->gdfIndex = GDF_24BIT_888RGB;
		break;
	case 32:
		pGD->gdfBytesPP = 4;
		pGD->gdfIndex = GDF_32BIT_X888RGB;
		break;
	}

	pGD->frameAdrs = CFG_LCD_FBUFFER;
	pGD->memSize = VIDEO_MEM_SIZE;

	/* Clear video memory */
	memset((void *)pGD->frameAdrs, 0x0, pGD->memSize);

	return ((void*)&smi);
 }

void
video_set_lut (unsigned int index,      /* color number */
                unsigned char r, /* red */
                unsigned char g, /* green */
                unsigned char b  /* blue */
		)
{

}
