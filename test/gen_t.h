#ifndef GEN_TEST
#define GEN_TEST

#include <assert.h>
#include <stdlib.h>

typedef unsigned int uint;

#include "../dynarray.h"
#include "../nodelist.h"

#define dynarray_type(X, type) \
	_Generic(type, int: X(int), uint: X(uint), double: X(double))
#define dynarray_types(X) X(int) X(uint) X(double) static_assert(1, #X)

dynarray_types(dynarray_declare);
dynarray_types(dynarray_deduced);

#define nodelist_type(X, type) _Generic(type, uint: X(uint))
#define nodelist_types(X) X(uint) static_assert(1, #X)

nodelist_types(nodelist_declare);
nodelist_types(nodelist_deduced);

#endif // !GEN_TEST
