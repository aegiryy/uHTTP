CC=gcc
SRC=source
INC=include
CFLAGS=-I$(INC) -Wall

all: gate.o control.o static.o
	$(CC) gate.o control.o static.o -o server

gate.o: $(SRC)/gate.c
	$(CC) $(CFLAGS) -c $(SRC)/gate.c 

control.o: $(SRC)/control.c
	$(CC) $(CFLAGS) -c $(SRC)/control.c

static.o: $(SRC)/static.c
	$(CC) $(CFLAGS) -c $(SRC)/static.c

clean:
	rm *.o
	rm server
