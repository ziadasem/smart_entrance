#ifndef PC_CONTROL_H_
#define PC_CONTROL_H_

#include "../std_macros.h"

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



extern int enableEntrance ;
extern int globalEntrance  ;
extern int savedPasswordIndex ;
extern char savedPassword[4];
extern char start_flag;
extern volatile int pepoleEntered ;
extern uint8_t hoursLogged[500] ;
extern uint8_t minutesLogged[500];




void _filter_recieved_char(char val);
void _update_password(char input);
void _update_clock(char input);
void _send_all_logs(void);
void _send_single_log(void);



#endif /* PC_CONTROL_H_ */