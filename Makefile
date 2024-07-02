MODE = debug

include config.mk
export CFLAGS LDFLAGS

.PHONY: all
all: test

test: test.o arena.o
	gcc $(LDFLAGS) -o $@ $^

arena.o: arena.c arena.h
	gcc $(CFLAGS) -c $<

test.o: test.c arena.h common.h
	gcc $(CFLAGS) -c $<

.PHONY: json
json: clean
	bear -- make test

.PHONY: clean
clean:
	-rm -f *.o && cd tests && make clean
