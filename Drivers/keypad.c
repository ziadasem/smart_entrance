
#include "KEYPAD.h"
const uint8_t keypad_matrix[4][4] =
{
	{'1','4','7','*'},
	{'2','5','8','0'},
	{'3','6','9','#'},
	{'A','B','C','D'},
};
unsigned char keypad[4][4] = {
	
    '1', '2',  '3', 'A',
	'4', '5',  '6', 'B',
	'7', '8', '9',  'C',
	'*', '0',  '#',  'D'
};

// PD0 to PD3 input pull up, PD4 to PD7 output
void KEYPAD_init(void){						
	DDRD &= 0x0F ;
	PORTD |= 0xF0; 
	
	DDRB |= 0b00011110;
	PORTB |= 0b00011110;
	//INIT_KEYPAD_PIN;
  
}
uint8_t KEYPAD_read(void){
	K4(0); K5(1); K6(1); K7(1);
	if(K0 == 0) return keypad_matrix[0][0];
	if(K1 == 0) return keypad_matrix[0][1];
	if(K2 == 0) return keypad_matrix[0][2];
	if(K3 == 0) return keypad_matrix[0][3];
	_delay_ms(10);
	
	K4(1); K5(0); K6(1); K7(1);
	if(K0 == 0) return keypad_matrix[1][0];
	if(K1 == 0) return keypad_matrix[1][1];
	if(K2 == 0) return keypad_matrix[1][2];
	if(K3 == 0) return keypad_matrix[1][3];
	_delay_ms(10);
	
	K4(1); K5(1); K6(0); K7(1);
	if(K0 == 0) return keypad_matrix[2][0];
	if(K1 == 0) return keypad_matrix[2][1];
	if(K2 == 0) return keypad_matrix[2][2];
	if(K3 == 0) return keypad_matrix[2][3];
	_delay_ms(10);
	
	K4(1); K5(1); K6(1); K7(0);
	if(K0 == 0) return keypad_matrix[3][0];
	if(K1 == 0) return keypad_matrix[3][1];
	if(K2 == 0) return keypad_matrix[3][2];
	if(K3 == 0) return keypad_matrix[3][3];
	_delay_ms(10);
	
	
	K4(1); K5(1); K6(1); K7(1);
	return 0;
}
unsigned char colloc, rowloc;

char keyfind()
{
	while(1)
	{
		DDRB |= 0x0F;
		PORTB |= 0x0F;

		DDRD &= 0x0F;
		PORTB |= 0x0F;
				

		do
		{
			PORTB = 0x0F;      /* mask PORT for column read only */
			asm("NOP");
			colloc = (PIND & 0xF0); /* read status of column */
		}while(colloc != 0xF0);
		
		do
		{
			do
			{
				_delay_ms(20);             /* 20ms key debounce time */
				colloc = (PIND & 0xF0); /* read status of column */
				colloc &= 0xF0 ;
				}while(colloc == 0xF0);        /* check for any key press */
				
				_delay_ms (40);	            /* 20 ms key debounce time */
				colloc = (PIND & 0xF0);
			}while(colloc == 0xF0);

			/* now check for rows */
			PORTB = 0xFE;            /* check for pressed key in 1st row */
			asm("NOP");
			colloc = (PIND & 0xF0);
			if(colloc != 0xF0)
			{
				rowloc = 0;
				break;
			}

			PORTB = 0xFD;		/* check for pressed key in 2nd row */
			asm("NOP");
			colloc = (PIND & 0xF0);
			if(colloc != 0xF0)
			{
				rowloc = 1;
				break;
			}
			
			PORTB = 0xFB;		/* check for pressed key in 3rd row */
			asm("NOP");
			colloc = (PIND & 0xF0);
			if(colloc != 0xF0)
			{
				rowloc = 2;
				break;
			}

			PORTB = 0xF7;		/* check for pressed key in 4th row */
			asm("NOP");
			colloc = (PIND & 0xF0);
			if(colloc != 0xF0)
			{
				rowloc = 3;
				break;
			}
		}

		if(colloc == 0x0E)
		return(keypad[rowloc][0]);
		else if(colloc == 0x0D)
		return(keypad[rowloc][1]);
		else if(colloc == 0x0B)
		return(keypad[rowloc][2]);
		else
		return(keypad[rowloc][3]);
	}