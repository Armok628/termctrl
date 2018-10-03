CFLAGS=-Wall -Wextra -Wpedantic -std=gnu99 -g
CC=clang

game:
	$(CC) $(CFLAGS) main.c src/*.c

world:
	$(CC) $(CFLAGS) worldgen.c src/*.c
