///*
 //* test_draft.c
 //*
 //* Created: 2023/11/17 14:12:28
 //* Author : Chen Chen
 //*/ 
//
//


#include <avr/io.h>
#include <util/delay.h>

#include "lib/common.h"
#include "lib/uart.h"
#include "lib/IR.h"
#include "lib/stepper.h"
#include "lib/servo.h"
#include "lib/BH1750.h"
#include "lib/ST7735.h"
#include "lib/LCD_GFX.h"
#include "lib/SHT30.h"

#define SLAVE_ADDR	0x09


enum mode control_mode=MANUAL;


void write_to_slave(char* buf){
	TWI_start();
	TWI_write(SLAVE_ADDR << 1);
	int i=0;
	while (buf[i] != '\0'){
		TWI_write(buf[i++]);
	}
	TWI_stop();
}

int main(void) {	
	
	UART_init(BAUD_PRESCALER);
	init_stepper();
	setupPWM();
	BH1750_init();
	init_IR();
	TWI_init();
	
	Open_window_to(0);
	
	char buf[30];
	while (1){
		
		// auto control
		if (control_mode == AUTO){
			
			// check illumination
			uint16_t lux = BH1750_read();
			UART_putstring("BH1750 complete\n");
			if (lux > LUX_THRESHOLD_HIGH){
				sprintf(buf, "lux:%u\n", lux);
				UART_putstring(buf);
				curtain_roll2top();
			}
			else if (lux < LUX_THRESHOLD_LOW){				
				sprintf(buf, "lux:%u\n", lux);
				UART_putstring(buf);
				curtain_roll2bottom();		
			}
			
			// get temperature and humidity
			triggerMeasurement();
			UART_putstring("trigger complete\n");
			char t_h[6];
			readTH(t_h);
			UART_putstring("readTH complete\n");
			
			
			// check humidity
			uint16_t hum = atoi(t_h) % 100;
			if (hum > HUM_THRESHOLD_HIGH){
				sprintf(buf, "hum:%u\n", hum);
				UART_putstring(buf);
				Open_window_to(0);
			}
			else if (hum < HUM_THRESHOLD_LOW){
				sprintf(buf, "hum:%u\n", hum);
				UART_putstring(buf);
				Open_window_to(48);
			}
			
			write_to_slave(t_h);
		}		
		_delay_ms(20000);
	}
}


//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//#include <util/twi.h>
//
//
//#include "lib/common.h"
//#include "lib/uart.h"
//#include "lib/ST7735.h"
//#include "lib/LCD_GFX.h"
//#include "lib/IR.h"
//
//// Define I2C Slave Address
//#define SLAVE_ADDR 0x09
//
//// Buffer for storing received data
//volatile char i2c_buffer[10];
//enum mode control_mode=AUTO;
//
//// Color definitions
//uint16_t white = 0xffff;
//uint16_t red = 0xf800;
//uint16_t blue = 0x041f;
//uint16_t grey = 0xce79;
//uint16_t green = 0x2668;
//uint16_t black = 0x0000;
//
//
//void I2C_init(uint8_t slave_address) {
	//TWAR = (slave_address << 1);
	//TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
//}
//
//ISR(TWI_vect) {	
	//static volatile uint8_t i2c_index = 0;
	//switch(TW_STATUS) {
		//case TW_SR_DATA_ACK:
		//// Received data from master, ACK sent
		//i2c_buffer[i2c_index++] = TWDR;
		//TWCR |= (1 << TWINT) | (1 << TWEA);
		//break;
		//case TW_SR_STOP:
		//// Stop or repeated start condition received
		//i2c_buffer[i2c_index] = '\0'; // Null-terminate the string
		//i2c_index = 0; // Reset buffer index
		//TWCR |= (1 << TWINT) | (1 << TWEA);
		//break;
		//default:
		//TWCR |= (1 << TWINT);
		//break;
	//}
//}
//
//void Initialize() {
	//UART_init(BAUD_PRESCALER); // Initialize UART
	//lcd_init(); // Initialize LCD
	//I2C_init(SLAVE_ADDR); // Initialize I2C
	//sei(); // Enable global interrupts
//}
//
//void screen_init() {
	//LCD_setScreen(white); // Set background to white
	//LCD_drawStringLarge(5, 5, "Temperature:", red, white, 2);
	//LCD_drawStringLarge(45, 25, "~C", red, white, 2);
	//LCD_drawStringLarge(5, 50, "Humidity:", blue, white, 2);
	//LCD_drawStringLarge(45, 75, "%", blue, white, 2);
//}
//
//
//int main(void) {
	//Initialize();
	//screen_init();
//
	//while (1) {
		//if (i2c_buffer[0] != '\0') { // Check if buffer has data
			//char buf1[3];
			//char buf2[3];
			//buf1[0] = i2c_buffer[0];
			//buf1[1] = i2c_buffer[1];
			//buf1[2] = '\0';
			//buf2[0] = i2c_buffer[2];
			//buf2[1] = i2c_buffer[3];
			//buf2[2] = '\0';
			//LCD_drawStringLarge(10, 25, buf1, red, white, 2); // Display the temperature
			//LCD_drawStringLarge(10, 75, buf2, blue, white, 2); // Display the humidity
		//}
		//_delay_ms(3000);
	//}
//}

