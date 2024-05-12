#include "../std_macros.h"
#ifndef US_
#define US_
#define  F_CPU 16000000UL
#define	 Trigger_port   PORTB		//Trigger port
#define  Trigger_DDR	DDRB		//Trigger DDDR
#define  Trigger_pin	5		// Trigger pin  pin 13

void initUltrasonic(void);
int getDistance(void);
int ultrasonic_distance(void);

#endif 