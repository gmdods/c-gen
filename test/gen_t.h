#ifndef GEN_TEST
#define GEN_TEST

#include <stdlib.h>

typedef unsigned int uint;

typedef struct {
	size_t index;
	uint elt;
} node_t;

#define dynarray_type(F, type) \
	_Generic(type, \
	    int: F(int), \
	    uint: F(uint), \
	    double: F(double), \
	    node_t: F(node_t))

#include "../dynarray.h"

dynarray_declare(int);
dynarray_declare(uint);
dynarray_declare(double);
dynarray_declare(node_t);

#endif // !GEN_TEST
