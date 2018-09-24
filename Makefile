CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99
CC=clang

test:
	$(CC) $(CFLAGS) *.c src/*.c
