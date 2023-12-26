#ifndef GEN_DYNARRAY
#define GEN_DYNARRAY

#include "macros.h"
#include <assert.h>
#include <stdlib.h>

#define dynarray_t(type_t) struct CONCAT(dynarray_, type_t)

#define dynarray_init_fn(type_t) CONCAT(dynarray_init_, type_t)
#define dynarray_deinit_fn(type_t) CONCAT(dynarray_deinit_, type_t)
#define dynarray_reserve_fn(type_t) CONCAT(dynarray_reserve_, type_t)

#define dynarray_init(type, sz) dynarray_init_fn(type)(sz)
#define dynarray_deinit(array_ref) \
	GEN_TYPE(dynarray_deinit_fn, *(array_ref)->ptr)(array_ref)
#define dynarray_reserve(array_ref, sz) \
	GEN_TYPE(dynarray_reserve_fn, *(array_ref)->ptr)(array_ref, sz)

#define dynarray_declare(type_t) \
	dynarray_t(type_t) { \
		type_t * ptr; \
		size_t size; \
		size_t capacity; \
	}; \
	static_assert(GEN_TYPE(sizeof, (type_t){0}) == sizeof(type_t), \
		      STRING(type_t) " dynarray"); \
	DECLARE(dynarray_t(type_t), dynarray_init_fn(type_t), size_t) \
	DECLARE(void, dynarray_deinit_fn(type_t), dynarray_t(type_t) *) \
	DECLARE(void, dynarray_reserve_fn(type_t), dynarray_t(type_t) *, size_t)

#define dynarray_define(type_t) \
	dynarray_t(type_t) dynarray_init_fn(type_t)(size_t size) { \
		return (dynarray_t(type_t)){ \
		    .ptr = calloc(size, sizeof(type_t)), \
		    .size = size, \
		    .capacity = size, \
		}; \
	} \
	void dynarray_deinit_fn(type_t)(dynarray_t(type_t) * array) { \
		if (!array) return; \
		if (array->ptr) free(array->ptr); \
		*array = (dynarray_t(type_t)){0}; \
	} \
	void dynarray_reserve_fn(type_t)(dynarray_t(type_t) * array, \
					size_t capacity) { \
		if (capacity <= array->capacity) return; \
		*array = (dynarray_t(type_t)){ \
		    .ptr = realloc(array->ptr, capacity * sizeof(type_t)), \
		    .size = array->size, \
		    .capacity = capacity, \
		}; \
	}

#endif // !GEN_DYNARRAY
