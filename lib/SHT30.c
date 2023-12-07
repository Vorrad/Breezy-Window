#include <avr/io.h>
#include <util/delay.h>

#include "SHT30.h"
#include "TWI.h"
#include "uart.h"
#include "common.h"


char String[100];

// SHT30 command to trigger measurement in high repeatability with clock stretching
#define SHT30_CMD_MEASURE_HIGH 0x2C06

void readTemperature();
void triggerMeasurement();


void triggerMeasurement() {
	TWI_start();
	TWI_write(SHT30_ADDR << 1);
	TWI_write(SHT30_CMD_MEASURE_HIGH >> 8); // Write MSB of the command
	TWI_write(SHT30_CMD_MEASURE_HIGH & 0xFF); // Write LSB of the command
	TWI_stop();

	_delay_ms(500); // Wait for measurement to complete
	
	 
}

void readTH(char* buffer) {
	// Read temperature data (first 3 bytes: 2 for temperature, 1 for CRC)
	TWI_start();
	UART_putstring("TWI_start\n");
	TWI_write((SHT30_ADDR << 1) | 1); // Send address with read bit
	UART_putstring("TWI_write ADDR\n");

	uint8_t tempMSB = TWI_read_ACK();
	uint8_t tempLSB = TWI_read_ACK();
 	TWI_read_ACK(); //skip
	uint8_t humMSB = TWI_read_ACK();
	uint8_t humLSB = TWI_read_ACK();
	 
	TWI_read_NACK(); // CRC byte, ignored for simplicity
	UART_putstring("TWI_read_NACK\n");
	TWI_stop();
	UART_putstring("TWI_stop\n");
	 
 	
	uint16_t rawTemperature = (tempMSB << 8) | tempLSB;
	uint16_t rawHumidity = (humMSB << 8) | humLSB;
	int temperature = (int)(-45 + 175 * (rawTemperature / 65535.0));
	int humidity = (int)(100 * (rawHumidity / 65535.0));
	
	sprintf(buffer,"%d%d",temperature, humidity);
	
	return;
}
