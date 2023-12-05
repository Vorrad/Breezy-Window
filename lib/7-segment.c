#include <avr/io.h>
#include <util/delay.h>

// Define the port for the segments (including DP)
#define SEGMENT_PORT PORTD
#define SEGMENT_DDR DDRD

// Define the port for the digits
#define DIGIT_PORT PORTB
#define DIGIT_DDR DDRB

// Segment codes for 0-9 and C (in a common cathode setup, change for common anode)
uint8_t segment_codes[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x39, 0x80}; // Add 0x80 for DP

void display_digit(int digit, int number);

int main(void)
{
	// Initialize the segment and digit pins as outputs
	SEGMENT_DDR = 0xFF; // Assuming all 8 pins of PORTD are connected to segments + DP
	DIGIT_DDR = 0x0F;   // Assuming first 4 pins of PORTB are connected to digits

	while (1)
	{
		display_digit(3, 2); 
		_delay_ms(5); // Adjust delay for visibility
		display_digit(2, 5); 
		_delay_ms(5); // Adjust delay for visibility
		display_digit(1, 10);
		_delay_ms(5); // Adjust delay for visibility
	}
}

void display_digit(int digit, int number) {
	SEGMENT_PORT = segment_codes[number]; // Set the segments for the number
	DIGIT_PORT = (DIGIT_PORT & 0xF0) | ~(1 << digit); // Activate only the selected digit
	_delay_ms(1); // Very brief delay
}