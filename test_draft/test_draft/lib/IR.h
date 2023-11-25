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

// PIN DEFINITION
#define IR_BIT PORTD2	// DDD2 == PORTD2 == PIND2 == 2
#define IR_PIN PIND

// DECODING DEFINITION
#define START_CNT (9000 / 4)
#define SHORT_CNT (560  / 4)
#define LONG_CNT  (1690 / 4)
#define BIAS      (80   / 4)
#define MAX_COUNT 100

#ifndef IR_H_
#define IR_H_

void init_IR(); // initialize Infrared Receiver



#endif /* IR_H_ */