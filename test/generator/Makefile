CFLAGS=-g -Wall 

.PHONY: all clean generate-output

all: bin bin/generate generate-output

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

bin/generate: generate-data.c lib/person.c lib/log.c lib/error.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

bin:
	mkdir -p bin

clean:
	rm -f *.o lib/*.o bin/generate 

generate-output: 
	./random-ouput
