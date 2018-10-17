/****************************************************************************************************  
文件功能描述：GUI绘图函数集，可以根据自己的需求添加绘图函数，已有画点、实线、虚线、矩形填充、任意大小圆形函数
****************************************************************************************************/

#ifndef uchar
#define uchar unsigned char 
#endif

#ifndef uint
#define uint unsigned int 
#endif

#include "math.h"

#define PI    3.1415926                        //计算坐标用到的PI

uchar Taby[][7]={"0.0","0.5","1.0","1.5","2.0","2.5","3.0"} ; //y刻度值，可自由定义
uchar Tabx[][9]={"0","1","2","3","4","5","6","7","8","9"} ;   //x刻度值，可自由定义

void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1);
void  Write_Data_U16(unsigned int y);

/****************************************************************************************************
//  实现功能：	画点
//  输入参数： x，y 需要画点坐标
//            color 点的颜色
****************************************************************************************************/
void GUIpoint(unsigned int x,unsigned int y,unsigned int color)
{
	LCD_SetPos(y,y,319-x,319-x);	//横屏这里要进行坐标变换，如果是竖屏，则直接是x0,x1,y0,y1做参数

	Write_Data_U16(color);      	//在指定点写入颜色数据
}

//***********************************************************************
//         大屏GUI画线函数，（x0,y0）为起始坐标,（x1,y1）为终点坐标
//		   line_color 线的颜色
//***********************************************************************

void GUIline(uint x0,uint y0,uint x1,uint y1,uint line_color)   //画线函数
{
    int temp; 
    int dx,dy;               			//定义起点到终点的横、纵坐标增加值 
    int s1,s2,status,i; 
    int Dx,Dy,sub; 
	
	  dx = x1 - x0; 
    if(dx >= 0)                 	//X的方向是增加的 
        s1 = 1; 
    else                     			//X的方向是降低的 
        s1 = -1;      
    dy = y1 - y0;               	//判断Y的方向是增加还是降到的 
    if(dy >= 0) 
        s2 = 1; 
    else 
        s2 =- 1; 
   
    Dx = abs(x1-x0);             	//计算横、纵标志增加值的绝对值 
    Dy = abs(y1-y0); 
        
    if(Dy > Dx)                               
    {                     	        //以45度角为分界线，靠进Y轴是status=1,靠近X轴是status=0  
        temp = Dx; 
        Dx = Dy; 
        Dy = temp; 
    status = 1; 
    }  
    else 
        status = 0; 

/*********Bresenham算法画任意两点间的直线********/  
    sub = Dy + Dy - Dx;                 //第1次判断下个点的位置 
    for(i = 0;i < Dx;i ++) 
    {  
        GUIpoint(x0,y0,line_color);           //画点  
        if(sub >= 0)                                
        {  
            if(status == 1)               //在靠近Y轴区，x值加1 
                x0 += s1;  
            else                     //在靠近X轴区，y值加1                
                y0 += s2;  
            sub -= (Dx + Dx);                 //判断下下个点的位置  
        }  
        if(status == 1) 
            y0 += s2;  
        else        
            x0 += s1;  
        sub += Dy + Dy;  
         
    }  
} 

//***********************************************************************
//         大屏GUI画虚线函数，（x0,y0）为起始坐标,（x1,y1）为终点坐标
//		   line_color 线的颜色
//***********************************************************************

