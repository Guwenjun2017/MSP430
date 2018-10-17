/*
 * TFT.c
 *
 *  Created on: 2018年10月11日
 *      Author: Jack
 */
#include<msp430f169.h>
#include"TFT.h"
#include"type.h"
#include"delay.h"

/*“写命令/数据”操作的子函数*/
void write(uint argu)
{
    CS_clr;
    RD_set;

    DataPort = argu>>8;
    LE_set;                   //利用锁存器，节省IO口
    LE_clr;
    DataPort = argu;

    RW_clr;
    RW_set;
    CS_set;
}

/*TFT写命令，RS复位*/
void TFT_write_cmd(uint cmd)
{
    RS_clr;
    write(cmd);
}
/*TFT写数据，RS置位*/
void TFT_write_data(uint data)
{
    RS_set;
    write(data);
}

void TFT_write_cmd_data(uint reg, uint data)
{
    TFT_write_cmd(reg);
    TFT_write_data(data);
}
void TFT_address_set(uint xstart, uint ystart, uint xend, uint yend)
{
    TFT_write_cmd(0x0022); TFT_write_data(xstart);
    TFT_write_cmd(0x0021); TFT_write_data(ystart);
    TFT_write_cmd(0x0050); TFT_write_data(xstart);
    TFT_write_cmd(0x0051); TFT_write_data(xend);
    TFT_write_cmd(0x0052); TFT_write_data(ystart);
    TFT_write_cmd(0x0053); TFT_write_data(yend);
    TFT_write_cmd(0x0022);
}

void TFT_clear(uint color)
{
    uint i, j;
    TFT_address_set(0, 0, Lcd_Width-1, Lcd_Height-1);
    for(i=0; i<Lcd_Width; i++)
    {
        for(j=0; j<Lcd_Height; j++)
        {
            TFT_write_data(color);
        }
    }
}

void TFT_Fill(uint xstart, uint ystart, uint xend, uint yend, uint color)
{
    uint i,j;
    TFT_address_set(xstart, ystart, xend, yend);
    for(i=ystart; i<=yend; i++)
    {
        for(j=xstart; j<=xend; j++)
        {
            TFT_write_data(color);
        }
    }
}

/*画点，点的大小取决于坐标差值*/
void TFT_draw_point(uint x, uint y, uint color)
{
    TFT_address_set(x-2, y-2, x, y);
    TFT_write_data(color);
}
/*TFT彩屏初始化，只针对此型号彩屏，取决于其芯片*/
void TFT_init()
{
    RST_set;
    delay_ms(1);
    RST_clr;
    delay_ms(10);
    RST_set;
    delay_ms(50);

    TFT_write_cmd_data(0x0000,0x0001);        //Set the OSC bit as ‘1’ to start the internal oscillator
    TFT_write_cmd_data(0x0001,0x0100);      // set SS and SM bit
    TFT_write_cmd_data(0x0002,0x0700);        // set 1 line inversion
    TFT_write_cmd_data(0x0003,0x1098);        //set GRAM Write direction and BGR=1
    TFT_write_cmd_data(0x0004,0x0000);      // Resize register
    TFT_write_cmd_data(0x0008,0x0202);        // set the back porch and front porch
    TFT_write_cmd_data(0x0009,0x0000);      // set non-display area refresh cycle ISC[3:0]
    TFT_write_cmd_data(0x000A,0x0000);      // FMARK function
    TFT_write_cmd_data(0x000C,0x0000);      // RGB interface setting
    TFT_write_cmd_data(0x000D,0x0000);        // Frame marker Position
    TFT_write_cmd_data(0x000F,0x0000);      // RGB interface polarity
    delay_ms(30);
    //*************Power On sequence ****************//
    TFT_write_cmd_data(0x0010, 0x16b0);   // SAP, BT[3:0], AP, DSTB, SLP, STB
    delay_ms(30);
    TFT_write_cmd_data(0x0011, 0x0007);   //Write final user’s setting values to VC bit
    TFT_write_cmd_data(0x0012, 0x013a);   // set Internal reference voltage
    TFT_write_cmd_data(0x0013, 0x1a00);   // VDV[4:0] for VCOM amplitude
    delay_ms(30);
    TFT_write_cmd_data(0x0029, 0x000c);   // Set VCM[5:0] for VCOMH
    delay_ms(30); // Delay 50ms
    // ----------- Adjust the Gamma Curve ----------//
    TFT_write_cmd_data(0x0030, 0x0000);
    TFT_write_cmd_data(0x0031, 0x0505);
    TFT_write_cmd_data(0x0032, 0x0304);
    TFT_write_cmd_data(0x0035, 0x0006);
    TFT_write_cmd_data(0x0036, 0x0707);
    TFT_write_cmd_data(0x0037, 0x0105);
    TFT_write_cmd_data(0x0038, 0x0002);
    TFT_write_cmd_data(0x0039, 0x0707);
    TFT_write_cmd_data(0x003C, 0x0704);
    TFT_write_cmd_data(0x003D, 0x0807);

    //------------------ Set GRAM area ---------------//
    TFT_write_cmd_data(0x0050, 0x0000); // Horizontal GRAM Start Address
    TFT_write_cmd_data(0x0051, 0x00EF); // Horizontal GRAM End Address
    TFT_write_cmd_data(0x0052, 0x0000); // Vertical GRAM Start Address
    TFT_write_cmd_data(0x0053, 0x013F); // Vertical GRAM Start Address
    TFT_write_cmd_data(0x0060, 0x2700); // Gate Scan Line
    TFT_write_cmd_data(0x0061, 0x0001); // NDL,VLE, REV
    TFT_write_cmd_data(0x006A, 0x0000); // set scrolling line
    TFT_write_cmd_data(0x0020, 0x0000);   // GRAM horizontal Address
    TFT_write_cmd_data(0x0021, 0x0000);   // GRAM Vertical Address

    //-------------- Partial Display Control ---------//
    TFT_write_cmd_data(0x0080, 0x0000);
    TFT_write_cmd_data(0x0081, 0x0000);
    TFT_write_cmd_data(0x0082, 0x0000);
    TFT_write_cmd_data(0x0083, 0x0000);
    TFT_write_cmd_data(0x0084, 0x0000);
    TFT_write_cmd_data(0x0085, 0x0000);

    //-------------- Panel Control ---------//
    TFT_write_cmd_data(0x0090,0x0010);   //Frame Cycle Contral
    TFT_write_cmd_data(0x0092,0x0000);   //Panel Interface Contral
    TFT_write_cmd_data(0x0093,0x0003);   //Panel Interface Contral 3.
    TFT_write_cmd_data(0x0095,0x0110);   //Frame Cycle Contral
    TFT_write_cmd_data(0x0097,0x0000);   //
    TFT_write_cmd_data(0x0098,0x0000);   //Frame Cycle Contral.

    //-------------- Display on ---------//
    TFT_write_cmd_data(0x0007,0x0173);
}


