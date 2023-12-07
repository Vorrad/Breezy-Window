#include <avr/io.h>
#include <util/delay.h>

#include "stepper.h"

volatile int stepper_angle=0;

// Functions
void step_CW( void );

//** GLOBAL VARIABLES **
int phase_step = 1; // We will use wave stepping, so the steps are numbered 1-4, then will repeat.  

void init_stepper()
{
   DDRB = 0b00001111;
}

void curtain_roll2top(){
	if (stepper_angle < 6144){
		for (int i=stepper_angle; i < 6144; i++){
			step_CW();
			_delay_ms(40);
		}
		stepper_angle = 6144;
	}
}

void curtain_roll2bottom(){
	if (stepper_angle > 0){
		for (int i=stepper_angle; i > 0; i--){
			step_CCW();
			_delay_ms(40);
		}
		stepper_angle = 0;
	}
}

void curtain_rolling (int8_t direction, int steps)//2048 steps for 1 revolution
{
	if (direction>0)
	{
		if (stepper_angle + steps < 6144){			
			for (int i=0;i<steps;i++)
			{
				step_CW();
				_delay_ms(40);
				stepper_angle++;
			}
		}
		else{
			for (int i=stepper_angle; i < 6144; i++)
			{
				step_CW();
				_delay_ms(40);
				stepper_angle++;
			}
		}
	}
	else
	{
		if (stepper_angle - steps > 0){			
			for (int i=0; i<steps; i++)
			{
				step_CCW();
				_delay_ms(40);
				stepper_angle--;
			}
		}
		else{
			for (int i=stepper_angle; i>0; i--){
				step_CCW();
				_delay_ms(40);
				stepper_angle--;
			}
		}
	}
}


void step_CCW() {
	switch (phase_step) {
		case 1:
		PORTB = 0b00001000;
		phase_step = 2;
		break;
		case 2:
		PORTB = 0b00000100;
		phase_step = 3;
		break;
		case 3:
		PORTB = 0b00000010;
		phase_step = 4;
		break;
		case 4:
		// step to 3;
		PORTB = 0b00000001;
		phase_step = 1;
		break;
	}
}


void step_CW() 
{
	switch (phase_step) 
	{
		case 1:
		PORTB = 0b00000001;
		phase_step = 2;
		break;
		case 2:
		PORTB = 0b00000010;
		phase_step = 3;
		break;
		case 3:
		PORTB = 0b00000100;
		phase_step = 4;
		break;
		case 4:
		// step to 3;
		PORTB = 0b00001000;
		phase_step = 1;
	    break;  
	}
}