void GUIDotline(uint x0,uint y0,uint x1,uint y1,uint line_color)   //画虚线函数
{
    int temp; 
    int dx,dy;               	//定义起点到终点的横、纵坐标增加值 
    int s1,s2,status,i; 
    int Dx,Dy,sub; 
	
	  dx = x1 - x0; 
    if(dx >= 0)                 //X的方向是增加的 
        s1 = 1; 
    else                     	  //X的方向是降低的 
        s1 = -1;      
    dy = y1 - y0;               //判断Y的方向是增加还是降到的 
    if(dy >= 0) 
        s2 = 1; 
    else 
        s2 =- 1; 
   
    Dx = abs(x1-x0);             //计算横、纵标志增加值的绝对值 
    Dy = abs(y1-y0); 
        
    if(Dy > Dx)                               
    {                     				//以45度角为分界线，靠进Y轴是status=1,靠近X轴是status=0  
        temp = Dx; 
        Dx = Dy; 
        Dy = temp; 
    status = 1; 
    }  
    else 
        status = 0; 

/*********Bresenham算法画任意两点间的直线********/  
    sub = Dy + Dy - Dx;                		         //第1次判断下个点的位置 
    for(i = 0;i < Dx;i ++) 
    {  
        if((i%10)<=4) GUIpoint(x0,y0,line_color);       //画点，虚线间距大小可以在这里调节，设置不同的判断条件 
        if(sub >= 0)                                
        {  
            if(status == 1)               	        //在靠近Y轴区，x值加1 
                x0 += s1;  
            else                     			//在靠近X轴区，y值加1                
                y0 += s2;  
            sub -= (Dx + Dx);                 		//判断下下个点的位置  
        }  
        if(status == 1) 
            y0 += s2;  
        else        
            x0 += s1;  
        sub += Dy + Dy;  
				
         
    }  
} 

/**************************************************************************** 
* 名称：GUI_Full(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char full_colour) 
* 功能：用绘图的方法填充一个矩形 
* 入口参数：x0 			起始点横坐标 
*          y0    		起始点纵坐标 
*          x1 			终止点横坐标 
*          y1      	终止点纵坐标 
*          full_color      	显示颜色 
* 出口参数：无 
****************************************************************************/ 
void GUIfull(uint x0,uint y0,uint x1,uint y1,uint full_colour) 
{ 
    uint i,j; 
for(j = y0;j <= y1;j ++) 
    for(i = x0;i <= x1;i ++) 
    GUIpoint(i,j,full_colour); 
} 


/**************************************************************************** 
* 名称：plotC(int x,int y,int xc,int yc,unsigned char yc_colour) 
* 功能：八分点画圆函数 
* 入口参数：x    		指定线起点所在行的位置 
*           y    		指定线起点所在列的位置 
*          yc_color 	显示颜色,根据彩色代码设定 
* 出口参数：无 
****************************************************************************/ 
void plotC(uint x,uint y,uint xc,int yc,uint yc_colour) 
{ 
    GUIpoint(xc+x,yc+y,yc_colour); 
    GUIpoint(xc+x,yc-y,yc_colour); 
    GUIpoint(xc-x,yc+y,yc_colour); 
    GUIpoint(xc-x,yc-y,yc_colour); 
    GUIpoint(xc+y,yc+x,yc_colour); 
    GUIpoint(xc+y,yc-x,yc_colour); 
    GUIpoint(xc-y,yc+x,yc_colour); 
    GUIpoint(xc-y,yc-x,yc_colour); 
} 

/**************************************************************************** 
* 名称：GUI_Circle(int xc,int yc,int r,unsigned char circle_colour) 
* 功能：画圆 
* 入口参数：xc 				圆心的行坐标 
*           yc    		圆心的列坐标 
*           r 				半径 
*          circle_color     显示颜色 
* 出口参数：无 
****************************************************************************/ 
void GUIcircle(uint xc,uint yc,uint r,uint circle_colour) 
{ 
    int x,y,d; 
    y = r; 
    d = 3 - (r + r); 
    x = 0; 
    while(x <= y) 
    { 
        plotC(x,y,xc,yc,circle_colour); 
        if(d < 0) 
            d += (x + x + x + x) + 6; 
        else 
        { 
            d+=((x - y) + (x - y) + (x - y) + (x - y)) + 10; 
            y = y - 1; 
        } 
        x = x + 1; 
    } 
}

