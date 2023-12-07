/*
 * SHT30.h
 *
 * Created: 2023/12/6 19:26:55
 *  Author: 15217
 */ 

#ifndef SHT30_H_
#define SHT30_H_

#define SHT30_ADDR 0x44 // SHT30 I2C address

void triggerMeasurement();
void readTH(char* buffer);

#endif /* SHT30_H_ */