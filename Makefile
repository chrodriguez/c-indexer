CFLAGS=-g -Wall
CC=gcc $(LDFLAGS) $(CFLAGS)

.PHONY: all clean test test-indexes

all: bin bin/indexer bin/sort

bin/indexer: indexer.c lib/*c
	$(CC) -o $@  $^

bin/sort: sort.c lib/error.c lib/index_data.c
	$(CC) -o $@  $^

clean: bin/indexer test-clean
	rm -fr bin

bin:
	mkdir -p bin

test: bin bin/indexer
	make -C test/generator

test-indexes:
	./test/index-all.sh

test-clean: 
	make -C test/generator clean
