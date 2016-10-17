#define F_CPU 14745600
#include <pololu/3pi.h>
#include <stdio.h>

#define F_CPU 14745600

#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

//#include "../libnerdkits/delay.h"
//#include "../libnerdkits/lcd.h"
#include "../libnerdkits/uart.h"
#include "servo.h"

#define ULTRA_SONIC_MASK (1<<4)

#define ULTRA_SONIC_OUT (DDRB|=ULTRA_SONIC_MASK)
#define ULTRA_SONIC_IN (DDRB&=~ULTRA_SONIC_MASK)
#define ULTRA_SONIC_SET (PORTB|=ULTRA_SONIC_MASK)
#define ULTRA_SONIC_RESET (PORTB&=~ULTRA_SONIC_MASK)
#define ULTRA_SONIC_IS_LOW (!(PINB&ULTRA_SONIC_MASK))
#define ULTRA_SONIC_IS_HIGH ((PINB&ULTRA_SONIC_MASK)==ULTRA_SONIC_MASK)


#define ULTRA_SONIC_MASK_RIGHT (1<<1)

//#define ULTRA_SONIC_OUT (DDRB|=ULTRA_SONIC_MASK)
#define ULTRA_SONIC_IN_RIGHT (DDRB&=~ULTRA_SONIC_MASK_RIGHT)
#define ULTRA_SONIC_SET_RIGHT (PORTB|=ULTRA_SONIC_MASK_RIGHT)
#define ULTRA_SONIC_RESET_RIGHT (PORTB&=~ULTRA_SONIC_MASK_RIGHT)
#define ULTRA_SONIC_IS_LOW_RIGHT (!(PINB&ULTRA_SONIC_MASK_RIGHT))
#define ULTRA_SONIC_IS_HIGH_RIGHT ((PINB&ULTRA_SONIC_MASK_RIGHT)==ULTRA_SONIC_MASK_RIGHT)


#define ULTRA_SONIC_MASK_LEFT (1<<3)

//#define ULTRA_SONIC_OUT (DDRB|=ULTRA_SONIC_MASK)
#define ULTRA_SONIC_IN_LEFT (DDRB&=~ULTRA_SONIC_MASK_LEFT)
#define ULTRA_SONIC_SET_LEFT (PORTB|=ULTRA_SONIC_MASK_LEFT)
#define ULTRA_SONIC_RESET_LEFT (PORTB&=~ULTRA_SONIC_MASK_LEFT)
#define ULTRA_SONIC_IS_LOW_LEFT (!(PINB&ULTRA_SONIC_MASK_LEFT))
#define ULTRA_SONIC_IS_HIGH_LEFT ((PINB&ULTRA_SONIC_MASK_LEFT)==ULTRA_SONIC_MASK_LEFT)



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

//FILE uart_stream;

#ifdef __cplusplus
extern "C"{
 FILE * uart_str;
}
#endif 

void init_run() {
	TCNT1=0;
	// set PB2,PB5 as output
	//DDRB |= (1<<PB2) | (1<<PB5);
	DDRB |= (1<<PB1) | (1<<PB2);// | (1<<PB3);
	//DDRB |= (1<<PB5); // wheel motor outs
	DDRB &= ~(1<<PB5);   // set PB3 as input sensing line
	//DDRB &= ~(1<<PB3);   // set PB5 as input sensing line
	// init PWM
	pwm_init();
	//FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
	//lcd_write_string(PSTR(" Wheel controller "));
	//return lcd_stream;
	uart_init();
	uart_str = fdevopen(uart_putchar, uart_getchar);
	stdout = stdin = uart_str;
	pololu_3pi_init(2000);
}

enum instruction_t {
	CONTINUE,
	GO_TO,
	STOP_DRIVING,
	DRIVE_FRONT_RIGHT,
	DRIVE_FRONT_LEFT,
	DRIVE_FORWARD,
	DRIVE_BACKWARD,
	DRIVE_BACK_RIGHT,
	DRIVE_BACK_LEFT,
	TERMINATE
};

typedef struct {
	instruction_t instruction;
	int16_t value;
} Command;

void getCommand(Command &cmd) {
	cmd.value=0;
	unsigned char tmp_c=0;
	int16_t tmp_i = 0;

	// get char
	int i=fgetc(uart_str);
	if (i==EOF) { return; }
	tmp_c=(char)i;
	// get space
	i=fgetc(uart_str);
	if (i==EOF) { return; }
	// get number
	i=fgetc(uart_str);
	if (i==EOF) { return; }
	char buff[2]={0,0};
	buff[0]=(char)i;
	tmp_i=atoi(buff);
	
	cmd.value=tmp_i;
	switch (tmp_c) {
		case 'G': cmd.instruction=GO_TO;break;
		case 'S': cmd.instruction=STOP_DRIVING;break;
		case 'E': cmd.instruction=DRIVE_FRONT_RIGHT;break;
		case 'Q': cmd.instruction=DRIVE_FRONT_LEFT;break;
		case 'F': cmd.instruction=DRIVE_FORWARD;break;
		case 'B': cmd.instruction=DRIVE_BACKWARD;break;
		case 'C': cmd.instruction=DRIVE_BACK_RIGHT;break;
		case 'Z': cmd.instruction=DRIVE_BACK_LEFT;break;
		case 'T': cmd.instruction=TERMINATE;break;
		default:break;
	}
}

void resetCommand(Command &cmd) {
	cmd.instruction=CONTINUE;
	cmd.value=0;
}