/**************************************************************************** 
* 名称：GUITable(u16 x0,u16 y0,u16 x1,u16 y1,uchar TH,uchar TL,u16 colour) 
* 功能：绘制任意行列的表格
* 入口参数：x0 		起始点横坐标 
*          y0    	起始点纵坐标 
*          x1 		终止点横坐标 
*          y1      	终止点纵坐标
*          TH           表格行数
*          TL           表格列数
*          color        表格边框颜色 
* 出口参数：无 
****************************************************************************/ 
void GUITable(uint x0,uint y0,uint x1,uint y1,uchar TH,uchar TL,uint color) 
{ 
  uchar i;
  
  for(i=0;i<=TH;i++)
  {
    GUIline(x0,y0+i*(y1-y0)/TH,x1,y0+i*(y1-y0)/TH,color);
  }
  
  for(i=0;i<=TL;i++)
  {
    GUIline(x0+i*(x1-x0)/TL,y0,x0+i*(x1-x0)/TL,y1,color);
  }
   
} 

/**************************************************************************** 
* 名称：GUICoordinate(u16 x0,u16 y0,u16 xm,u16 ym,uchar pix,uint ccolor,u16 bcolor) 
* 功能：绘制一个坐标轴，以用于ADC显示电压为参考，可自行更改
* 入口参数：x0 		原点横坐标 
*           y0    	原点纵坐标 
*           xm 		横坐标长度
*           ym      	纵坐标长度
*           pix         坐标轴像素值大小，如1,2,3……
*           bcolor      背景颜色 
*           ccolor      字符颜色
* 出口参数：无 
****************************************************************************/ 
void GUICoordinate(uint x0,uint y0,uint xm,uint ym,uchar pix,uint ccolor,uint bcolor) 
{ 
  uchar i,xj,yj;
  
  for(i=0;i<pix;i++)
  {
    GUIline(x0-i,y0,x0-i,y0-ym,ccolor);                     //画Y轴
  } 
  
  for(i=0;i<pix;i++)
  {
    GUIline(x0-pix+1,y0+i,x0+xm,y0+i,ccolor);               //画X轴
  }
  
  GUIline(x0,y0-ym,x0-10*cos(1*PI/3),y0-ym+10*sin(1*PI/3),ccolor);  //绘制各轴箭头，10个像素长，大小可更改里面的10这个值
  GUIline(x0,y0-ym,x0+10*cos(1*PI/3),y0-ym+10*sin(1*PI/3),ccolor);
  LCD_PutString(x0+10*cos(1*PI/3),y0-ym+10*sin(1*PI/3),"Voltage(V)",ccolor,bcolor);      //显示Y轴参数定义，如电压V
  
  GUIline(x0+xm,y0,x0+xm-10*sin(1*PI/3),y0+10*cos(1*PI/3),ccolor);
  GUIline(x0+xm,y0,x0+xm-10*sin(1*PI/3),y0-10*cos(1*PI/3),ccolor);
  LCD_PutString(x0+xm-10*sin(1*PI/3)-32,y0-10*cos(1*PI/3)-16,"T(t)",ccolor,bcolor);      //显示X轴参数定义，如时间T
  
  
  xj=10;  //横坐标10个刻度
  yj=6;   //纵坐标5个刻度线，共6段
  
  for(i=1;i<yj;i++)
  {
    GUIline(x0,y0-i*ym/yj,x0-5,y0-i*ym/yj,ccolor);    //绘制刻度，Y轴，等分绘制
    
    LCD_PutString(x0-5-24,y0-i*ym/yj-8,Taby[i],ccolor,bcolor);  //显示刻度参数值，定义在Tab表里
  }
  
  for(i=1;i<xj;i++)
  {
    GUIline(x0+i*xm/xj,y0,x0+i*xm/xj,y0+5,ccolor);   //绘制刻度，X轴，等分绘制
    LCD_PutString(x0+i*xm/xj-4,y0+5,Tabx[i],ccolor,bcolor); //显示刻度参数值，定义在Tab表里
  }
  
  LCD_PutString(x0-12,y0+4,"O",ccolor,bcolor);      //显示原点0
} 