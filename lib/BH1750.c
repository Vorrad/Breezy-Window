#include <avr/io.h>

#include "common.h"
#include "BH1750.h"


void TWI_init() {
	TWBR = ((F_CPU / F_SCL) - 16) / 2;
	TWCR = (1 << TWEN);		// Enable TWI
}

void TWI_start() {
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

void TWI_stop() {
	TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
}

void TWI_write(uint8_t u8data) {
	TWDR = u8data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

uint8_t TWI_read_ACK() {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t TWI_read_NACK() {
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

void BH1750_init() {
	TWI_init();
	TWI_start();
	TWI_write(BH1750_ADDR << 1);					// device address | write
	TWI_write(BH1750_CMD_CONT_HIGH_RES_MODE);		// set 
	TWI_stop();
}

uint16_t BH1750_read(){
	
	static uint16_t lux = 0;
	
	TWI_start();
	TWI_write((BH1750_ADDR << 1) | 1); // 发送地址+读位
	lux = TWI_read_ACK();
	lux = (lux << 8) | TWI_read_NACK(); // 读取两个字节
	TWI_stop();
	
	return lux;
}