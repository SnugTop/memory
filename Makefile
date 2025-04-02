CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/memory.c src/backing_store.c src/utils.c
OUT = bin/a.out

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f bin/a.out