#ifndef PNEUMATIC_H_
#define PNEUMATIC_H_

/************************************************************************
 *	DEFINES
 */
/* Shift Buttons */
#define DDR_SHIFT_BUTTON	DDRD
#define PIN_SHIFT_BUTTON	PIND					
#define PIN_SHIFT_UP		PD2								/* INT0 */		
#define PIN_SHIFT_DOWN		PD3								/* INT1 */

/* Solenoids */
#define DDR_SOLENOID		DDRD
#define PORT_SOLENOID		PORTD
#define PIN_SOLENOID_UP		PD6
#define PIN_SOLENOID_DOWN	PD7

/* Reed Sensors */
#define DDR_REED_SENSORS	DDRC
#define PORT_REED_SENSORS	PORTC
#define PIN_REED_SENSORS	PINC
#define PIN_REED_UP			PC3
#define PIN_REED_CENTER		PC4
#define PIN_REED_DOWN		PC5

/* Reed Sensor Indicator LED's */
#define PIN_REED_LED_UP		PC0
#define PIN_REED_LED_CENTER PC1
#define PIN_REED_LED_DOWN	PC2

/* System ONLINE Indicator */
#define DDR_ONLINE_IND		DDRD
#define PORT_ONLINE_IND		PORTD
#define PIN_ONLINE_IND		PD5

/* TIMERS */
#define DEBOUNCE_TIME		100
#define SOLENOID_TIMEOUT	1000

/************************************************************************
 *	ENUMERATION
 */
typedef enum
{
	BUTTON_RELEASED = 0,
	BUTTON_PRESSED,
	BUTTON_CHECK_PRESSED,
	BUTTON_CHECK_RELEASED
			
} ButtonState;

typedef enum
{
	PISTON_UP = 0,
	PISTON_CENTER,
	PISTON_DOWN,
	PISTON_UNKNOWN
	
} PistonPos;

typedef enum
{
	PNEUM_IDLE = 0,
	PNEUM_PRESSED_UP,
	PNEUM_PRESSED_DOWN,
	PNEUM_SOLENOID_UP,
	PNEUM_SOLENOID_DOWN,
	PNEUM_SHIFTED_UP,
	PNEUM_SHIFTED_DOWN
	
} PneumaticState;
/************************************************************************
 *	FUNCTION PROTOTYPES
 */

PistonPos	GetPistonPos  ( void );
void		DispPistonPos ( void );


#endif /* PNEUMATIC_H_ */