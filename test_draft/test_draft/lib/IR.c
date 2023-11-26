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
volatile uint8_t  period_flag;

volatile int timer2_acc;

volatile DATA_TYPE data[MAX_COUNT];
volatile DATA_CNT_TYPE data_cnt;

void clean_data()
{
	for (int i=0; i < MAX_COUNT; i++)
		data[i]=0;

	data_cnt = 0;
}

void init_vars()
{
	ovf_flag = 0;
	period_flag = 0;
	timer2_acc = 0;
}

void init_CLK(){
	// TIMER1
	// Prescaler = 64
	TCCR1A &= 0;
	TCCR1B |= (1 << CS01) | (1 << CS00);
	
	// TIMER2
	// Prescaler = 1024
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
	TCCR2A &= ~(1 << WGM20);
	TCCR2A |= (1 << WGM21);
	TCCR2B &= ~(1 << WGM22);
	
	// set 90ms interval at the 6th time of TIMER2_COMPA interrupt
	OCR2A = 234;
	
	TIMSK2 |= (1 << OCIE2A);
}

void init_IR(){
	
	init_CLK();
	init_vars();
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

void dump_data()
{
	if (data_cnt != 0)
	{
		UART_putstring("--------------------------------------\n");
		for (int i=1; i< data_cnt; i++)
		{
			volatile char buf[50];
			sprintf(buf, "[data %u] %ld\n",i , data[i]);
			UART_putstring(buf);
		}
	}
}

void decode_data(uint8_t *output, DATA_TYPE d, DATA_CNT_TYPE c)
{
	// for data: c == 68
	// for dito: c ==  4
	switch (c){
		case  0: break;
		case  4:
		{
			if (data[0] > START_CNT && data[3] < LS_CNT)
			*output = DITTO_CODE;
			else
			UART_putstring("[ERR] unrecognized data\n");
			break;
		}
		case 68:
		{
			uint8_t tmp1=0, tmp2=0;
			for (int i=0; i<8; i++){
				tmp1 |= ( (data[2*i+36] < LS_CNT) ? 0 : 1) << i;
				tmp2 |= ( (data[2*i+52] < LS_CNT) ? 0 : 1) << i;
			}
			if ( (tmp1 ^ tmp2) == 0xff)
				*output = tmp1;
			else
				UART_putstring("[ERR] checksum failed\n");				
			break;
		}
		default:
		{
			*output = ERROR_CODE;
			UART_putstring("[ERR] unrecognized data\n");
		}
	}
}

ISR(TIMER1_OVF_vect){
	cli();
	ovf_flag++;
	sei();
}

// debug: manually dump data
ISR(PCINT0_vect){
	if (!(PINB & (1 << PINB0)))
	{
		dump_data();
		clean_data();
	}
}

// debug: show D2 input at D3

ISR(PCINT2_vect){
	
	static volatile uint16_t last_time=0;
	static volatile uint16_t curr_time;
	
	curr_time = TCNT1;
	
	// reset data dumping timer (timer2)
	if (!period_flag){
		TCNT2 = 0;
		timer2_acc = 0;
		period_flag = 1;
	}
	if (data_cnt < MAX_COUNT){

		// now LOW, data was HIGH
		if (!(IR_PIN & (1 << IR_BIT)))	
			data[data_cnt] = +(curr_time - last_time + ovf_flag * 65536);
		else // now HIGH, data was LOW
			data[data_cnt] = -(curr_time - last_time + ovf_flag * 65536);
		data_cnt++;
		ovf_flag = 0;
		last_time = curr_time;
	}
}

ISR(TIMER2_COMPA_vect) {
	timer2_acc++;
	
	// triggered every 90 ms
	if (timer2_acc == 6 && data_cnt != 0){
		uint8_t d;
		decode_data(&d, data, data_cnt);
		char buf[30];
		sprintf(buf, "decoded data:%u\n", d);
		UART_putstring(buf);
		clean_data();
		timer2_acc = 0;
		period_flag = 0;
	}
}

