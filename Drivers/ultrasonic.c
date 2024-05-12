
#include "ultrasonic.h"

int TimerOverflow = 0;

//initialize timers and DIOs
void initUltrasonic(void){

	Trigger_DDR |= (1<<Trigger_pin); // Make trigger pin as output 
	TIMSK1 = (1 << TOIE1);			 // Enable Timer1 overflow interrupts 
	TCCR1A = 0;						//  Set all bit to zero Normal operation 
}

//sends trigger pulse for 10us to start sending waves
void sendTriggerPulse(void){
	Trigger_port |= (1 << Trigger_pin);
	_delay_us(10);
	Trigger_port &= (~(1 << Trigger_pin));
}

int getDistance(void)
{
	PORTB |= (1 << PORTB5);
	_delay_us(10);
	PORTB &= (~(1 << PORTB5));
	
	long count;
	int distance ;

	TCNT1 = 0;			// Clear Timer counter 
	TCCR1B = 0x41;		// Capture on rising edge, No prescaler
	TIFR1 = 1<<ICF1;	// Clear ICP flag (Input Capture flag)  -wait for the new
	TIFR1 = 1<<TOV1;	// Clear Timer Overflow flag 

	while ((TIFR1 & (1 << ICF1)) == 0);	// Wait for rising edge to start count
	TCNT1 = 0;			// Clear Timer counter to start new count
	TCCR1B = 0x01;		// Capture on falling edge, No prescaler 
	TIFR1 = 1<<ICF1;	// Clear ICP flag (Input Capture flag) to wait for the new flag --clear flag is by setting it
	TIFR1 = 1<<TOV1;	// Clear Timer Overflow flag 
	TimerOverflow = 0;  // Clear Timer overflow count 

	while ((TIFR1 & (1 << ICF1)) == 0);		// Wait for falling edge 
	count = ICR1 + (65535 * TimerOverflow);	// Take count 
	distance = count / 466.47;
	return distance;
}

int ultrasonic_distance(void){
	//initUltrasonic();
	sendTriggerPulse();
	int distance= getDistance();
	return distance ;
}

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	// Increment Timer Overflow count 
}
