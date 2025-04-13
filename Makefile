CC = gcc
CFLAGS = -Wall -std=c99 -Isrc

MAIN_SRC = src/main.c src/memory.c src/backing_store.c src/utils.c
MAIN_OUT = bin/a.out

TEST_SRC = test/check_bin.c
TEST_OUT = test/check_bin

.PHONY: all clean main test

all: main test

main: $(MAIN_SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(MAIN_SRC) -o $(MAIN_OUT)

test: $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_OUT)

clean:
	rm -f $(MAIN_OUT) $(TEST_OUT)