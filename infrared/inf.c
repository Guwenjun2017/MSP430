/*
 * inf.c
 *
 *  Created on: 2018年11月9日
 *      Author: Jack
 */
#include"inf.h"
extern unsigned char RECV_FLAG;
extern unsigned char ircode[4];
extern unsigned char data[33];
extern unsigned char VALUE;

/*
 * 名    称：infrared_decode()
 * 功    能：红外译码
 * 参    数：无
 * 返回值：无
 */
void infrared_decode()
{
    if(RECV_FLAG == 1)    //若成功接收到一组数据（33个下降沿触发后）
    {
        unsigned char k=1, i, j,value;    //k设置为1，因为第一位为引导码，需舍掉
        for(j=0; j<4; j++)                //分4组，每组8位，依次解码
        {
            for(i=0; i<8; i++)
            {
                value = value>>1;
                if(data[k]>6)             //若该位数据持续的count数>6,即为“1”码，否则为“0”码
                {
                    value = value|0x80;
                }
                k++;
            }
            ircode[j]=value;              //ircode数组依次存储两个用户码、一个数据码、一个数据码反码、
        }
        RECV_FLAG=0;                      //接收标志位清零
        P6DIR = 0xff;                     //38 39行起调试作用，用于观测每个按键对应的数据不同
        P6OUT = ircode[2];                //ircode[2]为数据码
        write_cmd_1602(0x01);                  //lcd1602清屏
        write_cmd_1602(0x80);                  //在第一行显示
        dis_num(ircode[2]);               //显示ircode[2]
    }
}
/*
 * 名    称：set_value()
 * 功    能：依据不同参数，设置与之对应的所需的键值
 * 参    数：每个按键对应的不同的数据码
 * 返回值：无
 */
void set_value(unsigned char argu)
{
    switch(argu)    //这里只设置了遥控器后四行的按键值，可根据需要修改
    {
    case  7: VALUE = 'S'; break; case 21: VALUE = 'A'; break; case  9: VALUE = 'E'; break;
    case 22: VALUE = 0; break;
    case 12: VALUE = 1; break;   case 24: VALUE = 2; break;   case 94: VALUE = 3; break;
    case  8: VALUE = 4; break;   case 28: VALUE = 5; break;   case 90: VALUE = 6; break;
    case 66: VALUE = 7; break;   case 82: VALUE = 8; break;   case 74: VALUE = 9; break;
    default: break;
    }
}
/*
 * 名    称：init_port1()
 * 功    能：设置P1.0端口的中断
 * 参    数：无
 * 返回值：无
 */
void init_port1()
{
    P1DIR &= ~BIT0;     //设置为输入方向
    P1IES |= BIT0;      //选择下降沿触发
    P1IE |= BIT0;       //打开中断允许
    P1IFG = ~BIT0;      //P1IES的切换可能使P1IFG置位，需清除
}
/*
 * 名    称：init_devi()
 * 功    能：设置TIMERA
 * 参    数：无
 * 返回值：无
 */
void init_device()
{
    TACCTL0 = CCIE;            //CCR0中断使能
    TACCR0 = 33;               //计数终点
    TACTL = TASSEL_2 + MC_1;   //TIMER_A选择timer时钟MCLK和增计数模式
    BCSCTL2 |= SELS;
    _EINT();                   //开启总中断
}
