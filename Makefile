CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99 -g
CC=clang

test:
	$(CC) $(CFLAGS) *.c src/*.c
