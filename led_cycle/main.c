#include <msp430f169.h>

#define CPU_F1 ((double) 1000000)
#define delay_us1M(x) __delay_cycles((long)(CPU_F1*(double)x/1000000.0))
#define delay_ms1M(x) __delay_cycles((long)(CPU_F1*(double)x/1000.0))

/*
 * main.c
 */

void main(void)
{
    unsigned char j,k;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P6DIR = 0xff;
    while(1)
    {
        k = 1;
        for(j = 0; j < 8; j++){
            P6OUT = ~k;

            delay_ms1M(300);
            k = k<<1;
        }
    }
}
