CFLAGS=-Wextra -Werror -Wall
CC=gcc

all:
	$(CC) -fPIC -c helpers.c -o helpers.o $(CFLAGS)
	$(CC) -shared -o libhelpers.so helpers.o 

clean:
	rm -f helpers.o
	rm -f libhelpers.so
