#ifndef LM1602and12864_h
#define LM1602and12864_h

#define RS_set P5OUT |= BIT5         //RS����λ
#define RS_clr P5OUT &= ~BIT5        //RS�˸�λ

#define RW_set P5OUT |= BIT6         //RW����λ
#define RW_clr P5OUT &= ~BIT6        //RW�˸�λ

#define E_set P5OUT |= BIT7          //Enable����λ
#define E_clr P5OUT &= ~BIT7         //Enable�˸�λ

#define dataout P4DIR = 0xff         //P4�˿�״̬Ϊȫ���
#define dataport P4OUT               //����˿�

#define PSB_set P5OUT |= BIT0        //
#define PSB_clr P5OUT &= ~BIT0       //

#define RET_set P5OUT |= BIT1        //RET����λ
#define RET_clr P5OUT &= ~BIT1       //RET�˸�λ


#endif /* 1602OR12864_H_ */
