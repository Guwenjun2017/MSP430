#include<msp430f169.h>
typedef unsigned char uchar;
typedef unsigned int  uint;
/**************�궨��***************/
#define DataDir  P4DIR
#define DataPort  P4OUT
#define Busy  0x80
#define CtrlDir     P5DIR
#define CLR_RS P5OUT&=~BIT5;    //RS = P3.0
#define SET_RS P5OUT|=BIT5;
#define CLR_RW P5OUT&=~BIT6; //RW = P3.1
#define SET_RW P5OUT|=BIT6;
#define CLR_EN P5OUT&=~BIT7; //EN = P3.2
#define SET_EN P5OUT|=BIT7;
/**************�궨��***************/
#define DS_RST  BIT0 //DS_RST = P2.7
#define DS_SCL  BIT1  //DS_SCL = P2.5
#define DS_SDA  BIT2  //DS_SDA = P2.6

#define DS_RST_IN P5DIR &= ~DS_RST
#define DS_RST_OUT P5DIR |= DS_RST
#define DS_RST0 P5OUT &= ~DS_RST
#define DS_RST1 P5OUT |= DS_RST

#define DS_SCL_IN P5DIR &= ~DS_SCL
#define DS_SCL_OUT P5DIR |= DS_SCL
#define DS_SCL0 P5OUT &= ~DS_SCL
#define DS_SCL1 P5OUT |= DS_SCL

#define DS_SDA_IN P5DIR &= ~DS_SDA
#define DS_SDA_OUT P5DIR |= DS_SDA
#define DS_SDA0 P5OUT &= ~DS_SDA
#define DS_SDA1 P5OUT |= DS_SDA
#define DS_SDA_BIT P5IN & DS_SDA
uchar flag;//����һ������־λ
uchar second,minute,hour,week,day,month,year;//�롢�֡�ʱ�����ڡ��ա��¡���
/*******************************************
�������ƣ�delay
��    �ܣ���ʱһ��ʱ��
��    ����time--��ʱ����
********************************************/
void delay(uint time)
{
    uint i;
    for(i = 0;i < time;i++)     _NOP();
}
/*******************************************
�������ƣ�Delay5ms ��    �ܣ���ʱԼ5ms
********************************************/
void Delay5ms(void)
{
    uint i=40000;
    while (i != 0)
    {
        i--;
    }
}
/*******************************************
�������ƣ�WaitForEnable
��    �ܣ��ȴ�1602Һ������ڲ�����
********************************************/
void WaitForEnable(void)
{
    P4DIR &= 0x00;  //��P4���л�Ϊ����״̬
    CLR_RS;
    SET_RW;
    _NOP();
    SET_EN;
    _NOP();
    _NOP();

    while((P4IN & Busy)!=0);  //���æ��־
    CLR_EN;
    P4DIR |= 0xFF;  //��P4���л�Ϊ���״̬
}
/*******************************************
�������ƣ�write_com
��    �ܣ���Һ��ģ��д������
********************************************/
void write_com(uchar cmd)
{
    WaitForEnable();   // ���æ�ź�?

    CLR_RS;
    CLR_RW;
    _NOP();
    DataPort = cmd;             //��������д�����ݶ˿�
    _NOP();

    SET_EN;                     //����ʹ�������ź�
    _NOP();
    _NOP();
    CLR_EN;
}

/*******************************************
�������ƣ�write_data
��    �ܣ���Һ����ʾ�ĵ�ǰ��ַд����ʾ����
��    ����data--��ʾ�ַ�����
����ֵ  ����
********************************************/
void write_data( uchar data )
{
    WaitForEnable();        //�ȴ�Һ����æ
    SET_RS;
    CLR_RW;
    _NOP();
    DataPort = data;        //����ʾ����д�����ݶ˿�
    _NOP();
    SET_EN;                 //����ʹ�������ź�
    _NOP();
    _NOP();
    CLR_EN;
}
void Write1602(uchar add,uchar dat)
{
 write_com(add);
 write_data(dat);
}

