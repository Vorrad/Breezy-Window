#ifndef COMMON_H
#define COMMON_H

#define F_CPU 16000000UL
#define BAUD_RATE 9600UL
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

// DECISION VARIABLES
#define LUX_THRESHOLD_HIGH	 500
#define LUX_THRESHOLD_LOW	 100

// PIN DEFINITION
#define IR_BIT		PORTD2	// DDD2 == PORTD2 == PIND2 == 2
#define IR_PIN		PIND
#define STEP_PIN	

extern uint8_t control_mode;

#endif