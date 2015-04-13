CC = gcc
LIB = ../lib
NAME=buffilter
CFLAGS=-Wextra -Wall -Wl,-rpath,$(LIB)

all: $(NAME)
	
buffilter:	$(NAME).c
	$(MAKE) -C $(LIB)
	$(CC) $(NAME).c -o $(NAME) -L$(LIB) -I$(LIB) -lhelpers -lbufio $(CFLAGS)


clean:
	rm -f $(NAME)
