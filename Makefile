CC=gcc
SRC=source
INC=include
CFLAGS=-I$(INC) -Wall

all: server.o static.o
	$(CC) server.o static.o -o server

server.o: $(SRC)/server.c
	$(CC) $(CFLAGS) -c $(SRC)/server.c

static.o: $(SRC)/static.c
	$(CC) $(CFLAGS) -c $(SRC)/static.c

clean:
	rm *.o
	rm server
