/*
 * PNEUMATIC_10_10_12.c
 *
 * Created: 10-10-2012 PM 6:15:55
 * Author: Ayush Sinha
 */ 
 
 /*
 * SHIFT PROCEDURE:
 * Shift only when piston is at center
 * Switch off solenoid with feedback from up/down reed sensor or if timeout (safety) 
 */

#define	 F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "init.h"
#include "pneumatic.h"

/************************************************************************
 *	GLOBAL VARIABLES
 */
/* Volatile */
volatile uint16_t debounce_ticker0 = 0;
volatile uint16_t debounce_ticker1 = 0;
volatile uint16_t solenoid_ticker_up = 0;
volatile uint16_t solenoid_ticker_down = 0;
volatile ButtonState button_int0 = BUTTON_RELEASED;
volatile ButtonState button_int1 = BUTTON_RELEASED;
volatile PneumaticState pneum_state = PNEUM_IDLE;

/* Non Volatile */
uint16_t solenoid_ticker_down_cpy = 0;
uint16_t solenoid_ticker_up_cpy = 0;

/************************************************************************
 *	MAIN
 */
int main(void)
{
	/* Initialization */
	GPIO_Init();								/* GPIO */
	UART_Init();								/* UART */	
	Timer_Init();								/* TIMER */
	INT_Init();									/* External  Interrupt */
	
	sei();										/* Enable Global Interrupts */
	
	UART_TxStr_p( PSTR("Pneumatic Gear Shift Controller v1.0\n\n") );
	
	while(1){
		
		ATOMIC_BLOCK( ATOMIC_RESTORESTATE ){	/* Atomic Block */
			solenoid_ticker_down_cpy = solenoid_ticker_down;
			solenoid_ticker_up_cpy = solenoid_ticker_up;	
		}
		
		
		DispPistonPos();						/* Display Piston Position */
		
		switch ( pneum_state ){					/* Check Pneumatic States */
			
			case PNEUM_PRESSED_UP	:	SET( PORT_SOLENOID, 1<<PIN_SOLENOID_DOWN );		/* Actuate Opposite Solenoid */
										solenoid_ticker_down = SOLENOID_TIMEOUT;
										pneum_state = PNEUM_SOLENOID_DOWN;
										break;
			
			case PNEUM_SOLENOID_DOWN:	if( ( GetPistonPos() == PISTON_UP ) || ( solenoid_ticker_down_cpy == 0 ) ){
											CLR( PORT_SOLENOID, 1<<PIN_SOLENOID_DOWN ); /* Stop Opposite Solenoid */
											pneum_state = PNEUM_IDLE;
										}
										break;
										
			case PNEUM_PRESSED_DOWN :	SET( PORT_SOLENOID, 1<<PIN_SOLENOID_UP );		/* Actuate Opposite Solenoid */
										solenoid_ticker_up = SOLENOID_TIMEOUT;
										pneum_state = PNEUM_SOLENOID_UP;
										break;
										
			case PNEUM_SOLENOID_UP  :	if( ( GetPistonPos() == PISTON_DOWN ) || ( solenoid_ticker_up_cpy == 0 ) ){
											CLR( PORT_SOLENOID, 1<<PIN_SOLENOID_UP );	/* Stop Opposite Solenoid */
											pneum_state = PNEUM_IDLE;
										}
										break;
										
							default:	break;				
		}
			
    }
}

/************************************************************************
 *	INT0 INTERRUPT
 */
ISR( INT0_vect )
{	
	/* Noise detected, since next edge is less than debounce time. */
	if( ( button_int0 == BUTTON_CHECK_PRESSED ) && ( debounce_ticker0 < DEBOUNCE_TIME ) ){		
		
		button_int0 = BUTTON_RELEASED;	
	}
	
	/* Noise detected, since next edge is less than debounce time */
	if( ( button_int0 == BUTTON_CHECK_RELEASED ) && ( debounce_ticker0 < DEBOUNCE_TIME ) ){	
		
		button_int0 = BUTTON_PRESSED;
	}	
	
	/* Start incrementing timer if key was released and is now pressed. This can be noise or valid */
	if( button_int0 == BUTTON_RELEASED ){					
		
		debounce_ticker0 = 0;
		button_int0 = BUTTON_CHECK_PRESSED;		
	}	
	
	/* Start incrementing timer if key was pressed. This is the trailing noise edge. */
	if( button_int0 == BUTTON_PRESSED ){
		
		debounce_ticker0 = 0;
		button_int0 = BUTTON_CHECK_RELEASED;	
	}
}

/************************************************************************
 *	INT1 INTERRUPT
 */
