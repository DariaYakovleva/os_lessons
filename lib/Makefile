CFLAGS=-Wextra -Wall
CC=gcc

all:	helpers bufio

bufio:
	$(CC) -fPIC -c bufio.c -o bufio.o $(CFLAGS)
	$(CC) -shared -o libbufio.so bufio.o 

helpers:
	$(CC) -fPIC -c helpers.c -o helpers.o $(CFLAGS)
	$(CC) -shared -o libhelpers.so helpers.o 


clean:
	rm -f helpers.o
	rm -f libhelpers.so
	rm -f bufio.o
	rm -f libbufio.so
