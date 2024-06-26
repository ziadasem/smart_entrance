
#ifndef STD_MACROS_H_
#define STD_MACROS_H_

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define SETBIT(REG,BIT) (REG |= 1<<BIT)
#define CLRBIT(REG,BIT) (REG &= ~(1<<BIT))
#define TOGBIT(REG,BIT) (REG ^= 1<<BIT)
#define READBIT(REG,BIT)  ((REG >> BIT) & 1)


typedef unsigned char uint8;
typedef unsigned short uint16; 
typedef unsigned long uint32;


#endif /* STD_MACROS_H_ */