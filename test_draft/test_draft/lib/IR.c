/*
 * IR.c
 *
 * Created: 2023/11/17 14:54:58
 *  Author: Chen Chen
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "uart.h"
#include "IR.h"

// to store date from the IR pulse
volatile uint8_t  ovf_flag;
// uint16_t data[100];

volatile int32_t data[MAX_COUNT];
volatile uint16_t data_cnt;

void clean_data()
{
	for (int i=0; i < MAX_COUNT; i++)
		data[i]=0;

	data_cnt = 0;
}

void init_CLK(){
	// TIMER1
	// Prescaler = 64
	TCCR1A &= 0;
	TCCR1B |= (1 << CS01) | (1 << CS00);
}

void init_IR(){
	
	init_CLK();
	clean_data();
	
	// D2 as input
	DDRD &= ~(1 << IR_BIT);
	// debug: D3 as output, showing D2's result
	DDRD |= (1 << DDD3);
	
	// pin change interrupt
	// D2 - PCINT18
	PCICR  |= (1 << PCIE2);
	PCMSK2 |= (1 << IR_BIT);
	
	// timer 1 overflow interrupt
	TIMSK1 |= (1 << TOIE1); // Enable Timer1 overflow interrupt
	
	// for DEBUG
	PCICR  |= (1 << PCIE0);
	DDRB   &= ~(1 << DDB0);
	PORTB  |= (1 << PORTB0);
	PCMSK0 |= (1 << PCINT0);
	
	sei();
}

ISR(TIMER1_OVF_vect){
	cli();
	ovf_flag += 1;
	sei();
}

// debug: check the results data
ISR(PCINT0_vect){
	cli();	
	if (!(PINB & (1 << PINB0)))
	{
		volatile char buf[50];
		for (int i=1; i< data_cnt; i++)
		{
			sprintf(buf, "[data %u] %ld\n",i , data[i]);
			UART_putstring(buf);
		}
	}
	
	clean_data();
	sei();
}

// debug: show D2 input at D3

ISR(PCINT2_vect){
	
	static volatile uint16_t last_time=0;
	static volatile uint16_t curr_time;
	
	// LOW detected
	if (!(IR_PIN & (1 << IR_BIT)))
	{
		curr_time = TCNT1;
		if (data_cnt < MAX_COUNT){			
			data[data_cnt] = -(curr_time - last_time + ovf_flag * 65536);
			data_cnt++;
			ovf_flag = 0;
			last_time = curr_time;
		}
	}
	else
	{
		curr_time = TCNT1;
		if (data_cnt < MAX_COUNT){
			data[data_cnt] = +(curr_time - last_time + ovf_flag * 65536);
			data_cnt++;
			ovf_flag = 0;
			last_time = curr_time;
		}
	}
}