CC = gcc
LIB = ../lib
NAME=filter
CFLAGS=-Wextra -Wall -Wl,-rpath,$(LIB)

all: filter
	
filter:
	$(MAKE) -C $(LIB)
	$(CC) $(NAME).c -o $(NAME) -L$(LIB) -I$(LIB) -lhelpers $(CFLAGS)


clean:
	rm -f $(NAME)
