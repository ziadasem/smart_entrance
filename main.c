#include "main.h"


uint8_t hoursLogged[500] = {0} ;
uint8_t minutesLogged[500]= {0};
volatile int pepoleEntered = 0 ;

static volatile int clockSettingIndex = 0 ;

static volatile int hours = 0;
static volatile int minutes = 0;

int tot_overflow ;
int enableEntrance = 0;
int globalEntrance  = ENTRANCE_WITH_PASS;

char savedPassword[4] ="----" ; 
int savedPasswordIndex = 0;

static char password[4]  ;
int insertedCount= 0;
int tot_overflow  = 0;
char start_flag = ' ' ;

int showPassword = 0 ;

char topLineText[] = "Welcome";


int main(void)
{
	LCD_init();
	USART_Init(57600);	
	KEYPAD_init();
	timer0_init();	

	//LOAD Password from EEPROM
	_load_password();
	
	LCD_write_command(0x01);
	LCD_write_string(topLineText);
	LCD_write_command(0xC0);
	
	initUltrasonic();
	DDRD |= (1<<DDD2);
	PORTD |= (1<<PORTD2);

	DDRD |= (1<<DDD3);
	if (globalEntrance == NO_ENTRANCE || globalEntrance == ENTRANCE_WITH_PASS){
		ENTRANCE_PORT &=~ (1<<ENTRANCE_LED);
	}else if (globalEntrance == ENTRANCE){
		ENTRANCE_PORT |= (1<<ENTRANCE_LED);
	}
	
	sei();
 	while(1)
	{
		uint8_t keypadVal = KEYPAD_read();
		if (keypadVal != 0 && globalEntrance == ENTRANCE_WITH_PASS){
			_delay_ms(100);
			_respond_to_char(keypadVal);
		}else if (globalEntrance != ENTRANCE){
			monitorUS();
		}
		
	}
	return 0;
}


void _respond_to_char(char val){
	if (insertedCount > 3){
		insertedCount = 0 ;
		 Clear_Second_Line();
	}
	if (_is_number(val)){
		_validate_password(val);
	}else if (_is_command_char(val)){
		_do_action(val);
	}	
}

int _is_number(char val){
	int valCode = (int) val ;
	if (valCode > 47 && valCode<58){
		return 1 ;
	}
	return 0 ;
}

int _is_command_char(char val){
	int valCode = (int) val ;
	if (valCode == 35 || valCode == 42 || (valCode >= 65 && valCode<=68)){
		return 1 ;
	}
	return 0 ;
}

void _do_action(char val){
	if (val == 42){ //show/hide password
		_show_hide_password();
	}else if (val  == 35 && insertedCount > 0){
		clear_charachter();
	}
}

void _show_hide_password(){
	Clear_Second_Line();
	if (showPassword == 0){	
		for (int i = 0 ; i < insertedCount ; i++){			
			LCD_write_data(password[i]);
		}
		showPassword = 1 ;
	}else{
		for (int i = 0 ; i < insertedCount ; i++){
			LCD_write_data(42);
		}
		showPassword = 0 ;
	}	
}
void _validate_password(char val){

	password[insertedCount++]  = val;
	LCD_write_data(showPassword ==1 ?val : 42);   
	 if(insertedCount == 4){
		 Clear_Second_Line();
		 
		 if (compareString(password, savedPassword, 4)){	
			 char str[] = "Done at";
			 LCD_write_string(str);
			 get_clock();
			 handle_success_entrance();
		}else{
			 char str[] = "Wrong";
			  LCD_write_string(str);
		 }
	 }	 
}

void clear_charachter(){
	Clear_Second_Line();
	for (int i= 0 ; i<insertedCount -1 ; i++ )
	{
		 LCD_write_data(showPassword ==1 ?password[i] : 42); 
	}
	insertedCount -- ;
}


void _save_password(){
		eeprom_busy_wait();					
		eeprom_write_block(savedPassword,0,4);
}