void zifuchuan(uchar *ch)
{
  while(*ch!=0)
  write_data(*ch++);
  Delay5ms();
}
/*******************************************
�������ƣ�LcdReset
��    �ܣ���1602Һ��ģ����и�λ����
********************************************/
void LcdReset(void)
{
    CtrlDir |= 0x07;  //�����߶˿���Ϊ���״̬
    DataDir  = 0xFF;  //���ݶ˿���Ϊ���״̬

    write_com(0x38);//�涨�ĸ�λ����
    Delay5ms();
    write_com(0x38);
    Delay5ms();
    write_com(0x38);
    Delay5ms();
    write_com(0x38);//��ʾģʽ����
    write_com(0x06);//д�ַ�ʱ���岻�ƶ�
    write_com(0x0c);//��ʾ���������α꣬����˸
     write_com(0x01);//��ʾ����

Write1602(0x80,'2');
Write1602(0x81,'0');
Write1602(0x80+4,'-');
Write1602(0x80+7,'-');
Write1602(0x80+0x40+5,':');
Write1602(0x80+0x40+8,':');


}
/*******************************************
�������ƣ�Reset_DS1302
��    �ܣ���DS1302���и�λ����
********************************************/
void Reset_DS1302(void)
{
    DS_RST_OUT;  //RST��Ӧ��IO����Ϊ���״̬
    DS_SCL_OUT;  //SCLK��Ӧ��IO����Ϊ���״̬
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    delay(10);
    DS_SCL1;    //SCLK=1
}
/*******************************************
�������ƣ�Write1Byte
��    �ܣ���DS1302д��1���ֽڵ�����
********************************************/
void Write_Byte(uchar wdata)
{
    uchar i;
    DS_SDA_OUT;  //SDA��Ӧ��IO����Ϊ���״̬
    DS_RST1;  //REST=1;
    for(i = 8; i > 0; i--)
    {
        if(wdata&0x01)
         DS_SDA1;
        else
        DS_SDA0;
        DS_SCL0;
        delay(10);
        DS_SCL1;
        delay(10);
        wdata >>= 1;
    }
}
/*******************************************
�������ƣ�Read1Byte
��    �ܣ���DS1302����1���ֽڵ�����
********************************************/
uchar Read_Byte()
{
    uchar i;
    uchar rdata = 0X00;
    DS_SDA_IN;  //SDA��Ӧ��IO����Ϊ����״̬
    DS_RST1;    //REST=1;
    for(i = 8; i > 0; i--)
    {
        DS_SCL1;
        delay(10);
        DS_SCL0;
        delay(10);
        rdata >>= 1;
        if(DS_SDA_BIT)
        rdata |= 0x80;
    }

    return(rdata);
}
/*******************************************
�������ƣ�Write_dat
��    �ܣ���ĳ���Ĵ���д��һ���ֽ�����
��    ����add--�Ĵ�����ַ
          dat--д�������
********************************************/
void Write_dat(uchar add, uchar dat)
{
    DS_RST0;
    DS_SCL0;
    _NOP();
    DS_RST1;
    Write_Byte(add);   //д���ַ
    Write_Byte(dat);  //д������
    DS_SCL1;
    DS_RST0;
}
/*******************************************
�������ƣ�Read_dat
��    �ܣ���ĳ���Ĵ�������һ���ֽ�����
��    ����addr--�Ĵ�����ַ
����ֵ  ������������
********************************************/
uchar read_1302add(uchar add)
{
    uchar rdata;
    DS_RST0;
    DS_SCL0;
    _NOP();
    DS_RST1;
    Write_Byte(add);    //д���ַ
    rdata = Read_Byte();  //��������
    DS_SCL1;
    DS_RST0;

    return(rdata);
}
/***********��ʼ��1302*************/
void init_1302()
{
  flag=read_1302add(0x81);//����Ĵ������λ,����ʱ��״̬
  if(flag&0x80)//�ж�ʱ���Ƿ�ر�,���ڲ��ر�,���ʼ��,���������
  {
    Write_dat(0x8e,0x00);
    Write_dat(0x80,((55/10)<<4|(55%10)));//д��Ĵ�������д���ֵ55
   Write_dat(0x82,((59/10)<<4|(59%10)));//д�ּĴ�������д���ֵ59
   Write_dat(0x84,((22/10)<<4|(22%10)));//дСʱ�Ĵ�������д���ֵ23
   Write_dat(0x86,((24/10)<<4|(24%10)));//д�ռĴ�������д���ֵ18
   Write_dat(0x88,((2/10)<<4|(2%10)));//д�¼Ĵ�������д���ֵ2
   Write_dat(0x8a,((5/10)<<4|(5%10)));//д�ܼĴ�������д���ֵ5
   Write_dat(0x8c,((12/10)<<4|(12%10)));//д��Ĵ�������д���ֵ12������д2012��
    Write_dat(0x90,0xa5);//д��緽ʽ
    Write_dat(0x8e,0x80);//����д����

  }
}
/*****************�������ʮ������***************************/

