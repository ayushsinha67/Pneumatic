#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"
#include "adc.c"

/* Bitwise Manipulation */
#define CHK(x,b) (x&b)
#define CLR(x,b) (x&=~b)
#define SET(x,b) (x|=b)
#define TOG(a,b) (a^=b)

/* Global Variables */
uint16_t val;
enum POSITION pos;

/* Enumerations */
enum POSITION
{
    UPSHIFT = 0,
    CENTER,
    DOWNSHIFT
};

enum POSITION GetPos(void)
{
    enum POSITION pos_var;
    val = ADC_read(0);  // Read value from channel 0
    if( val >= 900 ){ pos_var = UPSHIFT; }
    else if( (val < 900) && ( val > 200 ) ){ pos_var = CENTER; }
    else if( ( val <= 200 ) ){ pos_var = DOWNSHIFT; }
    return pos_var;
}


// ------------------------

int main(void)
{
    /* Initialize IO Pins */
    DDRD = 0xFF;    // UPSHIFT & DOWNSHIFT LEDS PD6, PD7
    DDRC = 0x00;    // Declare Input Port
    PORTC = 0xFF;   // Pull Up

    Init_ADC();     // Initialize ADC

    while(1){

        pos = GetPos();

        if( !CHK( PINC, 1<<PC0 ) && ( pos == CENTER ) ){
            _delay_ms(200);

            PORTD = 0x40;
            do{
                pos = GetPos();
            }while( pos!= UPSHIFT );
            PORTD = 0x00;
        }

        else if( !CHK( PINC, 1<<PC1 ) && ( pos == CENTER ) ){
            _delay_ms(300);

            PORTD = 0x80;
            do{
                pos = GetPos();
            }while( pos!= DOWNSHIFT );
            PORTD = 0x00;
        }

//        if( pos == UPSHIFT )  { PORTD = 0x40; }
//        else if( pos == DOWNSHIFT) { PORTD = 0x80; }

//        if( val > 512 ){
//           PORTD = 0x40;
//        }
//
//        else if ( val < 512 ){
//            PORTD = 0x80;
//        }
        _delay_ms(100);


//        if( !CHK( PINC, 1<<PC0 ) ){
//            SET( PORTD, 1<<PD6);
//        }
//        else{
//            CLR( PORTD, 1<<PD6);
//        }
//
//        if( !CHK( PINC, 1<<PC1) ){
//            SET( PORTD, 1<<PD7);
//        }
//        else{
//            CLR( PORTD, 1<<PD7);
//        }

        /* Shift only when pot in middle
           Switch on LED only when it reaches an end
           Switch off LED only if it reaches the other end
        */
    }


    return 0;
}
