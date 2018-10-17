/****************************************************************************************************  
文件功能描述：320x240TFT驱动程序，控制TFT实现汉字,字符显示,画点功能。
****************************************************************************************************/
#include "config.h"
#ifndef u8
#define u8 unsigned char 
#endif

#ifndef u16
#define u16 unsigned int 
#endif
//**************声明外部函数和变量**************

extern unsigned int Device_code;

/****************************************************************************************************
//	实现功能：	延时
//	输入参数：	count 设置延时时间
****************************************************************************************************/
void delayms(unsigned int count)
{
    int i,j;                                                                                
    for(i=0;i<count;i++)                                                                    
       {
	     for(j=0;j<0x640;j++);
       }                                                                                     
}

/********************************************************************
//	实现功能：	写命令
//	输入参数：  DH 需要输入16bits命令的高8位
//              DL 需要输入16bits命令的低8位
********************************************************************/
void Write_Cmd(uchar DH,uchar DL)
{
  CS_CLR;
  RS_CLR;
  RD_SET;
  RW_CLR;

  //注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下
  DataPort=DH;              //送高8位命令给573待锁存
  LE_SET;                   //锁存位
  LE_CLR;                   //断开锁存,位选573的Q7~Q0仍保持 
  DataPort=DL;              //送低8位命令给TFT
  /*
  //如果使用16位数据口驱动16位数据模式，则无需IO扩展，直接将数据送到数据口即可
  DataPort_L=DL; 
  DataPort_H=DH;
  */

  RW_SET;
  CS_SET;
}

/********************************************************************
//	实现功能：	写数据(2*8bits)
//	输入参数：  DH 需要输入16bits数据的高8位
//              DL 需要输入16bits数据的低8位
********************************************************************/
void Write_Data(uchar DH,uchar DL)
{
  CS_CLR;
  RS_SET;
  //注意：当使用8位数据口驱动16位数据模式时，使用74HC573作为IO扩展，程序如下
  DataPort=DH;              //送高8位数据给573待锁存
  LE_SET;                   //锁存位
  LE_CLR;                   //断开锁存,位选573的Q7~Q0仍保持 
  DataPort=DL;              //送低8位数据给TFT

  /*
  //如果使用16位数据口驱动16位数据模式，则无需IO扩展，直接将数据送到数据口即可
  DataPort_L=DL; 
  DataPort_H=DH;
  */

  RW_CLR;
  RW_SET;
  CS_SET;
}

/****************************************************************************************************
//	实现功能：	写数据(16位)
//	输入参数：  y 需要输入16bits数据
****************************************************************************************************/
void  Write_Data_U16(unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	Write_Data(m,n);
}

/****************************************************************************************************
//	实现功能：	向x寄存器写入y数据
//	输入参数：  x 需要输入的命令 16位
//              y 需要输入的数据 16位
****************************************************************************************************/
void  Write_Cmd_Data (unsigned char x,unsigned int y)
{
	unsigned char m,n;
	m=y>>8;
	n=y;
	Write_Cmd(0x00,x);
	Write_Data(m,n);
}

/****************************************************************************************************
//	实现功能：	设置坐标
//  输入参数：  x0，y0 起始坐标
//              x1，y1 结束坐标
****************************************************************************************************/
void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1)
//void LCD_SetPos(int x0,int x1,int y0,int y1)
{

  Write_Cmd_Data(0x50,x0);  // Horizontal GRAM Start Address
  Write_Cmd_Data(0x51,x1);  // Horizontal GRAM End Address
  Write_Cmd_Data(0x52,y0);  // Vertical GRAM Start Address
  Write_Cmd_Data(0x53,y1);  // Vertical GRAM Start Address
  Write_Cmd_Data(0x20,x0);  // GRAM horizontal Address
  Write_Cmd_Data(0x21,y0);  // GRAM Vertical Address
  Write_Cmd (0x00,0x22);    // 0x0022,Start to Write Data to GRAM 
}

/****************************************************************************************************
//	实现功能：	TFT清屏
//	输入参数：	bColor 清屏所使用的背景色
****************************************************************************************************/
void CLR_Screen(unsigned int bColor)
{
 unsigned int i,j;
 LCD_SetPos(0,239,0,319);													//320x240，坐标从0开始
 for (i=0;i<240;i++)
	{
	   for (j=0;j<320;j++)
	       Write_Data_U16(bColor);
	}
}

