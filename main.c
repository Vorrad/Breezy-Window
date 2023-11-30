/*
 * test_draft.c
 *
 * Created: 2023/11/17 14:12:28
 * Author : Chen Chen
 */ 


#include <avr/io.h>
#include <util/delay.h>

#include "lib/common.h"
#include "lib/uart.h"
#include "lib/IR.h"
#include "lib/stepper.h"
#include "lib/BH1750.h"


int main(void) {	
	
	UART_init(BAUD_PRESCALER);
	init_IR();
	init_stepper();
	TWI_init();
	BH1750_init();
	
	while (1){
		BH1750_read();
		_delay_ms(2000);
	}
}

