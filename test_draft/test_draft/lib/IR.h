/*
 * IR.h
 *
 * Created: 2023/11/17 14:51:57
 *  Author: Chen Chen
 */ 

/*
	Infrared Receiver Decoding Module
	Using Timer0 with prescaler 64

*/

// TYPE DEFINITION
#define DATA_TYPE int32_t
#define DATA_CNT_TYPE uint16_t

// PIN DEFINITION
#define IR_BIT PORTD2	// DDD2 == PORTD2 == PIND2 == 2
#define IR_PIN PIND

// DECODING DEFINITION
#define START_CNT 2000		// if duration more than 2000: start
#define LS_CNT	  280
#define MAX_COUNT 100

// CODE DEFINITION
#define ERROR_CODE 0xff
#define DITTO_CODE 0x0f

#ifndef IR_H_
#define IR_H_

void init_IR(); // initialize Infrared Receiver



#endif /* IR_H_ */