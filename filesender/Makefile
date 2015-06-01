CC = gcc
LIB = ../lib
NAME=filesender
CFLAGS=-Wextra -Wall -Wl,-rpath,$(LIB)

all: $(NAME)
	
filesender:	$(NAME).c
	make -C $(LIB)
	$(CC) $(NAME).c -o $(NAME) -L$(LIB) -I$(LIB) -lbufio $(CFLAGS)


clean:
	rm -f $(NAME)