/****************************************************************************************************
//	实现功能：	显示Ascii字符
//  输入参数：  x 横坐标
//              y 纵坐标
//		        c 需要显示的字符
//		        fColor 字符颜色
//		        bColor 字符背景颜色
****************************************************************************************************/
//void LCD_PutChar(unsigned int x, unsigned int y, char c, unsigned int fColor, unsigned int bColor)
//{
// unsigned int i,j;
// LCD_SetPos(x,x+16-1,y-8+1,y);                    //设置字符显示位置
// for(i=0; i<16;i++) {                             //循环写入16字节，一个字符为16字节
//		unsigned char m=Font8x16[(c-0x20)*16+i];  //提取c字符的第i个字节以,c减去0x20是由于Ascii码库中的0~1f被去掉
//		for(j=0;j<8;j++) {                        //循环写入8位，一个字节为8位
//			if((m&0x80)==0x80) {                  //判断最高位是否为1
//				Write_Data_U16(fColor);           //最高位为1，写入字符颜色
//				}
//			else {
//				Write_Data_U16(bColor);           //最高位为0，写入背景颜色
//				}
//			m<<=1;                                //左移1位，准备写下一位
//			}
//		}
//}
//
///****************************************************************************************************
////	实现功能：	显示单个Ascii字符，主要是为横屏而设置
////  输入参数： x 横坐标
////            y 纵坐标
////		        c 需要显示的字符
////		        fColor 字符颜色
////		        bColor 字符背景颜色
//****************************************************************************************************/
//void LCD_PutSingleChar(unsigned int x, unsigned int y, char c, unsigned int fColor, unsigned int bColor)
//{
// unsigned int i,j;
//
//	i=y;
//	j=x;
//	x=i;
//	y=319-j;
//
// LCD_SetPos(x,x+16-1,y-8+1,y);                    //设置字符显示位置
// for(i=0; i<16;i++) {                             //循环写入16字节，一个字符为16字节
//		unsigned char m=Font8x16[(c-0x20)*16+i];  		//提取c字符的第i个字节以,c减去0x20是由于Ascii码库中的0~1f被去掉
//		for(j=0;j<8;j++) {                        		//循环写入8位，一个字节为8位
//			if((m&0x80)==0x80) {                  			//判断最高位是否为1
//				Write_Data_U16(fColor);           				//最高位为1，写入字符颜色
//				}
//			else {
//				Write_Data_U16(bColor);           				//最高位为0，写入背景颜色
//				}
//			m<<=1;                                			//左移1位，准备写下一位
//			}
//		}
//}
//
///****************************************************************************************************
////	实现功能：	显示16x16汉字
////  输入参数： x 横坐标
////            y 纵坐标
////		        g 需要显示的字符编码
////		        fColor 字符颜色
////		        bColor 字符背景颜色
//****************************************************************************************************/
//
//void Put16x16(unsigned short x, unsigned short  y, unsigned char g[2], unsigned int fColor,unsigned int bColor)
//{
//	unsigned int i,j,k;
//
//	LCD_SetPos(x,  x+16-1,y-16+1, y);                       //设置汉字显示位置
//
//	for (k=0;k<64;k++)                                      //循环64次，查询汉字字模位置
//	{
//	  if ((ch16[k].GBK[0]==g[0])&&(ch16[k].GBK[1]==g[1]))   //判断第k个汉字的编码是否与输入汉字g[2]相等
//	  {
//    	for(i=0;i<32;i++)                                   //如果相等，既已找到待显示字模位置，循环写入32字节
//		{
//		  unsigned short m=ch16[k].hz16[i];                 	//读取32字节中的第i字节
//		  for(j=0;j<8;j++)                                  	//循环写入8位数据
//		   {
//			 if((m&0x80)==0x80) Write_Data_U16(fColor);     		//判断最高位是否为1,最高位为1，写入字符颜色
//			 else              Write_Data_U16(bColor);      		//最高位为0，写入背景颜色
//		     m<<=1;                                         	//左移1位，准备写下一位
//	       }
//		}
//	  }
//    }
//}
//
///****************************************************************************************************
//函数功能: 显示汉字24x24
//输入参数：x 横坐标
//         y 纵坐标
//         c 需要显示的汉字码
//	       fColor 字符颜色
//         bColor 字符背景颜色
//****************************************************************************************************/
//void PutGB2424(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int fColor,unsigned int bColor){
//	unsigned int i,j,k;
//
//	LCD_SetPos(x,  x+24-1,y-24+1, y);
//
//	for (k=0;k<12;k++) {															 //12表示自建汉字库中的个数，循环查询内码
//	  if ((codeGB_24[k].Index[0]==c[0])&&(codeGB_24[k].Index[1]==c[1])){
//    	for(i=0;i<72;i++) {
//		  unsigned short m=codeGB_24[k].Msk[i];
//		  for(j=0;j<8;j++) {
//			if((m&0x80)==0x80) {
//				Write_Data_U16(fColor);
//				}
//			else {
//				Write_Data_U16(bColor);
//				}
//			m<<=1;
//			}
//		  }
//		}
//	  }
//}
//
///****************************************************************************************************
////	实现功能：	显示中英文字符串
////  输入参数： x 横坐标
////            y 纵坐标
////		        *s 待显示的字符串,例如LCD_PutString(24,16,"123Eee",White,Blue);即把"123Eee"的第一个字符地址赋给指针变量s.
////		        bColor 字符背景颜色
//****************************************************************************************************/
//void LCD_PutString(unsigned short x, unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor)
//{
//	 unsigned char l=0;                            	//显示屏位置增量
//	 unsigned short i,j;
//
//	i=y;                                            //这里进行坐标变换，由竖屏变横屏需要变换
//	j=x;
//	x=i;
//	y=319-j;
//
//     while(*s)
//	 {
//		if( *s < 0x80)                             		//判断s指向的字符串中的某字符的编码值是否小于128,如果小于,即为ASCII字符
//		    {
//			 LCD_PutChar(x,y-l*8,*s,fColor,bColor);			//显示该字符
//		     s++;l++;                              		//指针加1,位置加1
//			}
//		else
//		    {
//			 Put16x16(x,y-l*8,(unsigned char*)s,fColor,bColor);		//显示该汉字
//		     s+=2;l+=2;                                        	//因为汉字为编码为2字节，指针加2,显示16x16所以位置加2
//			}
//	 }
//}
//
///****************************************************************************************************
//函数功能:显示字符串24x24大小,可以中英文同时显示
//输入参数：x 横坐标
//         y 纵坐标
//         *s 需要显示的字符串
//				 fColor 字符颜色
//         bColor 字符背景颜色
//****************************************************************************************************/
//void LCD_PutString24(unsigned short x , unsigned short y, unsigned char *s, unsigned int fColor, unsigned int bColor) {
//	 unsigned char l=0;
//	 unsigned short i,j;
//	i=y;																											//坐标变换
//	j=x;
//	x=i;
//	y=319-j;
//	while(*s) {
//		if( *s < 0x80)
//		    {
//			LCD_PutChar(x,y-l*8,*s,fColor,bColor);
//			s++;l++;
//			}
//		else
//		    {
//			PutGB2424(x,y-l*12,(unsigned char*)s,fColor,bColor);
//			s+=2;l+=2;
//			}
//		}
//	}
//
///****************************************************************************************************
////	实现功能：	指定位置显示RGB颜色
////  输入参数： x0，y0 起始坐标
////            x1，y1 结束坐标
////		        Color  背景颜色
//****************************************************************************************************/

