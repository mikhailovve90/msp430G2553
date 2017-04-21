PROJ=ekran

CPU=msp430g2553

CC=msp430-gcc
SZ=msp430-size
OD=msp430-objdump
DBG=mspdebug

CFLAGS =-Os -Wall -c -mmcu=$(CPU)
CFLAGS += -g -std=c99

LFLAGS=-mmcu=$(CPU)

#DBGFLAG="UIF -d /dev/ttyUSB0"
DBGFLAG=RF2500

$(PROJ).elf : ekran.o
	$(CC) $(LFLAGS) -o $(PROJ).elf ekran.o
	$(SZ) $(PROJ).elf
	$(OD) -dSt $(PROJ).elf > $(PROJ).lst
	# $(EDIT) $(PROJ).lst

izmer : ekran.c
	$(CC) $(CFLAGS) -o ekran.o ekran.c



.PHONY: load, debug, clean

load:  $(PROJ).elf
	$(DBG) $(DBGFLAG) "prog $(PROJ).elf"

debug: $(PROJ).elf
	$(DBG) $(DBGFLAG)

clean:
	-rm -rf *.o $(PROJ).elf $(PROJ).lst
