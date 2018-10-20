#ifndef delay_h
#define delay_h

#define CPU_F1 ((double) 8000000)
#define delay_us(x) __delay_cycles((long)(CPU_F1*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F1*(double)x/1000.0)) 

//串口波特率计算，当BRCLK=CPU_F时用下面的公式可以计算，否则要根据设置加入分频系数
#define baud           9600                                //设置波特率的大小
#define baud_setting   (uint)((unsigned long)CPU_F1/((unsigned long)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位
#endif
