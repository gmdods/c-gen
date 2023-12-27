#ifndef GEN_TEST
#define GEN_TEST

#include <stdlib.h>

typedef unsigned int uint;

#include "../dynarray.h"
#include "../nodelist.h"

#define nodelist_type(F, type) _Generic(type, uint: F(uint))

#define dynarray_type(F, type) \
	_Generic(type, \
	    int: F(int), \
	    uint: F(uint), \
	    double: F(double), \
	    node_t(uint): F(node_t(uint)))

nodelist_declare(uint);

dynarray_declare(int);
dynarray_declare(uint);
dynarray_declare(double);

#endif // !GEN_TEST
