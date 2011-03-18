CC=gcc
SRC=source
INC=include
CFLAGS=-I$(INC) -Wall -c
OBJS=server.o controller.o resolver.o cgi.o
TARGET=server

all: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

xinetd: $(SRC)/controller.c resolver.o cgi.o 
	$(CC) $(CFLAGS) -DXINETD $<
	$(CC) controller.o resolver.o cgi.o -o $(TARGET)

server.o: $(SRC)/server.c
	$(CC) $(CFLAGS) $<

controller.o: $(SRC)/controller.c
	$(CC) $(CFLAGS) $<

resolver.o: $(SRC)/resolver.c
	$(CC) $(CFLAGS) $<

cgi.o: $(SRC)/cgi.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o
	rm -f $(TARGET)
