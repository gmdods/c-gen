#ifndef GEN_TEST
#define GEN_TEST

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef _Bool boolean;

#include "../dynarray.h"
#include "../hashmap.h"
#include "../nodelist.h"

#define dynarray_type(X, type) \
	_Generic(type, int: X(int), size_t: X(size_t), double: X(double))
#define dynarray_types(X) X(int) X(double) static_assert(1, #X)

dynarray_types(dynarray_declare);
dynarray_types(dynarray_deduced);

#define nodelist_type(X, type) _Generic(type, uint: X(uint))
#define nodelist_types(X) X(uint) static_assert(1, #X)

nodelist_types(nodelist_declare);
nodelist_types(nodelist_deduced);

inline size_t uinthash(uint key) { return key * 7 + 3; }

#define hashmap_type(X, type) \
	_Generic(type, keyval_t(uint, boolean): X(keyval_t(uint, boolean)))
#define hashmap_types(X) X(uint, boolean) static_assert(1, #X)

hashmap_types(hashmap_declare);

#endif // !GEN_TEST
