#ifndef GEN_TEST
#define GEN_TEST

#include <stdlib.h>

typedef unsigned int uint;

#include "../dynarray.h"
#include "../nodelist.h"

#define dynarray_type(F, type) \
	_Generic(type, int: F(int), uint: F(uint), double: F(double))

dynarray_ensure(dynarray_declare, int);
dynarray_ensure(dynarray_declare, uint);
dynarray_ensure(dynarray_declare, double);

#define nodelist_type(F, type) _Generic(type, uint: F(uint))

nodelist_ensure(nodelist_declare, uint);

#endif // !GEN_TEST