uchar readsecond()
{
uchar dat;
dat=read_1302add(0x81);
second=((dat&0x70)>>4)*10+(dat&0x0f);
return second;
}
/*****************�����ֵ�ʮ������***************************/
uchar readminute()
{
uchar dat;
dat=read_1302add(0x83);
minute=((dat&0x70)>>4)*10+(dat&0x0f);
return minute;
}
/*****************����Сʱ��ʮ������***************************/
uchar readhour()
{
uchar dat;
dat=read_1302add(0x85);
hour=((dat&0x70)>>4)*10+(dat&0x0f);
return hour;
}
/*****************�������ʮ������***************************/
uchar readday()
{
uchar dat;
dat=read_1302add(0x87);
day=((dat&0x70)>>4)*10+(dat&0x0f);
return day;
}
/*****************�����µ�ʮ������***************************/
uchar readmonth()
{
uchar dat;
dat=read_1302add(0x89);
month=((dat&0x70)>>4)*10+(dat&0x0f);
return month;
}
/*****************�����ܵ�ʮ������***************************/
uchar readweek()
{
uchar dat;
dat=read_1302add(0x8b);
week=((dat&0x70)>>4)*10+(dat&0x0f);
return week;
}
/*****************�������ʮ������***************************/
uchar readyear()
{
uchar dat;
dat=read_1302add(0x8d);
year=((dat&0xf0)>>4)*10+(dat&0x0f);
return year;
}

/************************��������ʱ��**********************/
void  readtime()
{
readsecond();
readminute();
readhour();
readday();
readmonth();
readweek();
readyear();
}
/*********************��1602д��ʱ��****************************/
void write_second()
{
uchar shi,ge;
shi=second/10;
ge=second%10;
Write1602(0x80+0x40+9,0x30+shi);
Write1602(0x80+0x40+10,0x30+ge);
}
void write_minute()
{
uchar shi,ge;
shi=minute/10;
ge=minute%10;
Write1602(0x80+0x40+6,0x30+shi);
Write1602(0x80+0x40+7,0x30+ge);
}
void write_hour()
{
uchar shi,ge;
shi=hour/10;
ge=hour%10;
Write1602(0x80+0x40+3,0x30+shi);
Write1602(0x80+0x40+4,0x30+ge);
}
void write_day()
{
uchar shi,ge;
shi=day/10;
ge=day%10;
Write1602(0x80+8,0x30+shi);
Write1602(0x80+9,0x30+ge);
}
void write_month()
{
uchar shi,ge;
shi=month/10;
ge=month%10;
Write1602(0x80+5,0x30+shi);
Write1602(0x80+6,0x30+ge);
}
void write_year()
{
uchar shi,ge;
shi=year/10;
ge=year%10;
Write1602(0x80+2,0x30+shi);
Write1602(0x80+3,0x30+ge);
}
void write_week()
{
Write1602(0x80+12,0x30+week);
//uchar week;
switch(week)
  {
   case 1: Write1602(0x80+12,'M');
        Write1602(0x80+13,'O');
     Write1602(0x80+14,'N');
     break;
    case 2:Write1602(0x80+12,'T');
        Write1602(0x80+13,'U');
     Write1602(0x80+14,'E');
     break;
    case 3:Write1602(0x80+12,'W');
        Write1602(0x80+13,'E');
     Write1602(0x80+14,'N');
     break;
    case 4:Write1602(0x80+12,'T');
        Write1602(0x80+13,'H');
     Write1602(0x80+14,'U');
     break;
 case 5:Write1602(0x80+12,'F');
        Write1602(0x80+13,'R');
     Write1602(0x80+14,'I');
     break;
    case 6:Write1602(0x80+12,'S');
        Write1602(0x80+13,'A');
     Write1602(0x80+14,'T');
     break;
 case 7:Write1602(0x80+12,'S');
        Write1602(0x80+13,'U');
     Write1602(0x80+14,'N');
     break;
  }
}

/*************************������*************************/
void main( void )
{
    /*�������г���ر����е�IO��*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;

    WDTCTL = WDTPW + WDTHOLD;  //�ع�
    LcdReset();
 //zifuchuan("The time is:");
    //ѭ��������ʾ
    init_1302();
    Write_dat(0x80, 0x00);
  while(1)
    {

  readtime();
  write_second();
  write_minute();
  write_hour();
  write_day();
  write_month();
  write_year();
  write_week();

     }
}



