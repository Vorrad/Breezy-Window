#include <avr/io.h>
#include <util/delay.h>

#include "servo.h"

long mapServoAngle(long x, long in_min, long in_max, long out_min, long out_max);

int angle = 0;
 


void setupPWM() {
	DDRB |= (1 << DDB2); // Set PB2 as an output for OC1B

	// Set Fast PWM mode with OCR1A as TOP
	TCCR1A |= (1 << WGM11) | (1 << WGM10);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);

	// Set non-inverted mode for OC1B
	TCCR1A |= (1 << COM1B1);

	// Set prescaler to 8
	TCCR1B |= (1 << CS11);

	// Set OCR1A for a period of 20ms (50Hz frequency)
	OCR1A = 39999; // TOP value for the timer, controlling the frequency
}

void setServoPulseWidth(int pulseWidth) {
	// Set OCR1B to control the duty cycle for OC1B
	OCR1B = pulseWidth; // Pulse width for servo position
}

long mapServoAngle(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Window_Open_10deg ()
{
	
	while (angle<90)
	{
		for (int i=0; i<=10; i++) {
			angle++;
			setServoPulseWidth(mapServoAngle(angle, 0, 180, 1000, 5000));
			_delay_ms(500);
	}
	
}
}

void Window_close_10deg ()
{
	
	while (angle>0)
	{
		for (int i=0; i<=10; i++) 
		{
			angle--;
			setServoPulseWidth(mapServoAngle(angle, 0, 180, 1000, 5000));
			_delay_ms(500);
		}
		
	}
}