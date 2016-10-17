#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <inttypes.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/uart.h"
#include "getIO.h"

FILE uart_stream;

int getIO(char *c,int *i) {
	return scanf_P(PSTR("%c %d"), c,i);
}
void initIO() {
	uart_init();
	fdev_setup_stream(&uart_stream,uart_putchar, uart_getchar, _FDEV_SETUP_RW);
	stdin = stdout = &uart_stream;
}

