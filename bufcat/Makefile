CC = gcc
LIB = ../lib
NAME=bufcat
CFLAGS=-Wextra -Wall -Wl,-rpath,$(LIB)

all: $(NAME)
	
bufcat:	$(NAME).c
	make -C $(LIB)
	$(CC) $(NAME).c -o $(NAME) -L$(LIB) -I$(LIB) -lbufio $(CFLAGS)


clean:
	rm -f $(NAME)
