/****************************************************************************
                          keyboard for MSP430
                          Designed by CUGer——Mr.Wei
                程序可用，当系统时钟改变时，注意修改按键消抖延时
****************************************************************************/
#include<msp430f169.h>
#include"delay.h"
#include"lcd_1602.h"
#include"Port.h"

#define KEY_DIR P2DIR
#define KEY_OUT P2OUT
#define KEY_IN  P2IN
#define KEY_IE  P2IE
#define KEY_IES P2IES
#define KEY_IFG P2IFG

/***************全局变量***************/
unsigned char Key_Val;                              //存放键值

void CtrlKey(unsigned char sw);                     //控制键盘开关//sw=0关 sw=1开

/*******************************************
函数名称：Init_Keypad
功    能：初始化扫描键盘的IO端口
参    数：无
返回值  ：无
********************************************/
void Init_Keypad(void)
{
  KEY_DIR = 0x0f;                      //P1.0~P1.3设置为输出状态,P1.4~P1.7输入 状态(上拉H)
  KEY_OUT  &=0xf0;                     //P1.0~P1.3输出为低电平（卫编）

  KEY_IES =0xf0;                       //P1.4~P1.7下降沿触发中断
  KEY_IE  =0xf0;                       //P1.4~P1.7允许中断
  KEY_IFG= 0;                          //中断标志清0

  Key_Val = 16;                        //按键值初始设为非0~15的值均可
}

/*******************************************
函数名称：Check_Key
功    能：扫描键盘的IO端口，获得键值
参    数：无
返回值  ：无
********************************************/
//p14\5\6\7 接上拉电阻
/***************************************
            key_Val 对应键值
        列：[p14]  [p15]  [p16]   [p17]
             ↓     ↓     ↓     ↓
 行：
[p13]→      0     1      2      3
[p12]→      4     5      6      7
[p11]→      8     9      10     11
[p10]→     12     13     14     15
***************************************/
void Check_Key(void)
{
  unsigned char hang ,lie,tmp1,tmp2;
  unsigned char keymap[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};//设置键盘逻辑键值与程序计算键值的映射

  tmp1 = 0x08;
  for(hang = 0;hang < 4;hang++)              //行扫描
  {
    KEY_OUT = 0x0f;                       //P1.0~P1.3输出全1
    KEY_OUT -= tmp1;                      //P1.0~p1.3输出四位中有一个为0
    tmp1 >>=1;
    if((KEY_IN & 0xf0)<0xf0)              //是否P1IN的P1.4~P1.7中有一位为0
    {
      tmp2 = 0x10;                        // tmp2用于检测出哪一位为0
      for(lie = 0;lie < 4;lie++)          // 列检测
      {
        if((KEY_IN & tmp2) == 0x00)       // 是否是该列,等于0为是
        {
          Key_Val = keymap[hang*4 + lie];  // 获取键值
          return;                         // 退出循环
        }
        tmp2 <<= 1;                       // tmp2右移1位
      }
    }
  }
}


/*********************************************************************
控制打开或者关闭键盘中断
SW= 0：关闭；   ELSE：打开
*********************************************************************/
void CtrlKey(unsigned char sw)
{
 if(sw==0)
   KEY_IE =0;     //关闭端口中断
 else
           KEY_IE =0xf0; //打开端口中断
}

/*********************************************************************
                      主函数
*********************************************************************/

void main()
{
  WDTCTL=WDTPW+WDTHOLD;                    //关闭看门狗
  Init_Keypad();                           //键盘初始化
  delay_ms(10);
  init_1602_port();
  lcd1602_init();
  CtrlKey(1);                              //键盘打开
  while(1)
      {
          Check_Key();
          write_cmd(0x80);
          display(Key_Val);
      }

}

/*端口2按键中断*/
#pragma vector=PORT2_VECTOR
__interrupt void Port(void)
{
  if((KEY_IFG&0xf0)!=0)
  {
    if(Key_Val!=16)         //键值！=16有键按下
    {
      CtrlKey(0);         //关键盘中断
    }
  }

  KEY_IFG=0;KEY_OUT=0;   //清中断标志
}
