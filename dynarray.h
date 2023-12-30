#ifndef GEN_DYNARRAY
#define GEN_DYNARRAY

#include "macros.h"
#include <assert.h>
#include <stdlib.h>

#define dynarray_t(type_t) struct CONCAT(dynarray_, type_t)

#define dynarray_deinit_fn(type_t) CONCAT(dynarray_deinit_, type_t)
#define dynarray_add_fn(type_t) CONCAT(dynarray_add_, type_t)

#define dynarray_at(array, pos) arena_at((array).arena, pos)
#define dynarray_init(type, sz) \
	(dynarray_t(type)) { .arena = arena_init_fn(type)(sz) }
#define dynarray_reserve(array_ref, sz) \
	arena_type(arena_reserve_fn, \
		   *(array_ref)->arena.ptr)(&(array_ref)->arena, sz)

#define dynarray_associated(dynarray_fn, array_ref, ...) \
	arena_type(dynarray_fn, *(array_ref)->arena.ptr)( \
	    array_ref __VA_OPT__(, ) __VA_ARGS__)
#define dynarray_deinit(array_ref) \
	dynarray_associated(dynarray_deinit_fn, array_ref)
#define dynarray_add(array_ref, elt) \
	dynarray_associated(dynarray_add_fn, array_ref, elt)

#define dynarray_declare(type_t) \
	arena_declare(type_t) dynarray_t(type_t) { \
		arena_t(type_t) arena; \
		size_t size; \
	}; \
	void dynarray_deinit_fn(type_t)(dynarray_t(type_t) *); \
	void dynarray_add_fn(type_t)(dynarray_t(type_t) *, type_t);

#define dynarray_define(type_t) \
	void dynarray_deinit_fn(type_t)(dynarray_t(type_t) * array) { \
		if (!array) return; \
		arena_deinit_fn(type_t)(&array->arena); \
		*array = (dynarray_t(type_t)){0}; \
	} \
	void dynarray_add_fn(type_t)(dynarray_t(type_t) * array, type_t elt) { \
		if (array->arena.storage == 0) \
			*array = dynarray_init(type_t, 1); \
		else if (array->arena.storage <= array->size) \
			arena_reserve_fn(type_t)(&array->arena, \
						 2 * array->arena.storage); \
		array->arena.ptr[array->size++] = elt; \
	} \
	arena_define(type_t)

#define dynarray_deduced(type_t) \
	static_assert(arena_type(sizeof, (type_t){0}) == sizeof(type_t), \
		      STRING(type_t) " dynarray");

#endif // !GEN_DYNARRAY
