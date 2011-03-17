CC=gcc
SRC=source
INC=include
CFLAGS=-I$(INC) -Wall

all: server.o static.o resolver.o cgi.o
	$(CC) server.o static.o resolver.o cgi.o -o server

server.o: $(SRC)/server.c
	$(CC) $(CFLAGS) -c $(SRC)/server.c

static.o: $(SRC)/static.c
	$(CC) $(CFLAGS) -c $(SRC)/static.c

resolver.o: $(SRC)/resolver.c
	$(CC) $(CFLAGS) -c $(SRC)/resolver.c

cgi.o: $(SRC)/cgi.c
	$(CC) $(CFLAGS) -c $(SRC)/cgi.c

clean:
	rm *.o
	rm server