ISR( INT1_vect )
{	
	/* Noise detected, since next edge is less than debounce time. */
	if( ( button_int1 == BUTTON_CHECK_PRESSED ) && ( debounce_ticker1 < DEBOUNCE_TIME ) ){	
		
		button_int1 = BUTTON_RELEASED;	
	}
	
	/* Noise detected, since next edge is less than debounce time */
	if( ( button_int1 == BUTTON_CHECK_RELEASED ) && ( debounce_ticker1 < DEBOUNCE_TIME ) ){	
		
		button_int1 = BUTTON_PRESSED;
	}	
	
	/* Start incrementing timer if key was released and is now pressed. This can be noise or valid */
	if( button_int1 == BUTTON_RELEASED ){					
		
		debounce_ticker1 = 0;
		button_int1 = BUTTON_CHECK_PRESSED;		
	}	
	
	/* Start incrementing timer if key was pressed. This is the trailing noise edge. */
	if( button_int1 == BUTTON_PRESSED ){
		
		debounce_ticker1 = 0;
		button_int1 = BUTTON_CHECK_RELEASED;	
	}
}

/************************************************************************
 *	TIMER0 INTERRUPT (1 MS)
 */ 
 ISR(TIMER2_COMP_vect)
 {
	/* SOLENOID DECREMENT TICKERS ------------------------------- */
	if( solenoid_ticker_down > 0 )
		solenoid_ticker_down--;
	
	if( solenoid_ticker_up > 0 )
		solenoid_ticker_up--;
	
	/* DEBOUNCE INT0 -------------------------------------------- */
	
	/* Increment ticker if signal is high and in "pressed" check state */	
	if( ( button_int0 == BUTTON_CHECK_PRESSED ) && ( CHK( PIN_SHIFT_BUTTON, 1<<PIN_SHIFT_UP ) ) ){
		
		debounce_ticker0++;
	}
	
	/* Increment ticker if signal is low and in "released" check state */
	if( ( button_int0 == BUTTON_CHECK_RELEASED ) && ( !CHK( PIN_SHIFT_BUTTON, 1<<PIN_SHIFT_UP ) ) ){
		
		debounce_ticker0++;	
	}
	
	/* If there was no trailing noise edge on button release */
	if( ( button_int0 == BUTTON_PRESSED ) && ( !CHK( PIN_SHIFT_BUTTON, 1<<PIN_SHIFT_UP ) ) ){
		
		button_int0 = BUTTON_CHECK_RELEASED;
		debounce_ticker0 = 0;
	}
	
	/* A valid button press is detected */	
	if( ( button_int0 == BUTTON_CHECK_PRESSED ) && ( debounce_ticker0 >= DEBOUNCE_TIME ) ){
		
		button_int0 = BUTTON_PRESSED;
		debounce_ticker0 = 0;
		
		/* GET BUTTON PRESSED */
		if( ( GetPistonPos() == PISTON_CENTER ) && ( pneum_state == PNEUM_IDLE ) ){
			
			pneum_state = PNEUM_PRESSED_UP;
			UART_TxStr_p( PSTR("UP SHIFT\n") );
		}
	}
	
	/* A valid button release is detected */
	if( ( button_int0 == BUTTON_CHECK_RELEASED ) && ( debounce_ticker0 >= DEBOUNCE_TIME ) ){
		
		button_int0 = BUTTON_RELEASED;
	} 	 
	 	
	/* DEBOUNCE INT1 ----------------------------------------- */
		
	/* Increment ticker if signal is high and in "pressed" check state */
	if( ( button_int1 == BUTTON_CHECK_PRESSED ) && ( CHK( PIN_SHIFT_BUTTON, 1<<PIN_SHIFT_DOWN ) ) ){
		
		debounce_ticker1++;
	}
	
	/* Increment ticker if signal is low and in "released" check state */
	if( ( button_int1 == BUTTON_CHECK_RELEASED ) && ( !CHK( PIN_SHIFT_BUTTON, 1<<PIN_SHIFT_DOWN ) ) ){
		
		debounce_ticker1++;	
	}
	
	/* If there was no trailing noise edge on button release */
	if( ( button_int1 == BUTTON_PRESSED ) && ( !CHK( PIND, 1<<PIN_SHIFT_DOWN ) ) ){
		
		button_int1 = BUTTON_CHECK_RELEASED;
		debounce_ticker1 = 0;
	}
	
	/* A valid button press is detected */		
	if( ( button_int1 == BUTTON_CHECK_PRESSED ) && ( debounce_ticker1 >= DEBOUNCE_TIME ) ){
		
		button_int1 = BUTTON_PRESSED;
		debounce_ticker1 = 0;
		
		/* GET BUTTON PRESSED */
		if( ( GetPistonPos() == PISTON_CENTER ) && ( pneum_state == PNEUM_IDLE ) ){
			
			pneum_state = PNEUM_PRESSED_DOWN;					
			UART_TxStr_p( PSTR("DOWN SHIFT\n") );
		}
		
	}
	
	/* A valid button release is detected */
	if( ( button_int1 == BUTTON_CHECK_RELEASED ) && ( debounce_ticker1 >= DEBOUNCE_TIME ) ){
		
		button_int1 = BUTTON_RELEASED;
	} 	 
 }
 
