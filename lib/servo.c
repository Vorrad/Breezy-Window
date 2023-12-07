#include <avr/io.h>
#include <util/delay.h>

#include "servo.h"

long mapServoAngle(long x, long in_min, long in_max, long out_min, long out_max);

int angle = 0;


void setupPWM() {
	
	// set D5 (OC0B) as output
	DDRD |= (1 << DDD5);

	// prescaler factor = 1024
	TCCR0B |=  (1 << CS00);
	TCCR0B &= ~(1 << CS01);
	TCCR0B |=  (1 << CS02);
	
	// Phase correct PWM mode (TOP = OCR0A)
	TCCR0A |=  (1 << WGM00);
	TCCR0A &= ~(1 << WGM01);
	TCCR0B |=  (1 << WGM02);
	
	// Compare output mode settings: toggle at OCR0A -- determines the frequency
	TCCR0A &= ~(1 << COM0A1);
	TCCR0A |=  (1 << COM0A0);
	// Compare output mode settings: clear OCR0B when up counting -- determines the duty cycle
	TCCR0A |=  (1 << COM0B1);
	TCCR0A &= ~(1 << COM0B0);

	
	// 155 for 50 Hz
	OCR0A = 155;
	
}

void Open_window_to(uint8_t angle) {
	if (angle > 96)
	{
		angle = 96;  // Limit the angle to 96 degrees
	}
	if (angle < 0){
		angle = 0;
	}
	
	
	uint8_t units = angle / 12; //12 is unit degree it will rotate with OCR0B
	// Calculate the OCR0B value
	OCR0B= 3 + units; // 3 is the least value the OCR0B can be to drive the motor (0 degree)
	_delay_ms(10000);

}


void rotateServo(char direction) {
	uint8_t currentOCR0B = OCR0B; // Current OCR0B value, initialized to initial value
	if (direction == '+' && currentOCR0B < 11) {
		
		currentOCR0B++;
		} else if (direction == '-' && currentOCR0B > 3) {
		
		currentOCR0B--;
	}

	// Set the new OCR0B value
	OCR0B = currentOCR0B;
	_delay_ms(10000);
}