#include <msp430f169.h>
#define CPU_F ((double)8000000) 
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long

/*��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��*/
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ

#define DATAPORT	P4OUT                   //���ݿ����ڶ˿�P4
#define DATASEL         P4SEL                   //���ݿڹ��ܼĴ��������ƹ���ģʽ
#define DATADIR         P4DIR                   //���ݿڷ���Ĵ���

#define CTRPORT	        P5OUT                   //���������ڵĶ˿�P6
#define CTRSEL          P5SEL                   //���ƿڹ��ܼĴ��������ƹ���ģʽ
#define CTRDIR          P5DIR                   //���ƿڷ���Ĵ���

//������TFT��ʾ�������Ӳ������
#define RS_CLR	        P5OUT &= ~BIT5           //RS�õ�
#define RS_SET	        P5OUT |=  BIT5           //RS�ø�

#define RW_CLR	        P5OUT &= ~BIT6           //RW�õ�
#define RW_SET	        P5OUT |=  BIT6           //RW�ø�

#define RD_CLR	        P5OUT &= ~BIT7           //E�õ�
#define RD_SET	        P5OUT |=  BIT7           //E�ø�

#define CS_CLR	        P5OUT &= ~BIT0            //CS�õ�
#define CS_SET	        P5OUT |=  BIT0            //CS�ø�

#define RST_CLR	        P5OUT &= ~BIT3            //RST�õ�
#define RST_SET	        P5OUT |=  BIT3            //RST�ø�

#define LE_CLR	        P5OUT &= ~BIT1            //LE�õ�
#define LE_SET	        P5OUT |=  BIT1            //LE�ø�

//������TFT�������������Ӳ������
#define PEN_CLR	        P6OUT &= ~BIT0           //PEN�õ�,����������ʱ��ADS7843Penirq������δ����ʱ�ĸߵ�ƽ��Ϊ�͵�ƽ
#define PEN_SET	        P6OUT |=  BIT0           //PEN�ø�
#define PEN             (P6IN & 0x01)            //P6.0�����ֵ

#define TPDO_CLR	P6OUT &= ~BIT1           //TPDO�õ�
#define TPDO_SET	P6OUT |=  BIT1           //TPDO�ø�
#define TPDOUT          ((P6IN>>1)&0x01)         //P6.1�����ֵ

#define BUSY_CLR	P6OUT &= ~BIT2           //BUSY�õ�
#define BUSY_SET	P6OUT |=  BIT2           //BUSY�ø�

#define TPDI_CLR	P6OUT &= ~BIT3            //TPDI�õ�
#define TPDI_SET	P6OUT |=  BIT3            //TPDI�ø�

#define TPCS_CLR	P6OUT &= ~BIT4            //TPCS�õ�
#define TPCS_SET	P6OUT |=  BIT4            //TPCS�ø�

#define TPCLK_CLR	P6OUT &= ~BIT5            //TPCLK�õ�
#define TPCLK_SET	P6OUT |=  BIT5            //TPCLK�ø�


#define DataPort        P4OUT                     //P4��Ϊ���ݿ�


#define LED8            P3OUT                      //P34��P35�ڽ�2��LED�����ڲ���

/*12864Ӧ��ָ��*/
#define CLEAR_SCREEN	0x01		          //����ָ�������ACֵΪ00H
#define AC_INIT		0x02		          //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD	0x06		          //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE	0x30		          //����ģʽ��8λ����ָ�
#define DISPLAY_ON	0x0c		          //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF	0x08		          //��ʾ��
#define CURSE_DIR	0x14		          //�α������ƶ�:AC=AC+1
#define SET_CG_AC	0x40		          //����AC����ΧΪ��00H~3FH
#define SET_DD_AC	0x80                      //����DDRAM AC

uchar   key=0xFF;                                 //��ֵ����

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ��
//***********************************************************************

