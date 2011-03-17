CC=gcc
SRC=source
INC=include
CFLAGS=-I$(INC) -Wall -c
OBJS=server.o static.o resolver.o cgi.o
TARGET=server

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

server.o: $(SRC)/server.c
	$(CC) $(CFLAGS) $<

static.o: $(SRC)/static.c
	$(CC) $(CFLAGS) $<

resolver.o: $(SRC)/resolver.c
	$(CC) $(CFLAGS) $<

cgi.o: $(SRC)/cgi.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o
	rm -f $(TARGET)
