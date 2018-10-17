#ifndef meimeidexin_h
#define meimeidexin_h

#define CPU_F1 ((double) 8000000)
#define delay_ms(x) __delay_cycles((long)(CPU_F1*(double)x/1000.0)) 

#endif
