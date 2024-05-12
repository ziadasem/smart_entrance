#ifndef KEYPAD_H_
#define KEYPAD_H_
#include "../std_macros.h"

#define K0 READBIT(PIND,4)
#define K1 READBIT(PIND,5)
#define K2 READBIT(PIND,6)
#define K3 READBIT(PIND,7)
#define K4(x) if(x == 1) SETBIT(PORTB,1); else CLRBIT(PORTB,1);
#define K5(x) if(x == 1) SETBIT(PORTB,2); else CLRBIT(PORTB,2);
#define K6(x) if(x == 1) SETBIT(PORTB,3); else CLRBIT(PORTB,3);
#define K7(x) if(x == 1) SETBIT(PORTB,4); else CLRBIT(PORTB,4);

void KEYPAD_init(void);
uint8_t KEYPAD_read(void);
char keyfind(void);


#endif /* KEYPAD_H_ */