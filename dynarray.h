#ifndef GEN_DYNARRAY
#define GEN_DYNARRAY

#include "macros.h"
#include <stdlib.h>

#define dynarray_t(type_t) struct CAT(dynarray_, type_t)

#define dynarray_init(type_t) CAT(dynarray_init_, type_t)
#define dynarray_deinit(type_t) CAT(dynarray_deinit_, type_t)
#define dynarray_reserve(type_t) CAT(dynarray_reserve_, type_t)

#define dynarray_declare(type_t) \
	dynarray_t(type_t) { \
		type_t * ptr; \
		size_t size; \
		size_t capacity; \
	}; \
	DECLARE(dynarray_t(type_t), dynarray_init(type_t), size_t) \
	DECLARE(void, dynarray_deinit(type_t), dynarray_t(type_t) *) \
	DECLARE(void, dynarray_reserve(type_t), dynarray_t(type_t) *, size_t)

#define dynarray_define(type_t) \
	dynarray_t(type_t) dynarray_init(type_t)(size_t size) { \
		return (dynarray_t(type_t)){ \
		    .ptr = calloc(size, sizeof(type_t)), \
		    .size = size, \
		    .capacity = size, \
		}; \
	} \
	void dynarray_deinit(type_t)(dynarray_t(type_t) * array) { \
		if (!array) return; \
		if (array->ptr) free(array->ptr); \
		*array = (dynarray_t(type_t)){0}; \
	} \
	void dynarray_reserve(type_t)(dynarray_t(type_t) * array, \
				      size_t capacity) { \
		if (capacity <= array->capacity) return; \
		*array = (dynarray_t(type_t)){ \
		    .ptr = realloc(array->ptr, capacity * sizeof(type_t)), \
		    .size = array->size, \
		    .capacity = capacity, \
		}; \
	}

#endif // !GEN_DYNARRAY
