#ifndef INIT_H_
#define INIT_H_

/************************************************************************
 *	DEFINES
 */
/* Bit Manipulation */
#define CHK(x,b) (x&b)
#define CLR(x,b) (x&=~b)
#define SET(x,b) (x|=b)
#define TOG(a,b) (a^=b)

/************************************************************************
 *	FUNCTION PROTOTYPES
 */
void GPIO_Init( void );
void INT_Init( void );
void Timer_Init( void );

#endif /* INIT_H_ */