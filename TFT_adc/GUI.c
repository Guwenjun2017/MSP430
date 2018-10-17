/****************************************************************************************************  
�ļ�����������GUI��ͼ�����������Ը����Լ���������ӻ�ͼ���������л��㡢ʵ�ߡ����ߡ�������䡢�����СԲ�κ���
****************************************************************************************************/

#ifndef uchar
#define uchar unsigned char 
#endif

#ifndef uint
#define uint unsigned int 
#endif

#include "math.h"

#define PI    3.1415926                        //���������õ���PI

uchar Taby[][7]={"0.0","0.5","1.0","1.5","2.0","2.5","3.0"} ; //y�̶�ֵ�������ɶ���
uchar Tabx[][9]={"0","1","2","3","4","5","6","7","8","9"} ;   //x�̶�ֵ�������ɶ���

void LCD_SetPos(unsigned int x0,unsigned int x1,unsigned int y0,unsigned int y1);
void  Write_Data_U16(unsigned int y);

/****************************************************************************************************
//  ʵ�ֹ��ܣ�	����
//  ��������� x��y ��Ҫ��������
//            color �����ɫ
****************************************************************************************************/
void GUIpoint(unsigned int x,unsigned int y,unsigned int color)
{
	LCD_SetPos(y,y,319-x,319-x);	//��������Ҫ��������任���������������ֱ����x0,x1,y0,y1������

	Write_Data_U16(color);      	//��ָ����д����ɫ����
}

//***********************************************************************
//         ����GUI���ߺ�������x0,y0��Ϊ��ʼ����,��x1,y1��Ϊ�յ�����
//		   line_color �ߵ���ɫ
//***********************************************************************

void GUIline(uint x0,uint y0,uint x1,uint y1,uint line_color)   //���ߺ���
{
    int temp; 
    int dx,dy;               			//������㵽�յ�ĺᡢ����������ֵ 
    int s1,s2,status,i; 
    int Dx,Dy,sub; 
	
	  dx = x1 - x0; 
    if(dx >= 0)                 	//X�ķ��������ӵ� 
        s1 = 1; 
    else                     			//X�ķ����ǽ��͵� 
        s1 = -1;      
    dy = y1 - y0;               	//�ж�Y�ķ��������ӻ��ǽ����� 
    if(dy >= 0) 
        s2 = 1; 
    else 
        s2 =- 1; 
   
    Dx = abs(x1-x0);             	//����ᡢ�ݱ�־����ֵ�ľ���ֵ 
    Dy = abs(y1-y0); 
        
    if(Dy > Dx)                               
    {                     	        //��45�Ƚ�Ϊ�ֽ��ߣ�����Y����status=1,����X����status=0  
        temp = Dx; 
        Dx = Dy; 
        Dy = temp; 
    status = 1; 
    }  
    else 
        status = 0; 

/*********Bresenham�㷨������������ֱ��********/  
    sub = Dy + Dy - Dx;                 //��1���ж��¸����λ�� 
    for(i = 0;i < Dx;i ++) 
    {  
        GUIpoint(x0,y0,line_color);           //����  
        if(sub >= 0)                                
        {  
            if(status == 1)               //�ڿ���Y������xֵ��1 
                x0 += s1;  
            else                     //�ڿ���X������yֵ��1                
                y0 += s2;  
            sub -= (Dx + Dx);                 //�ж����¸����λ��  
        }  
        if(status == 1) 
            y0 += s2;  
        else        
            x0 += s1;  
        sub += Dy + Dy;  
         
    }  
} 

//***********************************************************************
//         ����GUI�����ߺ�������x0,y0��Ϊ��ʼ����,��x1,y1��Ϊ�յ�����
//		   line_color �ߵ���ɫ
//***********************************************************************

void GUIDotline(uint x0,uint y0,uint x1,uint y1,uint line_color)   //�����ߺ���
{
    int temp; 
    int dx,dy;               	//������㵽�յ�ĺᡢ����������ֵ 
    int s1,s2,status,i; 
    int Dx,Dy,sub; 
	
	  dx = x1 - x0; 
    if(dx >= 0)                 //X�ķ��������ӵ� 
        s1 = 1; 
    else                     	  //X�ķ����ǽ��͵� 
        s1 = -1;      
    dy = y1 - y0;               //�ж�Y�ķ��������ӻ��ǽ����� 
    if(dy >= 0) 
        s2 = 1; 
    else 
        s2 =- 1; 
   
    Dx = abs(x1-x0);             //����ᡢ�ݱ�־����ֵ�ľ���ֵ 
    Dy = abs(y1-y0); 
        
    if(Dy > Dx)                               
    {                     				//��45�Ƚ�Ϊ�ֽ��ߣ�����Y����status=1,����X����status=0  
        temp = Dx; 
        Dx = Dy; 
        Dy = temp; 
    status = 1; 
    }  
    else 
        status = 0; 

/*********Bresenham�㷨������������ֱ��********/  
    sub = Dy + Dy - Dx;                		         //��1���ж��¸����λ�� 
    for(i = 0;i < Dx;i ++) 
    {  
        if((i%10)<=4) GUIpoint(x0,y0,line_color);       //���㣬���߼���С������������ڣ����ò�ͬ���ж����� 
        if(sub >= 0)                                
        {  
            if(status == 1)               	        //�ڿ���Y������xֵ��1 
                x0 += s1;  
            else                     			//�ڿ���X������yֵ��1                
                y0 += s2;  
            sub -= (Dx + Dx);                 		//�ж����¸����λ��  
        }  
        if(status == 1) 
            y0 += s2;  
        else        
            x0 += s1;  
        sub += Dy + Dy;  
				
         
    }  
} 

