#include <avr/io.h>
#include <util/delay.h>

// Functions
void step_CW( void );

//** GLOBAL VARIABLES **
int phase_step = 1; // We will use wave stepping, so the steps are numbered 1-4, then will repeat.  

void init()
{
   DDRB = 0b00001111;
}

int main(void)
{
    init();
	curtain_rolling(1,2048);
	
	
	while(1)
	{

    }
}

void curtain_rolling (uint8_t direction,uint16_t steps,uint16_t delay)//2048 steps for 1 revolution
{
	if (direction>0)
	{
		for(int i=0;i<steps;i++)
		{
			step_CW();
			_delay_ms(30);
		}
	}
	else
	{
		for(int i=0;i<steps;i++)
		{
			step_CCW();
			_delay_ms(30);
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


