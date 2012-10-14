#include <avr/io.h>
#include "adc.h"

/* Initialize ADC */
void Init_ADC(void)
{
    /*
        10 Bit ADC
        Prescalar  = 128
        ADC Freq: 16000000/128 = 125000 Hz
        AREF = AVcc
    */
    ADMUX  = (1<<REFS0);
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}

//--------------------------------------
/* Read ADC Channel */

uint16_t ADC_read( uint8_t ch)
{
   ADMUX   = ( (ch&0b00000111) | (1<<REFS0) );  //Select ADC Channel ch must be 0-7
   ADCSRA |= (1<<ADSC);                         //Start Single conversion
   while( !(ADCSRA & (1<<ADIF)) );              //Wait for conversion to complete
   ADCSRA |= (1<<ADIF);                         //Clear ADIF by writing one to it

   return ADC;
}
