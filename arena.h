#ifndef GEN_ARENA
#define GEN_ARENA

#include "macros.h"
#include <assert.h>
#include <stdlib.h>

#define arena_t(type_t) struct CONCAT(arena_, type_t)

#define arena_init_fn(type_t) CONCAT(arena_init_, type_t)
#define arena_deinit_fn(type_t) CONCAT(arena_deinit_, type_t)
#define arena_reserve_fn(type_t) CONCAT(arena_reserve_, type_t)

#define arena_at(arena, pos) (arena).ptr[pos]
#define arena_init(type, sz) arena_init_fn(type)(sz)

#define arena_associated(arena_fn, arena_ref, ...) \
	arena_type(arena_fn, \
		   *(arena_ref)->ptr)(arena_ref __VA_OPT__(, ) __VA_ARGS__)
#define arena_deinit(arena_ref) arena_associated(arena_deinit_fn, arena_ref)
#define arena_reserve(arena_ref, sz) \
	arena_associated(arena_reserve_fn, arena_ref, sz)

#define arena_declare(type_t) \
	arena_t(type_t) { \
		type_t * ptr; \
		size_t storage; \
	}; \
	arena_t(type_t) arena_init_fn(type_t)(size_t); \
	void arena_deinit_fn(type_t)(arena_t(type_t) *); \
	void arena_reserve_fn(type_t)(arena_t(type_t) *, size_t); \

#define arena_define(type_t) \
	arena_t(type_t) arena_init_fn(type_t)(size_t size) { \
		return (arena_t(type_t)){ \
		    .ptr = calloc(size, sizeof(type_t)), \
		    .storage = size, \
		}; \
	} \
	void arena_deinit_fn(type_t)(arena_t(type_t) * arena) { \
		if (!arena) return; \
		if (arena->ptr) free(arena->ptr); \
		*arena = (arena_t(type_t)){0}; \
	} \
	void arena_reserve_fn(type_t)(arena_t(type_t) * arena, \
				      size_t storage) { \
		if (storage <= arena->storage) return; \
		*arena = (arena_t(type_t)){ \
		    .ptr = realloc(arena->ptr, storage * sizeof(type_t)), \
		    .storage = storage, \
		}; \
	}

#define arena_deduced(type_t) \
	static_assert(arena_type(sizeof, (type_t){0}) == sizeof(type_t), \
		      STRING(type_t) " arena");

#endif // !GEN_ARENA
