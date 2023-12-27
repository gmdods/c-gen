CC = gcc
CFLAGS = -Wall -Wpedantic -Wextra \
	 -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function \
	 -march=native -std=gnu11
TEST = -Og -g
PROD = -O3

DEPS = gen_t.o
TESTS = unittest.c test.c

test: test.out
.PHONY: test

gen_t.o: gen_t.c gen_t.h ../*.h
	$(CC) $(CFLAGS) $(TEST) -c $< -o $@

test.o: test.c $(TESTS)
	$(CC) $(CFLAGS) $(TEST) -c $< -o $@

test.out: test.o $(DEPS)
	$(CC) $(CFLAGS) $(TEST) $^ -o $@ && ./$@

clean:
	rm *.out *.o
