CC = gcc
LIB = ../lib
NAME=forking
NAME2=polling
CFLAGS=-Wextra -Wall -Wl,-rpath,$(LIB)

all: $(NAME) $(NAME2)
	
forking:	$(NAME).c
	make -C $(LIB)
	$(CC) $(NAME).c -o $(NAME) -L$(LIB) -I$(LIB) -lbufio $(CFLAGS)

polling:	$(NAME2).c
	make -C $(LIB)
	$(CC) $(NAME2).c -o $(NAME2) -L$(LIB) -I$(LIB) -lbufio $(CFLAGS)


clean:
	rm -f $(NAME)
	rm -f $(NAME2)
