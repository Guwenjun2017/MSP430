#include<msp430f169.h>
typedef unsigned char uchar;
typedef unsigned int  uint;
/**************宏定义***************/
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
/**************宏定义***************/
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
uchar flag;//定义一个读标志位
uchar second,minute,hour,week,day,month,year;//秒、分、时、星期、日、月、年
/*******************************************
函数名称：delay
功    能：延时一段时间
参    数：time--延时长度
********************************************/
void delay(uint time)
{
    uint i;
    for(i = 0;i < time;i++)     _NOP();
}
/*******************************************
函数名称：Delay5ms 功    能：延时约5ms
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
函数名称：WaitForEnable
功    能：等待1602液晶完成内部操作
********************************************/
void WaitForEnable(void)
{
    P4DIR &= 0x00;  //将P4口切换为输入状态
    CLR_RS;
    SET_RW;
    _NOP();
    SET_EN;
    _NOP();
    _NOP();

    while((P4IN & Busy)!=0);  //检测忙标志
    CLR_EN;
    P4DIR |= 0xFF;  //将P4口切换为输出状态
}
/*******************************************
函数名称：write_com
功    能：向液晶模块写入命令
********************************************/
void write_com(uchar cmd)
{
    WaitForEnable();   // 检测忙信号?

    CLR_RS;
    CLR_RW;
    _NOP();
    DataPort = cmd;             //将命令字写入数据端口
    _NOP();

    SET_EN;                     //产生使能脉冲信号
    _NOP();
    _NOP();
    CLR_EN;
}

