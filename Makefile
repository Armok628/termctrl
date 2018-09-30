CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99 -g
CC=clang

game:
	$(CC) $(CFLAGS) game.c src/*.c

world:
	$(CC) $(CFLAGS) worldgen.c src/*.c
