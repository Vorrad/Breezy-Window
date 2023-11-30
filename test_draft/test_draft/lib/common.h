#ifndef COMMON_H
#define COMMON_H

#define F_CPU 16000000UL
#define BAUD_RATE 9600UL
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)


#endif