/**************************************************************************** 
* ���ƣ�GUI_Full(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char full_colour) 
* ���ܣ��û�ͼ�ķ������һ������ 
* ��ڲ�����x0 			��ʼ������� 
*          y0    		��ʼ�������� 
*          x1 			��ֹ������� 
*          y1      	��ֹ�������� 
*          full_color      	��ʾ��ɫ 
* ���ڲ������� 
****************************************************************************/ 
void GUIfull(uint x0,uint y0,uint x1,uint y1,uint full_colour) 
{ 
    uint i,j; 
for(j = y0;j <= y1;j ++) 
    for(i = x0;i <= x1;i ++) 
    GUIpoint(i,j,full_colour); 
} 


/**************************************************************************** 
* ���ƣ�plotC(int x,int y,int xc,int yc,unsigned char yc_colour) 
* ���ܣ��˷ֵ㻭Բ���� 
* ��ڲ�����x    		ָ������������е�λ�� 
*           y    		ָ������������е�λ�� 
*          yc_color 	��ʾ��ɫ,���ݲ�ɫ�����趨 
* ���ڲ������� 
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
* ���ƣ�GUI_Circle(int xc,int yc,int r,unsigned char circle_colour) 
* ���ܣ���Բ 
* ��ڲ�����xc 				Բ�ĵ������� 
*           yc    		Բ�ĵ������� 
*           r 				�뾶 
*          circle_color     ��ʾ��ɫ 
* ���ڲ������� 
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
* ���ƣ�GUITable(u16 x0,u16 y0,u16 x1,u16 y1,uchar TH,uchar TL,u16 colour) 
* ���ܣ������������еı��
* ��ڲ�����x0 		��ʼ������� 
*          y0    	��ʼ�������� 
*          x1 		��ֹ������� 
*          y1      	��ֹ��������
*          TH           �������
*          TL           �������
*          color        ���߿���ɫ 
* ���ڲ������� 
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
* ���ƣ�GUICoordinate(u16 x0,u16 y0,u16 xm,u16 ym,uchar pix,uint ccolor,u16 bcolor) 
* ���ܣ�����һ�������ᣬ������ADC��ʾ��ѹΪ�ο��������и���
* ��ڲ�����x0 		ԭ������� 
*           y0    	ԭ�������� 
*           xm 		�����곤��
*           ym      	�����곤��
*           pix         ����������ֵ��С����1,2,3����
*           bcolor      ������ɫ 
*           ccolor      �ַ���ɫ
* ���ڲ������� 
****************************************************************************/ 
void GUICoordinate(uint x0,uint y0,uint xm,uint ym,uchar pix,uint ccolor,uint bcolor) 
{ 
  uchar i,xj,yj;
  
  for(i=0;i<pix;i++)
  {
    GUIline(x0-i,y0,x0-i,y0-ym,ccolor);                     //��Y��
  } 
  
  for(i=0;i<pix;i++)
  {
    GUIline(x0-pix+1,y0+i,x0+xm,y0+i,ccolor);               //��X��
  }
  
  GUIline(x0,y0-ym,x0-10*cos(1*PI/3),y0-ym+10*sin(1*PI/3),ccolor);  //���Ƹ����ͷ��10�����س�����С�ɸ��������10���ֵ
  GUIline(x0,y0-ym,x0+10*cos(1*PI/3),y0-ym+10*sin(1*PI/3),ccolor);
  LCD_PutString(x0+10*cos(1*PI/3),y0-ym+10*sin(1*PI/3),"Voltage(V)",ccolor,bcolor);      //��ʾY��������壬���ѹV
  
  GUIline(x0+xm,y0,x0+xm-10*sin(1*PI/3),y0+10*cos(1*PI/3),ccolor);
  GUIline(x0+xm,y0,x0+xm-10*sin(1*PI/3),y0-10*cos(1*PI/3),ccolor);
  LCD_PutString(x0+xm-10*sin(1*PI/3)-32,y0-10*cos(1*PI/3)-16,"T(t)",ccolor,bcolor);      //��ʾX��������壬��ʱ��T
  
  
  xj=10;  //������10���̶�
  yj=6;   //������5���̶��ߣ���6��
  
  for(i=1;i<yj;i++)
  {
    GUIline(x0,y0-i*ym/yj,x0-5,y0-i*ym/yj,ccolor);    //���ƿ̶ȣ�Y�ᣬ�ȷֻ���
    
    LCD_PutString(x0-5-24,y0-i*ym/yj-8,Taby[i],ccolor,bcolor);  //��ʾ�̶Ȳ���ֵ��������Tab����
  }
  
  for(i=1;i<xj;i++)
  {
    GUIline(x0+i*xm/xj,y0,x0+i*xm/xj,y0+5,ccolor);   //���ƿ̶ȣ�X�ᣬ�ȷֻ���
    LCD_PutString(x0+i*xm/xj-4,y0+5,Tabx[i],ccolor,bcolor); //��ʾ�̶Ȳ���ֵ��������Tab����
  }
  
  LCD_PutString(x0-12,y0+4,"O",ccolor,bcolor);      //��ʾԭ��0
} 