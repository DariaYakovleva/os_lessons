all:
	gcc -fPIC -c ../lib/helpers.c -o ../lib/helpers.o -Wextra -Werror -Wall
	gcc -shared -o ../lib/libhelpers.so ../lib/helpers.o
	gcc revwords.c -o revwords ../lib/libhelpers.so -Wextra -Werror -Wall
