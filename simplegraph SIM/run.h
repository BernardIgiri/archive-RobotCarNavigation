#define F_CPU 14745600
#include <stdio.h>

#define F_CPU 14745600

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/uart.h"
#include "servo.h"

#define ULTRA_SONIC_MASK (1<<4)

#define ULTRA_SONIC_OUT (DDRB|=ULTRA_SONIC_MASK)
#define ULTRA_SONIC_IN (DDRB&=~ULTRA_SONIC_MASK)
#define ULTRA_SONIC_SET (PORTB|=ULTRA_SONIC_MASK)
#define ULTRA_SONIC_RESET (PORTB&=~ULTRA_SONIC_MASK)
#define ULTRA_SONIC_IS_LOW (!(PINB&ULTRA_SONIC_MASK))
#define ULTRA_SONIC_IS_HIGH ((PINB&ULTRA_SONIC_MASK)==ULTRA_SONIC_MASK)

uint16_t ultra_sonic (void)
{
    uint16_t fcap=0,scap=0;
    ULTRA_SONIC_OUT;
    ULTRA_SONIC_SET;
    delay_ms(30);
    ULTRA_SONIC_RESET;
    ULTRA_SONIC_IN;
    while(ULTRA_SONIC_IS_LOW);
    fcap=TCNT1;
    while(ULTRA_SONIC_IS_HIGH);
    scap=TCNT1;
    return scap-fcap;
}

extern "C"{
 FILE * uart_str;
}

void init_run() {
	TCNT1=0;
	TCNT2=0;
	// set PB2,PB5 as output
	//DDRB |= (1<<PB2) | (1<<PB5);
	PORTD=0xFF;
	DDRB |= (1<<PB1) | (1<<PB2);// | (1<<PB3);
	//DDRB |= (1<<PB5); // wheel motor outs
	DDRB &= ~(1<<PB5);   // set PB3 aDDRD &= ~(1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD4) | (1<<PD5);s input sensing line
	//DDRB &= ~(1<<PB3);   // set PB5 as input sensing line	
	// init PWM
	pwm_init();
	//FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
	//lcd_write_string(PSTR(" Wheel controller "));
	//return lcd_stream;
	initIO();
}


#define LINE_LEFT	2500
#define LINE_CENTER	5000
#define LINE_RIGHT	7500

uint16_t read_line() {
	const uint16_t MAXVAL = 10;
	const uint16_t MULTIPLIER = 1000;
	uint16_t sensors[5];
	//uint16_t start=TCNT2,end;
	//end=start+MAXVAL;
	sensors[0]=MAXVAL*MULTIPLIER;
	sensors[1]=MAXVAL*MULTIPLIER;
	sensors[2]=MAXVAL*MULTIPLIER;
	sensors[3]=MAXVAL*MULTIPLIER;
	sensors[4]=MAXVAL*MULTIPLIER;
	// set d as output
	DDRD|= (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD4) | (1<<PD5);
	// pulse the lights
	PORTD=0x3E;
	delay_us(10);
	PORTD=0x00;
	// set d as input
	DDRD &= ~(1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD4) | (1<<PD5);
	uint16_t time=0;
	while (time<MAXVAL*MULTIPLIER) {
		delay_us(1);
		if (PIND&(1<<2)) {
			sensors[0]=time;
		}
		if (PIND&(1<<3)) {
			sensors[1]=time;
		}
		if (PIND&(1<<4)) {
			sensors[2]=time;
		}
		if (PIND&(1<<5)) {
			sensors[3]=time;
		}
		if (PIND&(1<<6)) {
			sensors[4]=time;
		}
		time+=MULTIPLIER;
	}
	return (sensors[0]+sensors[1]+sensors[2])/3 - (sensors[2]+sensors[3]+sensors[4])/3;
}

enum instruction_t {
	CONTINUE = 1,
	DRIVE_TO = 2,
	STOP_DRIVING = 3,
	DRIVE_FRONT_RIGHT = 4,
	DRIVE_FRONT_LEFT = 5,
	DRIVE_FORWARD = 6,
	DRIVE_BACKWARD = 7,
	DRIVE_BACK_RIGHT = 8,
	DRIVE_BACK_LEFT = 9,
	STOP_RUNNING = 10
};

typedef struct {
	instruction_t instruction;
	int16_t value;
} Command;

void getCommand(Command &cmd) {
	cmd.value=0;
	char tmp_c;
	int16_t tmp_i = 0;
	uint8_t result = getIO(&tmp_c,&tmp_i);//scanf_P("%c %d", &tmp_c, &tmp_i);
	if(result == 2) {
		cmd.value=tmp_i;
		if ('G'==tmp_c) { cmd.instruction=DRIVE_TO; } else
		if ('S'==tmp_c) { cmd.instruction=STOP_DRIVING; } else
		if ('E'==tmp_c) { cmd.instruction=DRIVE_FRONT_RIGHT; } else
		if ('Q'==tmp_c) { cmd.instruction=DRIVE_FRONT_LEFT; } else
		if ('F'==tmp_c) { cmd.instruction=DRIVE_FORWARD; } else
		if ('B'==tmp_c) { cmd.instruction=DRIVE_BACKWARD; } else
		if ('C'==tmp_c) { cmd.instruction=DRIVE_BACK_RIGHT; } else
		if ('Z'==tmp_c) { cmd.instruction=DRIVE_BACK_LEFT; } else
		if ('T'==tmp_c) { cmd.instruction=STOP_RUNNING; } else
		if ('L'==tmp_c) { cmd.instruction=CONTINUE; }
	}//*/
	//cmd.instruction=DRIVE_FRONT_LEFT;
}

void resetCommand(Command &cmd) {
	cmd.instruction=CONTINUE;
	cmd.value=0;
}
