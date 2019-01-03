CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99 -g -O3
CC=clang

zone:
	$(CC) $(CFLAGS) zonetest.c src/*.c

path:
	$(CC) $(CFLAGS) pathtest.c src/*.c

world:
	$(CC) $(CFLAGS) maptest.c src/*.c

size:
	$(CC) $(CFLAGS) termsize.c src/terminal.c
