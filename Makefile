CC=gcc
SRC=source
INC=include
CFLAGS=-I$(INC) -Wall

all: gate.o control.o
	$(CC) gate.o control.o -o server

gate.o: $(SRC)/gate.c
	$(CC) $(CFLAGS) -c $(SRC)/gate.c 

control.o: $(SRC)/control.c
	$(CC) $(CFLAGS) -c $(SRC)/control.c

clean:
	rm *.o
	rm server
