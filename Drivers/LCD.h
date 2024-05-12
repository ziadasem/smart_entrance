#ifndef LCD_H_
#define LCD_H_
#include "../std_macros.h"
#define INIT_LCD_PIN DDRC|=0b00111111;
#define EN(x) if (x==1) SETBIT(PORTC,0); else CLRBIT(PORTC,0); 
#define RS(x) if (x==1) SETBIT(PORTC,1); else CLRBIT(PORTC,1); 

#define D7(x) if (x==1) SETBIT(PORTC,5); else CLRBIT(PORTC,5); 
#define D6(x) if (x==1) SETBIT(PORTC,4); else CLRBIT(PORTC,4); 
#define D5(x) if (x==1) SETBIT(PORTC,3); else CLRBIT(PORTC,3); 
#define D4(x) if (x==1) SETBIT(PORTC,2); else CLRBIT(PORTC,2); 


void LCD_init(void);
void LCD_write_data(uint8_t data);
void LCD_write_command(uint8_t data);
void LCD_write_string(uint8_t *data);
void LCD_write_number(uint32_t data);
void LCD_write_number(uint32_t data);

#endif /* LCD_H_ */