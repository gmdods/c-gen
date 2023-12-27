#ifndef GEN_NODELIST
#define GEN_NODELIST

#include "macros.h"
#include <assert.h>
#include <stdlib.h>

#define node_t(type_t) CONCAT(node_, type_t)
#define nodelist_t(type_t) struct CONCAT(nodelist_, type_t)

#define nodelist_init_fn(type_t) CONCAT(nodelist_init_, type_t)
#define nodelist_deinit_fn(type_t) CONCAT(nodelist_deinit_, type_t)
#define nodelist_reserve_fn(type_t) dynarray_reserve_fn(node_t(type_t))
#define nodelist_add_fn(type_t) CONCAT(nodelist_add_, type_t)

#define nodelist_init(type, sz) nodelist_init_fn(type)(sz)
#define nodelist_associated(nodelist_fn, list_ref, ...) \
	nodelist_type(nodelist_fn, (list_ref)->array.ptr->elt)( \
	    list_ref __VA_OPT__(, ) __VA_ARGS__)
#define nodelist_deinit(list_ref) \
	nodelist_associated(nodelist_deinit_fn, list_ref)
#define nodelist_reserve(list_ref, sz) \
	nodelist_type(nodelist_reserve_fn, \
		      (list_ref)->array.ptr->elt)(&(list_ref)->array, sz)
#define nodelist_add(list_ref, elt) \
	nodelist_associated(nodelist_add_fn, list_ref, elt)

#define nodelist_declare(type_t) \
	typedef struct { \
		size_t index; \
		type_t elt; \
	} node_t(type_t); \
	dynarray_declare(node_t(type_t)) nodelist_t(type_t) { \
		dynarray_t(node_t(type_t)) array; \
		size_t tail; \
	}; \
	nodelist_t(type_t) nodelist_init_fn(type_t)(size_t); \
	void nodelist_deinit_fn(type_t)(nodelist_t(type_t) *); \
	void nodelist_add_fn(type_t)(nodelist_t(type_t) *, type_t);

#define nodelist_define(type_t) \
	nodelist_t(type_t) nodelist_init_fn(type_t)(size_t size) { \
		nodelist_t(type_t) \
		    list = {.array = dynarray_init(node_t(type_t), size)}; \
		dynarray_add_fn(node_t(type_t))(&list.array, \
						(node_t(type_t)){0}); \
		return list; \
	} \
	void nodelist_deinit_fn(type_t)(nodelist_t(type_t) * list) { \
		if (!list) return; \
		dynarray_deinit_fn(node_t(type_t))(&list->array); \
		list->tail = 0; \
	} \
	void nodelist_add_fn(type_t)(nodelist_t(type_t) * list, type_t elt) { \
		size_t size = list->array.size; \
		dynarray_add_fn(node_t(type_t))(&list->array, \
						(node_t(type_t)){.elt = elt}); \
		list->array.ptr[list->tail].index = size; \
		list->tail = size; \
	} \
	dynarray_define(node_t(type_t))

#define nodelist_deduced(type_t) \
	static_assert(nodelist_type(sizeof, (type_t){0}) == sizeof(type_t), \
		      STRING(type_t) " nodelist");

#endif // !GEN_NODELIST
