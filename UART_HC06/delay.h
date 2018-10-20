#ifndef delay_h
#define delay_h

#define CPU_F1 ((double) 8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F1*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F1*(double)x/1000.0)) 

//���ڲ����ʼ��㣬��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((unsigned long)CPU_F1/((unsigned long)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ
#endif
