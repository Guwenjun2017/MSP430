#include <msp430.h>

#define mosi BIT1
#define miso BIT2
#define scl BIT0
#define portout P6OUT
#define portin P6IN
#define portdir P6DIR
#define ss BIT3

#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

#define uchar unsigned char


void init_s(){
    portdir |= mosi;
    portdir &= (~miso);
    portdir |= scl;

}

void Scl(unsigned char a){
    if(a){
        portout|=scl;
    }else{
        portout&=(~scl);
    }
}

void Ss(unsigned char a){
    if(a){
        portout|=ss;
    }else{
        portout&=(~ss);
    }
}

void Mosi(unsigned char a){
    if(a&0x80){
        portout|=mosi;
    }else{
        portout&=(~mosi);
    }
}

unsigned char Miso(){
    if(portin&miso){
        return 1;
    }else{
        return 0;
    }
}



unsigned char write_read(unsigned char dat){
    unsigned char i=0;
    unsigned char recv=0;
    Ss(1);
    for(i=0;i<8;i++){
        recv<<=1;
        Scl(0);
        Mosi(dat);
        dat<<=1;
        Scl(1);
        delay_us(2);
        Scl(0);
        delay_us(2);
        recv|=Miso();
    }
    Ss(0);
    delay_us(2);
    return recv;

}

void init_m(){
    portdir |= mosi;
    portdir &= (~miso);
    portdir |= scl;
    portdir |= ss;
    Ss(0);
    Mosi(0);
}
/**
 * main.c
 */
void Clock_Init()
{
  uchar i;
  BCSCTL1&=~XT2OFF;                 //打开XT2振荡器
  BCSCTL2|=SELM1+SELS;              //MCLK为8MHZ，SMCLK为8MHZ
  do{
    IFG1&=~OFIFG;                   //清楚振荡器错误标志
    for(i=0;i<100;i++)
       _NOP();
  }
  while((IFG1&OFIFG)!=0);           //如果标志位1，则继续循环等待
  IFG1&=~OFIFG;
}
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    Clock_Init();
    int i=0;
    int j=0;
    init_m();
    for(i=0;i<16;i++){
        delay_ms(255);
    }
    while(1){
        write_read(j);
        j++;
        for(i=0;i<4;i++){
                delay_ms(255);
        }
    }
}
