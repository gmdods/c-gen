#ifndef GEN_DYNARRAY_TEST
#define GEN_DYNARRAY_TEST

#define GEN_TYPE(F, type) _Generic(type, int: F(int), double: F(double))

#include "../dynarray.h"

dynarray_declare(int)
dynarray_declare(double)

#endif // !GEN_DYNARRAY_TEST
