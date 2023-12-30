#ifndef GEN_TEST
#define GEN_TEST

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef unsigned int uint;
typedef _Bool boolean;
typedef const char * string;

#include "../arena.h"
#include "../dynarray.h"
#include "../hashmap.h"
#include "../nodelist.h"

#define arena_type(X, type) \
	_Generic(type, int: X(int), size_t: X(size_t), double: X(double))
#define arena_types(X) X(int) X(size_t) X(double) static_assert(1, #X)

arena_types(dynarray_declare);
arena_types(dynarray_deduced);

#define nodelist_type(X, type) _Generic(type, uint: X(uint))
#define nodelist_types(X) X(uint) static_assert(1, #X)

nodelist_types(nodelist_declare);
nodelist_types(nodelist_deduced);

inline size_t uinthash(uint key) { return key * 7 + 3; }
inline size_t strhash(string key) {
	size_t hash = 0xABCDEF;
	for (; *key; ++key) {
		hash = (hash << 1) | (hash >> ((sizeof(size_t) * 8) - 1));
		hash ^= *key;
	}
	return hash;
}

#define hashmap_hash(type) _Generic(type, uint: uinthash, string: strhash)(type)
#define hashmap_type(X, type) \
	_Generic(type, \
	    keyval_t(uint, boolean): X(keyval_t(uint, boolean)), \
	    keyval_t(string, uint): X(keyval_t(string, uint)))
#define hashmap_types(X) X(uint, boolean) X(string, uint) static_assert(1, #X)

hashmap_types(hashmap_declare);

#endif // !GEN_TEST
