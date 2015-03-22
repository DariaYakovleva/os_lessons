CFLAGS=-Wextra -Werror -Wall
CC = gcc
LIB = ../lib

all: revwords
	
revwords:
	$(MAKE) -C $(LIB)
	$(CC) revwords.c -o revwords -L$(LIB) -I$(LIB) -lhelpers $(CFLAGS)

clean:
	rm -f revwords

