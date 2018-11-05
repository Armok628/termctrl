CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99 -g
CC=clang

world:
	$(CC) $(CFLAGS) maptest.c src/*.c

zone:
	$(CC) $(CFLAGS) main.c src/*.c

size:
	$(CC) $(CFLAGS) termsize.c src/terminal.c
