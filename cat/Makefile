CC = gcc
LIB = ../lib
CFLAGS=-Wextra -Werror -Wall -Wl,-rpath,$(LIB)

all: cat
	
cat:
	make -C $(LIB)
	$(CC) cat.c -o cat -L$(LIB) -I$(LIB) -lhelpers $(CFLAGS)

clean:
	rm -f cat
