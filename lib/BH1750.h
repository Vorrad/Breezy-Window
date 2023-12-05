#ifndef BH1750_H
#define BH1750_H

#define F_SCL 100000UL		// SCL frequency
#define BH1750_ADDR 0x23	// BH1750 I2C address
#define BH1750_CMD_CONT_HIGH_RES_MODE 0x10 // continuous high-res command

void BH1750_init();
uint16_t BH1750_read();

#endif 