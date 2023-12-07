/*
 * TWI.h
 *
 * Created: 2023/12/6 12:19:53
 *  Author: 15217
 */ 
#ifndef TWI_H_
#define TWI_H_

#define F_SCL 100000UL		// SCL frequency

void TWI_init();
void TWI_start();
void TWI_stop();
void TWI_write(uint8_t u8data);
uint8_t TWI_read_ACK();
uint8_t TWI_read_NACK();

#endif /* TWI_H_ */