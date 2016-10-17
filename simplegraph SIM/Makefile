GCCFLAGS=-g -Os -Wall -mmcu=atmega168 -I/usr/avr/include/
LINKFLAGS=-Wl,-u,vfprintf -lprintf_flt -lm 
AVRDUDEFLAGS=-c avr109 -p m168 -b 115200 -P /dev/ttyUSB0
LINKOBJECTS=../libnerdkits/delay.o ../libnerdkits/uart.o

all:	main-upload

main.hex:	main.cpp
	make -C ../libnerdkits
	avr-gcc ${GCCFLAGS} ${LINKFLAGS} -o main.o main.cpp graphNode.cpp calcSteerDirection.cpp depthFirstSearch.cpp getIO.c ${LINKOBJECTS}
	avr-objcopy -j .text -O ihex main.o main.hex
	
main.ass:	main.hex
	avr-objdump -S -d main.o > main.ass
	
main-upload:	main.hex
	avrdude ${AVRDUDEFLAGS} -U flash:w:main.hex:a