void Show_RGB (unsigned short x0,unsigned short x1,unsigned short y0,unsigned short y1,unsigned int Color)
{
	unsigned int i,j;
	
	LCD_SetPos(y0,y1,319-x1,319-x0);//横屏这里要进行坐标变换，如果是竖屏，则直接是x0,x1,y0,y1做参数
	for (i=x0;i<=x1;i++)
	{
	   for (j=y0;j<=y1;j++)
	       Write_Data_U16(Color);
	}
}

/****************************************************************************************************
//	实现功能：	TFT初始化
****************************************************************************************************/
void TFT_Initial(void)
{      
	RST_SET;    
	delayms(1);                    			// Delay 1ms 
	RST_CLR;  
	delayms(10);                   			// Delay 10ms            
	RST_SET;  
	delayms(50);                   			// Delay 50 ms  
  
	if(Device_code==0x9320)
     {

    //************* Start Initial Sequence **********//
	Write_Cmd_Data(0x00,0x0001);   		//Set the OSC bit as ‘1’ to start the internal oscillator
  Write_Cmd_Data(0x01,0x0100);   	// set SS and SM bit
	Write_Cmd_Data(0x02,0x0700);   		// set 1 line inversion
	Write_Cmd_Data(0x03,0x1098);   		//set GRAM Write direction and BGR=1
  Write_Cmd_Data(0x04,0x0000);   	// Resize register
	Write_Cmd_Data(0x08,0x0202);   		// set the back porch and front porch
  Write_Cmd_Data(0x09,0x0000);   	// set non-display area refresh cycle ISC[3:0]
  Write_Cmd_Data(0x0A,0x0000);   	// FMARK function
  Write_Cmd_Data(0x0C,0x0000);   	// RGB interface setting
	Write_Cmd_Data(0x0D,0x0000);   		// Frame marker Position
  Write_Cmd_Data(0x0F,0x0000);   	// RGB interface polarity
  delayms(30);
	//*************Power On sequence ****************//
	Write_Cmd_Data(0x10, 0x16b0);   // SAP, BT[3:0], AP, DSTB, SLP, STB
	delayms(30);
	Write_Cmd_Data(0x11, 0x0007);   //Write final user’s setting values to VC bit
	Write_Cmd_Data(0x12, 0x013a);   // set Internal reference voltage
	Write_Cmd_Data(0x13, 0x1a00);   // VDV[4:0] for VCOM amplitude
  delayms(30);
  Write_Cmd_Data(0x29, 0x000c);   // Set VCM[5:0] for VCOMH
	delayms(30); // Delay 50ms

	// ----------- Adjust the Gamma Curve ----------//
	Write_Cmd_Data(0x0030, 0x0000);
	Write_Cmd_Data(0x0031, 0x0505);
	Write_Cmd_Data(0x0032, 0x0304);
	Write_Cmd_Data(0x0035, 0x0006);
	Write_Cmd_Data(0x0036, 0x0707);
	Write_Cmd_Data(0x0037, 0x0105);
	Write_Cmd_Data(0x0038, 0x0002);
	Write_Cmd_Data(0x0039, 0x0707);
	Write_Cmd_Data(0x003C, 0x0704);
	Write_Cmd_Data(0x003D, 0x0807);

	//------------------ Set GRAM area ---------------//
	Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
	Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
	Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0060, 0x2700); // Gate Scan Line
	Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV
	Write_Cmd_Data(0x006A, 0x0000); // set scrolling line
  Write_Cmd_Data(0x20, 0x0000);   // GRAM horizontal Address
	Write_Cmd_Data(0x21, 0x0000);   // GRAM Vertical Address

	//-------------- Partial Display Control ---------//
	Write_Cmd_Data(0x0080, 0x0000);
	Write_Cmd_Data(0x0081, 0x0000);
	Write_Cmd_Data(0x0082, 0x0000);
	Write_Cmd_Data(0x0083, 0x0000);
	Write_Cmd_Data(0x0084, 0x0000);
	Write_Cmd_Data(0x0085, 0x0000);

	//-------------- Panel Control ---------//
  Write_Cmd_Data(0x90,0x0010);   //Frame Cycle Contral
	Write_Cmd_Data(0x92,0x0000);   //Panel Interface Contral
	Write_Cmd_Data(0x93,0x0003);   //Panel Interface Contral 3. 
	Write_Cmd_Data(0x95,0x0110);   //Frame Cycle Contral
	Write_Cmd_Data(0x97,0x0000);   // 
	Write_Cmd_Data(0x98,0x0000);   //Frame Cycle Contral.     

	//-------------- Display on ---------//
  Write_Cmd_Data(0x07,0x0173); 

	}

	else if(Device_code==0x1505 )
     {

    //************* Start Initial Sequence **********//
	Write_Cmd_Data(0x00,0x0001);   //Set the OSC bit as ‘1’ to start the internal oscillator
  Write_Cmd_Data(0x01,0x0100);   // set SS and SM bit
	Write_Cmd_Data(0x02,0x0700);   // set 1 line inversion
	Write_Cmd_Data(0x03,0x1030);   //set GRAM Write direction and BGR=1
  Write_Cmd_Data(0x04,0x0000);   // Resize register
	Write_Cmd_Data(0x08,0x0202);   // set the back porch and front porch
  Write_Cmd_Data(0x09,0x0000);   // set non-display area refresh cycle ISC[3:0]
  Write_Cmd_Data(0x0A,0x0000);   // FMARK function
  Write_Cmd_Data(0x0C,0x0000);   // RGB interface setting
	Write_Cmd_Data(0x0D,0x0000);   // Frame marker Position
  Write_Cmd_Data(0x0F,0x0000);   // RGB interface polarity
  delayms(30);
	//*************Power On sequence ****************//
	Write_Cmd_Data(0x10, 0x16b0);   // SAP, BT[3:0], AP, DSTB, SLP, STB
	delayms(30);
	Write_Cmd_Data(0x11, 0x0007);   //Write final user’s setting values to VC bit
	Write_Cmd_Data(0x12, 0x013a);   // set Internal reference voltage
	Write_Cmd_Data(0x13, 0x1a00);   // VDV[4:0] for VCOM amplitude
  delayms(30);
  Write_Cmd_Data(0x29, 0x000c);   // Set VCM[5:0] for VCOMH
	delayms(30); // Delay 50ms

	// ----------- Adjust the Gamma Curve ----------//
	Write_Cmd_Data(0x0030, 0x0000);
	Write_Cmd_Data(0x0031, 0x0505);
	Write_Cmd_Data(0x0032, 0x0304);
	Write_Cmd_Data(0x0035, 0x0006);
	Write_Cmd_Data(0x0036, 0x0707);
	Write_Cmd_Data(0x0037, 0x0105);
	Write_Cmd_Data(0x0038, 0x0002);
	Write_Cmd_Data(0x0039, 0x0707);
	Write_Cmd_Data(0x003C, 0x0704);
	Write_Cmd_Data(0x003D, 0x0807);

	//------------------ Set GRAM area ---------------//
	Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
	Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
	Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM Start Address
	Write_Cmd_Data(0x0060, 0x2700); // Gate Scan Line
	Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV
	Write_Cmd_Data(0x006A, 0x2700); // set scrolling line
  Write_Cmd_Data(0x20, 0x0000);   // GRAM horizontal Address
	Write_Cmd_Data(0x21, 0x0000);   // GRAM Vertical Address

	//-------------- Partial Display Control ---------//
	Write_Cmd_Data(0x0080, 0x0000);
	Write_Cmd_Data(0x0081, 0x0000);
	Write_Cmd_Data(0x0082, 0x0000);
	Write_Cmd_Data(0x0083, 0x0000);
	Write_Cmd_Data(0x0084, 0x0000);
	Write_Cmd_Data(0x0085, 0x0000);

	//-------------- Panel Control ---------//
  Write_Cmd_Data(0x90,0x0010);   //Frame Cycle Contral
	Write_Cmd_Data(0x92,0x0000);   //Panel Interface Contral
	Write_Cmd_Data(0x93,0x0003);   //Panel Interface Contral 3. 
	Write_Cmd_Data(0x95,0x0110);   //Frame Cycle Contral
	Write_Cmd_Data(0x97,0x0000);   // 
	Write_Cmd_Data(0x98,0x0000);   //Frame Cycle Contral.     

	//-------------- Display on ---------//
  Write_Cmd_Data(0x07,0x0173); 

	}

  else if(Device_code==0x9328)
     {

    //************* Start Initial Sequence **********//
	 Write_Cmd_Data(0x0001,0x0100);   //set SS and SM bit //设置扫描方向  100
   Write_Cmd_Data(0x0002,0x0700);   //EOR=1 and B/C=1 to set the line inversion  //设置行反转
   Write_Cmd_Data(0x0003,0x1098);   //set Entry Mode  //设置进入模式    1030
   Write_Cmd_Data(0x0004,0x0000);   //
   Write_Cmd_Data(0x00A4,0x0001);
   Write_Cmd_Data(0x0008,0x0202); // set the back porch and front porch
   Write_Cmd_Data(0x0009,0x0000); // set non-display area refresh cycle ISC[3:0]
   Write_Cmd_Data(0x000A,0x0000); // FMARK function
   Write_Cmd_Data(0x000C,0x0000); // RGB interface setting
   Write_Cmd_Data(0x000D, 0x0000); // Frame marker Position
   Write_Cmd_Data(0x000F, 0x0000); // RGB interface polarity



//*************Power On sequence ****************//
    Write_Cmd_Data(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB 
    Write_Cmd_Data(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
    Write_Cmd_Data(0x0012, 0x0000); // VREG1OUT voltage
    Write_Cmd_Data(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude 
    delayms(30);
    Write_Cmd_Data(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB 
    Write_Cmd_Data(0x0011, 0x0227); // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
    delayms(30);
    Write_Cmd_Data(0x0012, 0x001C); // External reference voltage= Vci;
    delayms(30); 
    Write_Cmd_Data(0x0013, 0x1800); // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
    Write_Cmd_Data(0x0029, 0x001C); // R29=000C when R12=009D;VCM[5:0] for VCOMH
    Write_Cmd_Data(0x002B, 0x000D); // Frame Rate = 91Hz
    delayms(30);    
    Write_Cmd_Data(0x0020, 0x0000); // GRAM horizontal Address
    Write_Cmd_Data(0x0021, 0x0000); // GRAM Vertical Address
// ----------- Adjust the Gamma Curve ----------// 		 
	  Write_Cmd_Data(0x0030, 0x0007);
	  Write_Cmd_Data(0x0031, 0x0302);
    Write_Cmd_Data(0x0032, 0x0105);
	  Write_Cmd_Data(0x0035, 0x0206);
    Write_Cmd_Data(0x0036, 0x0808);	          
    Write_Cmd_Data(0x0037, 0x0206);
    Write_Cmd_Data(0x0038, 0x0504);
    Write_Cmd_Data(0x0039, 0x0007);
    Write_Cmd_Data(0x003C, 0x0105);
    Write_Cmd_Data(0x003D, 0x0808);
//------------------ Set GRAM area ---------------//
    Write_Cmd_Data(0x0050, 0x0000); // Horizontal GRAM Start Address
    Write_Cmd_Data(0x0051, 0x00EF); // Horizontal GRAM End Address
    Write_Cmd_Data(0x0052, 0x0000); // Vertical GRAM Start Address
	  delayms(30);
    Write_Cmd_Data(0x0053, 0x013F); // Vertical GRAM End Address
	  delayms(30);
    Write_Cmd_Data(0x0060, 0xA700); // Gate Scan Line
    Write_Cmd_Data(0x0061, 0x0001); // NDL,VLE, REV  
    Write_Cmd_Data(0x006A, 0x0000); // set scrolling line
//-------------- Partial Display Control ---------//
    Write_Cmd_Data(0x0080, 0x0000);
    Write_Cmd_Data(0x0081, 0x0000);
    Write_Cmd_Data(0x0082,0x0000); 
    Write_Cmd_Data(0x0083,0x0000); 
    Write_Cmd_Data(0x0084,0x0000); 
    Write_Cmd_Data(0x0085,0x0000); 
//-------------- Panel Control -------------------//        
    Write_Cmd_Data(0x0090, 0x0010);
    Write_Cmd_Data(0x0092, 0x0000);
    Write_Cmd_Data(0x0093, 0x0003);
    Write_Cmd_Data(0x0095, 0x0110);
    Write_Cmd_Data(0x0097, 0x0000);
    Write_Cmd_Data(0x0098, 0x0000);
    Write_Cmd_Data(0x0007, 0x0133); // 262K color and display ON

	}
}

/****************************************************************************************************
//	实现功能：	画点
//  输入参数： x，y 需要画点坐标
//            color 点的颜色
****************************************************************************************************/
void Put_pixel(unsigned int x,unsigned int y,unsigned int color)
{
	LCD_SetPos(y,y,319-x,319-x);												//横屏这里要进行坐标变换，如果是竖屏，则直接是x0,x1,y0,y1做参数

	Write_Data_U16(color);      												//在指定点写入颜色数据
}



