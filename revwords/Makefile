CC = gcc
LIB = ../lib
CFLAGS=-Wextra -Werror -Wall -Wl,-rpath,$(LIB)

all: revwords
	
revwords:
	$(MAKE) -C $(LIB)
	$(CC) revwords.c -o revwords -L$(LIB) -I$(LIB) -lhelpers $(CFLAGS)

clean:
	rm -f revwords

