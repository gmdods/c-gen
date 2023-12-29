#ifndef GEN_TEST
#define GEN_TEST

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef struct {
	uint key;
	bool val;
} keyval_t;

#include "../dynarray.h"
#include "../nodelist.h"

#define dynarray_type(X, type) \
	_Generic(type, int: X(int), size_t: X(size_t), double: X(double))
#define dynarray_types(X) X(int) X(size_t) X(double) static_assert(1, #X)

dynarray_types(dynarray_declare);
dynarray_types(dynarray_deduced);

#define nodelist_type(X, type) \
	_Generic(type, uint: X(uint), keyval_t: X(keyval_t))
#define nodelist_types(X) X(uint) X(keyval_t) static_assert(1, #X)

nodelist_types(nodelist_declare);
nodelist_types(nodelist_deduced);

inline size_t uinthash(uint key) { return key * 7 + 3; }

#endif // !GEN_TEST
