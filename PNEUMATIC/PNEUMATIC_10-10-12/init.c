#include <avr/io.h>
#include <avr/interrupt.h>
#include "pneumatic.h"
#include "init.h"

/************************************************************************
 *	INITIALIZE GPIO
 */
void GPIO_Init ( void )
{	
	/* REED SENSOR INPUTS */
	DDR_REED_SENSORS &= ~( ( 1<<PIN_REED_UP ) | ( 1<<PIN_REED_DOWN ) | ( 1<<PIN_REED_CENTER ) );
	
	/* REED SENSORS PULL UPS */
	PORT_REED_SENSORS &= ~( ( 1<<PIN_REED_UP ) | ( 1<<PIN_REED_DOWN ) | ( 1<<PIN_REED_CENTER ) );
	
	/* REED LED OUTPUT INDICATORS*/
	DDR_REED_SENSORS |= ( ( 1<<PIN_REED_LED_UP ) | ( 1<<PIN_REED_LED_DOWN ) | ( 1<<PIN_REED_LED_CENTER ) );	
	
	/* SOLENOID OUTPUTS */
	DDR_SOLENOID |= ( ( 1<<PIN_SOLENOID_UP ) | ( 1<<PIN_SOLENOID_DOWN ) );	
	
	/* INPUT SHIFT BUTTONS - Also configured as Interrupt */				
	DDR_SHIFT_BUTTON &= ~( ( 1<<PIN_SHIFT_UP ) | ( 1<<PIN_SHIFT_DOWN ) );						
							
}

/************************************************************************
 *	INITIALIZE TIMER
 *  
 *  TIMER2 - CTC MODE, F_CPU: 16Mhz, PS: 64, Frequency: 1 KHz, Period: 1 ms 
 */
void Timer_Init(void)
{
    TCCR2 |= ( 1<<WGM21 ) | ( 1<<CS22 );				/* CTC, PS: 64 */
    TIMSK |= ( 1<<OCIE2 );								/* Enable Interrupt */
    OCR2 = 249;
}

/************************************************************************
 *	EXTERNAL INTERRUPT INTITIALIZATION
 */
void INT_Init (void)
{	
	/* INT0 and INT1 */
	GICR = ( ( 1<<INT0 ) | ( 1<<INT1 ) );					
	
	/* INT0 and INT1 on rising edge */
	MCUCR = ( ( 1<<ISC01 ) | ( 1<<ISC00 ) | ( 1<<ISC10 ) | ( 1<<ISC11) );
}