void _load_password(){
	memset(savedPassword,0,4);
	eeprom_read_block(savedPassword,0,4);
	
	globalEntrance = eeprom_read_byte((uint8_t*)10);

}

void handle_success_entrance(){
	enableEntrance = 1 ;
	ENTRANCE_PORT |= (enableEntrance<<ENTRANCE_LED);
	
	hoursLogged[pepoleEntered]=hours;
	minutesLogged[pepoleEntered]= minutes;
	pepoleEntered ++ ;
	_send_single_log();
}


void _send_single_log(){
	USART_TxChar(ENTRANCE_HEADER);
	_delay_ms(50);
	USART_TxChar('1');
	_delay_ms(50);
	USART_TxChar(END_PACKET);
}

void _send_all_logs(){
	USART_TxChar(LOGS_HEADER);
	_delay_ms(50);
	for(int i =0 ; i < pepoleEntered ; i++){
		USART_TxChar((char) hoursLogged[i] + OFFSET_CLOCK);
		_delay_ms(50);
		USART_TxChar((char)minutesLogged[i] + OFFSET_CLOCK);
		_delay_ms(50);
	}
	USART_TxChar(END_PACKET);
}


void Clear_Second_Line(){
	 LCD_write_command(0x01);
	 LCD_write_string(topLineText);
	 LCD_write_command(0xC0);
 }


int compareString(char* string1,char* string2, int length){
	for (int i= 0 ; i < length ; i++){
		if (string1[i] != string2[i] ){
			return 0 ;
		}
	}
	return 1 ;
}

// initialize timer, interrupt and variable
void timer0_init()
{
	// set up timer with prescaler = 1024
	TCCR0B |= (1 << CS02) | (1 << CS00);
	
	// initialize counter
	TCNT0 = 0;
	// enable overflow interrupt
	TIMSK0 |= (1 << TOIE0);
	tot_overflow = 0;
}

void get_clock(){
	char currentClock[5] = "     ";
	if (hours <10){
		currentClock[0] = '0';
		currentClock[1] = (char) hours +48 ;
	}else{
		currentClock[0] = (char) (hours /10) + 48 ;
		currentClock[1] = (char) (hours %10) + 48 ;
	}
	currentClock[2] = ':';
	
	if (minutes <10){
		currentClock[3] = '0';
		currentClock[4] = (char) minutes +48 ;
		}else{
		currentClock[3] = (char) (minutes /10) + 48 ;
		currentClock[4] = (char) (minutes %10) + 48 ;
	}
	for (int i =0 ; i <5 ; i++){
		LCD_write_data(currentClock[i]);	
	}
}

void monitorUS(){
	int dis =ultrasonic_distance();
	if (dis <20){ //somebody in
		if (enableEntrance  == 1 && globalEntrance != NO_ENTRANCE){
			while(ultrasonic_distance() < 40);
			_delay_ms(1000);
			enableEntrance = 0;
			ENTRANCE_PORT &=~ (1<<ENTRANCE_LED);
		
		}else{
				_delay_ms(300);
				if (dis >20){
					return;
				}
				USART_TxChar(BREAK_IN_ALERT);
				_delay_ms(50);
				USART_TxChar('1');
				_delay_ms(50);
				USART_TxChar(END_PACKET);
				
				PORTD &=~ (1<<PORTD2);
				while(ultrasonic_distance() < 40);
				_delay_ms(1200);
				PORTD |= (1<<PORTD2);

		}
	}
}


ISR(USART_RX_vect){
	_filter_recieved_char(UDR0);
}
ISR(TIMER0_OVF_vect)
{
	tot_overflow++;
	if (tot_overflow >= 3665) // NOTE: '>=' used instead of '=='
	{
		minutes ++ ;
		if (minutes >= 60){
			minutes= 0 ;
			hours ++ ;
			if (hours >23){
				hours = 0;
			}
		}
		tot_overflow = 0;   // reset overflow counter
	}
}




  
