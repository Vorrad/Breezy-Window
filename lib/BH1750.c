#include <avr/io.h>

#include "common.h"
#include "TWI.h"
#include "BH1750.h"



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