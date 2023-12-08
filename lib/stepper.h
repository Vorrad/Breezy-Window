#ifndef STEPPER_H_
#define STEPPER_H_

#define WORKING_DELAY 42
#define WORKING_RANGE 5120

void init_stepper();
void curtain_rolling(int8_t direction,int steps);	//2048 steps for 1 revolution
void curtain_roll2top();
void curtain_roll2bottom();

#endif /* STEPPER_H_ */