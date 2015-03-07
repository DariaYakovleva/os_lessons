all:
	gcc -fPIC -c ../lib/helpers.c -o ../lib/helpers.o -Wextra -Werror -Wall
	gcc -shared -o ../lib/libhelpers.so ../lib/helpers.o
	gcc cat.c -o cat ../lib/libhelpers.so -Wextra -Werror -Wall
