#ifndef STEPPER_H_
#define STEPPER_H_

void init_stepper();
void curtain_rolling(int8_t direction,int steps);	//2048 steps for 1 revolution
void curtain_roll2top();
void curtain_roll2bottom();

#endif /* STEPPER_H_ */