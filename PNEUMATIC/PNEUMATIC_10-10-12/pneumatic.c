#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "pneumatic.h"
#include "init.h"

/************************************************************************
 *	GET PISTON POSITION
 */
PistonPos	GetPistonPos( void )
{
	ATOMIC_BLOCK( ATOMIC_RESTORESTATE ){
		
		uint8_t reed_port = ( CHK( PIN_REED_SENSORS, 1<<PIN_REED_UP ) | CHK( PIN_REED_SENSORS, 1<<PIN_REED_CENTER ) | CHK( PIN_REED_SENSORS, 1<<PIN_REED_DOWN ) );
		
		if( reed_port == ( 1<<PIN_REED_UP ) ){
			return PISTON_UP;
		}		
			
		else if( reed_port == ( 1<<PIN_REED_CENTER ) ){
			return PISTON_CENTER;
		}
		
		else if( reed_port == ( 1<<PIN_REED_DOWN ) ){
			return PISTON_DOWN;
		}
		
		else{
			return PISTON_UNKNOWN;
		}
	} 
}

/************************************************************************
 *	DISPLAY PISTON POSITION
 */
void DispPistonPos ( void )
{
	ATOMIC_BLOCK ( ATOMIC_RESTORESTATE ){
		
		/* Clear LED's */
		PORT_REED_SENSORS &= ~( ( 1<<PIN_REED_LED_CENTER) | ( 1<<PIN_REED_LED_DOWN) | ( 1<<PIN_REED_LED_UP) );
		
		if( CHK( PIN_REED_SENSORS, 1<<PIN_REED_UP ) ){
				PORT_REED_SENSORS |= ( 1<<PIN_REED_LED_UP );
		}		
			
		if( CHK( PIN_REED_SENSORS, 1<<PIN_REED_CENTER ) ){
			PORT_REED_SENSORS |= ( 1<<PIN_REED_LED_CENTER );
		}
		
		if( CHK( PIN_REED_SENSORS, 1<<PIN_REED_DOWN ) ){
			PORT_REED_SENSORS |= ( 1<<PIN_REED_LED_DOWN );
		}	
	}
	
}