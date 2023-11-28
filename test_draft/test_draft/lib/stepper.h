#ifndef STEPPER_H_
#define STEPPER_H_

void init_stepper();
void curtain_rolling(uint8_t direction,uint16_t steps);	//2048 steps for 1 revolution

#endif /* STEPPER_H_ */