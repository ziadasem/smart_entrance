/*
 * main.h
 *
 * Created: 5/12/2024 3:35:20 AM
 *  Author: Ziad
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <avr/io.h>
#include <avr/eeprom.h>
#include <string.h>

#include "Drivers/USART.h"
#include "Drivers/LCD.h"
#include "Drivers/keypad.h"
#include "Drivers/ultrasonic.h"

#include "Fetaures/PC_Control.h"


#define ENTRANCE_PORT PORTD
#define ENTRANCE_LED PORTD3

#define SET_PASSWORD 'S' //S
#define LOGS_HEADER 'L'
#define ENTRANCE_HEADER 'E'
#define ACKNOWLEDGE 'K'
#define BREAK_IN_ALERT 'A'
#define CLOCK_SETTING_HEADER 'C'
#define GLOBAL_ENTRANCE_HEADER 'D'

#define OFFSET_CLOCK 35

#define END_PACKET '>'  //62

#define NO_ENTRANCE 0
#define ENTRANCE_WITH_PASS 1
#define ENTRANCE 2


void _respond_to_char(char val);
int _is_number(char val);
int _is_command_char(char val);
void clear_charachter(void);
void _do_action(char val);
void _show_hide_password(void);
void _validate_password(char val );
void _save_password(void);
void _load_password(void);
void handle_success_entrance(void);
void Clear_Second_Line(void);
int compareString(char* string1,char* string2, int length);
void timer0_init(void);
void get_clock(void);
void monitorUS(void);




#endif /* MAIN_H_ */