CFLAGS=-Wextra -Werror -Wall
CC = gcc
LIB = ../lib
NAME=filter

all: filter
	
filter:
	$(MAKE) -C $(LIB)
	$(CC) $(NAME).c -o $(NAME) -L$(LIB) -I$(LIB) -lhelpers $(CFLAGS)

clean:
	rm -f $(NAME)
