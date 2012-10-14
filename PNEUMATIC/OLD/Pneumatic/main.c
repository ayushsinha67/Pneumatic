#include <avr/io.h>
#include <util/delay.h>

/* Bitwise Manipulation */
#define CHK(x,b) (x&b)
#define CLR(x,b) (x&=~b)
#define SET(x,b) (x|=b)
#define TOG(a,b) (a^=b)


int main(void)
{
    /* Data Direction */
    DDRD = 0xFF;    // UPSHIFT & DOWNSHIFT LEDS PD6, PD7

    while(1){

        PORTD = 0xFF;
        _delay_ms(1000);
        PORTD = 0x00;
        _delay_ms(1000);
    }


    return 0;
}
