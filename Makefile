CFLAGS=-g -Wall
CC=gcc $(LDFLAGS) $(CFLAGS)

.phony: all clean

all: bin bin/indexer bin/sort

bin/indexer: indexer.c lib/*c
	$(CC) -o $@  $^

bin/sort: sort.c lib/error.c lib/index_data.c
	$(CC) -o $@  $^

clean: bin/indexer
	rm -fr bin

bin:
	mkdir -p bin

