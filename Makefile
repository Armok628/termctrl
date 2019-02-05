CFLAGS=-Wall -Wextra -Wpedantic -lncurses -std=gnu99 -g -O3
CC=clang

world:
	$(CC) maptest.c src/util/*.c src/world/*.c $(CFLAGS)

zone:
	$(CC) zonetest.c src/util/*.c src/zone/*.c $(CFLAGS)
