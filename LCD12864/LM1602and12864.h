#ifndef LM1602and12864_h
#define LM1602and12864_h

#define RS_set P5OUT |= BIT5         //RS端置位
#define RS_clr P5OUT &= ~BIT5        //RS端复位

#define RW_set P5OUT |= BIT6         //RW端置位
#define RW_clr P5OUT &= ~BIT6        //RW端复位

#define E_set P5OUT |= BIT7          //Enable端置位
#define E_clr P5OUT &= ~BIT7         //Enable端复位

#define dataout P4DIR = 0xff         //P4端口状态为全输出
#define dataport P4OUT               //输出端口

#define PSB_set P5OUT |= BIT0        //
#define PSB_clr P5OUT &= ~BIT0       //

#define RET_set P5OUT |= BIT1        //RET端置位
#define RET_clr P5OUT &= ~BIT1       //RET端复位


#endif /* 1602OR12864_H_ */
