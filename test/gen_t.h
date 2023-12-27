#ifndef GEN_TEST
#define GEN_TEST

typedef unsigned int uint;

#define GEN_TYPE(F, type) \
	_Generic(type, int: F(int), uint: F(uint), double: F(double))
#include "../dynarray.h"

dynarray_declare(int);
dynarray_declare(uint);
dynarray_declare(double);

#endif // !GEN_TEST
