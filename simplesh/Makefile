CC = gcc
LIB = ../lib
NAME=simplesh
CFLAGS= -Werror -Wall -Wl,-rpath,$(LIB)

all: $(NAME)
	
simplesh: $(NAME).c
	$(MAKE) -C $(LIB)
	$(CC) $(NAME).c -o $(NAME) -L$(LIB) -I$(LIB) -lhelpers $(CFLAGS)


clean:
	rm -f $(NAME)
