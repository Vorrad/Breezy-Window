#include <avr/io.h>

#include "common.h"
#include "BH1750.h"
#include "uart.h"


void TWI_init() {
	TWBR = ((F_CPU / F_SCL) - 16) / 2;
	TWCR = (1 << TWEN); // ����TWI
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
	TWI_write(BH1750_ADDR << 1); // ���͵�ַ+дλ
	TWI_write(BH1750_CMD_CONT_HIGH_RES_MODE); // ��������
	TWI_stop();
}

void BH1750_read(){
	
	static uint16_t lux = 0;
	
	TWI_start();
	TWI_write((BH1750_ADDR << 1) | 1); // ���͵�ַ+��λ
	lux = TWI_read_ACK();
	lux = (lux << 8) | TWI_read_NACK(); // ��ȡ�����ֽ�
	TWI_stop();
	
	// ����ͷ���luxֵ�Ĵ���
	static char buf[30];
	sprintf(buf, "lux: %u\n", lux);
	UART_putstring(buf);
}