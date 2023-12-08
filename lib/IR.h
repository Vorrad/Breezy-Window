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

// DECODING DEFINITION
#define START_CNT 2000		// if duration more than 2000: start
#define LS_CNT	  280
#define MAX_COUNT 100

// CODE DEFINITION
#define ERROR_FLAG 0x55
#define DITTO_CODE 0xff

// MODE DEFINITION
enum mode {AUTO, MANUAL};
#define DEFAULT_MODE MANUAL

// COMMAND CODE DEFINITION
#define MODE_SELECT		71
#define CURTAIN_UP		70
#define CURTAIN_DOWN	21
#define WINDOW_OPEN		9
#define WINDOW_CLOSE	7

#ifndef IR_H_
#define IR_H_

void init_IR(); // initialize Infrared Receiver

#endif /* IR_H_ */
