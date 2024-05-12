
#include "LCD.h"
#include "../std_macros.h"

void LCD_init(void){
	
INIT_LCD_PIN; 

LCD_write_command(0x3);// function set: 8bit,1 line , 5*7 dots 
_delay_ms(4);
LCD_write_command(0x3);
_delay_ms(4);
LCD_write_command(0x3);
_delay_ms(4);

LCD_write_command(0x2); // enable 4bit mode
LCD_write_command(0x28); // function set: 4bit,2 line , 5*7 dots
LCD_write_command(0x08);// start writing first place first row
LCD_write_command(0x1);// clear the display
LCD_write_command(0x6); //to make the cursor increment to the right
LCD_write_command(0xc);// turn off the cursor, turn on the display

_delay_ms(20);

}
/*
function name: LCD_write_data
purpose:write  data to LCD
parameters: data : one of LCD characters
return value:void
*/
void LCD_write_data(uint8_t data){
	RS(1);
	D7(READBIT(data,7));
	D6(READBIT(data,6));
	D5(READBIT(data,5));
	D4(READBIT(data,4));
	EN(1);
	_delay_ms(1);
	EN(0);
	_delay_ms(1);

D7(READBIT(data,3));
D6(READBIT(data,2));
D5(READBIT(data,1));
D4(READBIT(data,0));
EN(1);
_delay_ms(1);
EN(0);
_delay_ms(1);
}

/*
function name: LCD_write_command
purpose:write  command to LCD
parameters: data : one of LCD commands
return value:void
*/
void LCD_write_command(uint8_t data){
RS(0);
D7(READBIT(data,7));
D6(READBIT(data,6));
D5(READBIT(data,5));
D4(READBIT(data,4));
EN(1);
_delay_ms(1);
EN(0);
_delay_ms(1);

D7(READBIT(data,3));
D6(READBIT(data,2));
D5(READBIT(data,1));
D4(READBIT(data,0));
EN(1);
_delay_ms(1);
EN(0);
_delay_ms(1);
}
/*
function name: LCD_write_string
purpose:write  string
parameters: data array of char
return value:void
*/
void LCD_write_string(uint8_t *data){
	//make sure that the each byte is not null
	uint8_t i=0;
	while(data[i]!=0)
	{
		LCD_write_data(data[i]);
	    i++;
	}
	
}
/*
if we have: 1234 , how we show it on the LCD? 
1- taking the modulus %10 of the number will give me the most right digit, ex: 12345%10=5
2- store that number in an array
3- remove that number from the main number by dividing by 10,ex: 12345/10=1234
4- repeat until main number =0
5- when print on the lcd, we want the most left number be shown first, so we will make a loop to start from the last stored element in the array 



function name: LCD_write_number
purpose:write  number
parameters: number 
return value:void
*/
void LCD_write_number(uint32_t data){
	uint8_t i=0;
	uint8_t arr[10];
	if (data==0)
	{
		LCD_write_data('0');
		//this return acts as break.
		return;
	}
	while(data>0)	
	{
		arr[i]=data%10;
		data=data/10;
		i++;
	}
	while(i!=0)
	{
	i--;
	LCD_write_data(arr[i]+48);
	}
		
}
 
