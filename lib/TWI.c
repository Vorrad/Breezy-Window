///*
 //* TWI.c
 //*
 //* Created: 2023/12/6 12:19:35
 //*  Author: 15217
 //*/ 


#include <avr/io.h>
#include <util/twi.h>

#include "common.h"
#include "TWI.h"
#include "uart.h"

void TWI_init() {
	TWSR = 0; // Set prescaler to zero
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