/*******************************************
函数名称：write_data
功    能：向液晶显示的当前地址写入显示数据
参    数：data--显示字符数据
返回值  ：无
********************************************/
void write_data( uchar data )
{
    WaitForEnable();        //等待液晶不忙
    SET_RS;
    CLR_RW;
    _NOP();
    DataPort = data;        //将显示数据写入数据端口
    _NOP();
    SET_EN;                 //产生使能脉冲信号
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
函数名称：LcdReset
功    能：对1602液晶模块进行复位操作
********************************************/
void LcdReset(void)
{
    CtrlDir |= 0x07;  //控制线端口设为输出状态
    DataDir  = 0xFF;  //数据端口设为输出状态

    write_com(0x38);//规定的复位操作
    Delay5ms();
    write_com(0x38);
    Delay5ms();
    write_com(0x38);
    Delay5ms();
    write_com(0x38);//显示模式设置
    write_com(0x06);//写字符时整体不移动
    write_com(0x0c);//显示开，不开游标，不闪烁
     write_com(0x01);//显示清屏

Write1602(0x80,'2');
Write1602(0x81,'0');
Write1602(0x80+4,'-');
Write1602(0x80+7,'-');
Write1602(0x80+0x40+5,':');
Write1602(0x80+0x40+8,':');


}
/*******************************************
函数名称：Reset_DS1302
功    能：对DS1302进行复位操作
********************************************/
void Reset_DS1302(void)
{
    DS_RST_OUT;  //RST对应的IO设置为输出状态
    DS_SCL_OUT;  //SCLK对应的IO设置为输出状态
    DS_SCL0;     //SCLK=0
    DS_RST0;     //RST=0
    delay(10);
    DS_SCL1;    //SCLK=1
}
/*******************************************
函数名称：Write1Byte
功    能：对DS1302写入1个字节的数据
********************************************/
void Write_Byte(uchar wdata)
{
    uchar i;
    DS_SDA_OUT;  //SDA对应的IO设置为输出状态
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
函数名称：Read1Byte
功    能：从DS1302读出1个字节的数据
********************************************/
uchar Read_Byte()
{
    uchar i;
    uchar rdata = 0X00;
    DS_SDA_IN;  //SDA对应的IO设置为输入状态
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
函数名称：Write_dat
功    能：向某个寄存器写入一个字节数据
参    数：add--寄存器地址
          dat--写入的数据
********************************************/
void Write_dat(uchar add, uchar dat)
{
    DS_RST0;
    DS_SCL0;
    _NOP();
    DS_RST1;
    Write_Byte(add);   //写入地址
    Write_Byte(dat);  //写入数据
    DS_SCL1;
    DS_RST0;
}
/*******************************************
函数名称：Read_dat
功    能：从某个寄存器读出一个字节数据
参    数：addr--寄存器地址
返回值  ：读出的数据
********************************************/
uchar read_1302add(uchar add)
{
    uchar rdata;
    DS_RST0;
    DS_SCL0;
    _NOP();
    DS_RST1;
    Write_Byte(add);    //写入地址
    rdata = Read_Byte();  //读出数据
    DS_SCL1;
    DS_RST0;

    return(rdata);
}
/***********初始化1302*************/
void init_1302()
{
  flag=read_1302add(0x81);//读秒寄存器最高位,读出时钟状态
  if(flag&0x80)//判断时钟是否关闭,若内部关闭,则初始化,否则继续走
  {
    Write_dat(0x8e,0x00);
    Write_dat(0x80,((55/10)<<4|(55%10)));//写秒寄存器，并写入初值55
   Write_dat(0x82,((59/10)<<4|(59%10)));//写分寄存器，并写入初值59
   Write_dat(0x84,((22/10)<<4|(22%10)));//写小时寄存器，并写入初值23
   Write_dat(0x86,((24/10)<<4|(24%10)));//写日寄存器，并写入初值18
   Write_dat(0x88,((2/10)<<4|(2%10)));//写月寄存器，并写入初值2
   Write_dat(0x8a,((5/10)<<4|(5%10)));//写周寄存器，并写入初值5
   Write_dat(0x8c,((12/10)<<4|(12%10)));//写年寄存器，并写入初值12，不能写2012年
    Write_dat(0x90,0xa5);//写充电方式
    Write_dat(0x8e,0x80);//加上写保护

  }
}
/*****************读出秒的十进制数***************************/

uchar readsecond()
{
uchar dat;
dat=read_1302add(0x81);
second=((dat&0x70)>>4)*10+(dat&0x0f);
return second;
}
/*****************读出分的十进制数***************************/
uchar readminute()
{
uchar dat;
dat=read_1302add(0x83);
minute=((dat&0x70)>>4)*10+(dat&0x0f);
return minute;
}
/*****************读出小时的十进制数***************************/
uchar readhour()
{
uchar dat;
dat=read_1302add(0x85);
hour=((dat&0x70)>>4)*10+(dat&0x0f);
return hour;
}
/*****************读出天的十进制数***************************/
uchar readday()
{
uchar dat;
dat=read_1302add(0x87);
day=((dat&0x70)>>4)*10+(dat&0x0f);
return day;
}
/*****************读出月的十进制数***************************/
uchar readmonth()
{
uchar dat;
dat=read_1302add(0x89);
month=((dat&0x70)>>4)*10+(dat&0x0f);
return month;
}
/*****************读出周的十进制数***************************/
uchar readweek()
{
uchar dat;
dat=read_1302add(0x8b);
week=((dat&0x70)>>4)*10+(dat&0x0f);
return week;
}
/*****************读出年的十进制数***************************/
uchar readyear()
{
uchar dat;
dat=read_1302add(0x8d);
year=((dat&0xf0)>>4)*10+(dat&0x0f);
return year;
}

/************************读出所有时间**********************/
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
/*********************向1602写入时间****************************/
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

/*************************主函数*************************/
void main( void )
{
    /*下面六行程序关闭所有的IO口*/
    P1DIR = 0XFF;P1OUT = 0XFF;
    P2DIR = 0XFF;P2OUT = 0XFF;
    P3DIR = 0XFF;P3OUT = 0XFF;
    P4DIR = 0XFF;P4OUT = 0XFF;
    P5DIR = 0XFF;P5OUT = 0XFF;
    P6DIR = 0XFF;P6OUT = 0XFF;

    WDTCTL = WDTPW + WDTHOLD;  //关狗
    LcdReset();
 //zifuchuan("The time is:");
    //循环读数显示
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



