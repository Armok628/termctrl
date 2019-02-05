CFLAGS=-Wall -Wextra -Wpedantic -lncurses -std=gnu99 -g -O3
CC=clang

world:
	$(CC) $(CFLAGS) maptest.c src/util/*.c src/world/*.c

zone:
	$(CC) $(CFLAGS) zonetest.c src/util/*.c src/zone/*.c
