#ifndef delay_h
#define delay_h

#define CPU_F1 ((double) 1000000)
#define delay_ms(x) __delay_cycles((long)(CPU_F1*(int)x/1000.0))

#endif
