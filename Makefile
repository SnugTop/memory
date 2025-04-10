CC = gcc
CFLAGS = -Wall -std=c99 -Isrc
SRC = src/main.c src/memory.c src/backing_store.c src/utils.c
OUT = bin/a.out

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
