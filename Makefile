#Author:   Colby Ackerman
#Class:    Operating Systems - CS4760
#Assign:   #6
#Date:     11/30/19
#=======================================================

CC = gcc
CFLAGS = -I . -g
OBJECTS = shared.o processManage.o interrupts.o clock.o message.o

.SUFFIXES: .c .o

all: oss usr

oss: oss.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ oss.o $(OBJECTS) -lpthread

usr: usr.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ usr.o $(OBJECTS) -lpthread

test: test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ test.o $(OBJECTS) -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -f *.o oss usr test *.txt