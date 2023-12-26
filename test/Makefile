CC = gcc
CFLAGS = -Wall -Wpedantic -Wextra \
	 -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function \
	 -march=native -std=gnu11
TEST = -Og -g
PROD = -O3

DEPS = dynarray_t.o
TESTS = unittest.c test.c

test: test.out
.PHONY: test

dynarray_t.o: dynarray_t.c dynarray_t.h ../dynarray.h
	$(CC) $(CFLAGS) $(TEST) -c $< -o $@

test.o: test.c $(TESTS)
	$(CC) $(CFLAGS) $(TEST) -c $< -o $@

test.out: test.o $(DEPS)
	$(CC) $(CFLAGS) $(TEST) $^ -o $@ && ./$@

clean:
	rm *.out *.o
