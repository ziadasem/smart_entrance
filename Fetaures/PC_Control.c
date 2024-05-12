#include "../std_macros.h"
#include "PC_Control.h"

static volatile int clockSettingIndex = 0 ;
static volatile int hours = 0;
static volatile int minutes = 0;


void _filter_recieved_char(char val){
	
	if (val == SET_PASSWORD){
		start_flag = SET_PASSWORD ; //set password
		savedPasswordIndex = 0 ;
		}else if (val == CLOCK_SETTING_HEADER){
		start_flag = CLOCK_SETTING_HEADER ; //set password
		clockSettingIndex = 0 ;
	}
	else if (val == LOGS_HEADER){
		start_flag = LOGS_HEADER ; //set password
	}
	else if(val == GLOBAL_ENTRANCE_HEADER){
		start_flag = GLOBAL_ENTRANCE_HEADER ;
	}
	else if (val == END_PACKET){//end char
		start_flag = ' ' ; //end packet
	}
	
	
	else if(start_flag == SET_PASSWORD){
		_update_password(val);
		}else if (start_flag == CLOCK_SETTING_HEADER){
		_update_clock(val);
		}else if (start_flag == LOGS_HEADER){
		_send_all_logs();
		start_flag = ' ';
		}else if (start_flag == GLOBAL_ENTRANCE_HEADER){
		
		globalEntrance = val - 48 ;
		eeprom_update_byte((uint8_t*)10, globalEntrance);
		if (globalEntrance == NO_ENTRANCE || globalEntrance == ENTRANCE_WITH_PASS){
			ENTRANCE_PORT &=~ (1<<ENTRANCE_LED);
			}else if (globalEntrance == ENTRANCE){
			ENTRANCE_PORT |= (1<<ENTRANCE_LED);
		}
	}
	else if (globalEntrance == ENTRANCE_WITH_PASS){
		_respond_to_char(val);
	}
}

void _update_password(char input){
	savedPassword[savedPasswordIndex ++] = input ;
	if (savedPasswordIndex >3  ){
		//LCD_write_string(savedPassword);
		_save_password();
		USART_TxChar(ACKNOWLEDGE);
		_delay_ms(50);
		USART_TxChar('1');
		_delay_ms(50);
		USART_TxChar(END_PACKET);
		_delay_ms(50);

	}
}

void _update_clock(char input){
	int temp = (int) input ;
	temp -= OFFSET_CLOCK ;
	if (clockSettingIndex == 0){
		clockSettingIndex++ ;
		hours = temp ;
		USART_TxChar(hours +OFFSET_CLOCK);
		_delay_ms(10);
		}else{
		clockSettingIndex++ ;
		minutes = temp ;
		USART_TxChar(minutes +OFFSET_CLOCK);
		_delay_ms(10);

	}
	
void _send_single_log(void){
		USART_TxChar(ENTRANCE_HEADER);
		_delay_ms(50);
		USART_TxChar('1');
		_delay_ms(50);
		USART_TxChar(END_PACKET);
	}

void _send_all_logs(void){
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

}