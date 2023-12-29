#ifndef GEN_HASHMAP
#define GEN_HASHMAP

#include "macros.h"
#include <assert.h>
#include <stdlib.h>

#define keyval_t(key_t, val_t) CONCAT(CONCAT(key_t, _to_), val_t)
#define hashmap_t(key_t, val_t) struct CONCAT(hashmap_, keyval_t(key_t, val_t))

#define hashmap_deinit_fn(kv_t) CONCAT(hashmap_deinit_, kv_t)
#define hashmap_reserve_fn(kv_t) nodelist_reserve_fn(kv_t)
#define hashmap_add_fn(kv_t) CONCAT(hashmap_add_, kv_t)
#define hashmap_del_fn(kv_t) CONCAT(hashmap_del_, kv_t)
#define hashmap_lookup_fn(kv_t) CONCAT(hashmap_lookup_, kv_t)

#define hashmap_init(key_t, val_t, sz, sl) \
	(hashmap_t(key_t, val_t)) { \
		.array = dynarray_init(size_t, sl), \
		.list = nodelist_init(keyval_t(key_t, val_t), sz) \
	}
#define hashmap_associated(hashmap_fn, map_ref, ...) \
	hashmap_type(hashmap_fn, (map_ref)->list.array.ptr->elt)( \
	    map_ref __VA_OPT__(, ) __VA_ARGS__)
#define hashmap_trait(hashmap_fn, map, ...) \
	hashmap_type(hashmap_fn, (map).list.array.ptr->elt)(map __VA_OPT__(, ) \
								__VA_ARGS__)
#define hashmap_deinit(map_ref) hashmap_associated(hashmap_deinit_fn, map_ref)
#define hashmap_reserve(map_ref, sz) \
	hashmap_type(hashmap_reserve_fn, (map_ref)->list.array.ptr->elt)( \
	    &(map_ref)->list.array, sz)
#define hashmap_add(map_ref, elt) \
	hashmap_associated(hashmap_add_fn, map_ref, elt)
#define hashmap_del(map_ref, key) \
	hashmap_associated(hashmap_del_fn, map_ref, key)
#define hashmap_lookup(map, key) hashmap_trait(hashmap_lookup_fn, map, key)

#define hashmap_declare(key_t, val_t) \
	typedef struct { \
		key_t key; \
		val_t val; \
	} keyval_t(key_t, val_t); \
	dynarray_declare(size_t) nodelist_declare(keyval_t(key_t, val_t)) \
	    hashmap_t(key_t, val_t) { \
		dynarray_t(size_t) array; \
		nodelist_t(keyval_t(key_t, val_t)) list; \
	}; \
	void hashmap_deinit_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) *); \
	void hashmap_add_fn(keyval_t(key_t, val_t))(hashmap_t(key_t, val_t) *, \
						    keyval_t(key_t, val_t)); \
	void hashmap_del_fn(keyval_t(key_t, val_t))(hashmap_t(key_t, val_t) *, \
						    key_t); \
	val_t hashmap_lookup_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t), key_t);

#define hashmap_define(key_t, val_t) \
	void hashmap_deinit_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) * map) { \
		if (!map) return; \
		dynarray_deinit_fn(size_t)(&map->array); \
		nodelist_deinit_fn(keyval_t(key_t, val_t))(&map->list); \
		*map = (hashmap_t(key_t, val_t)){0}; \
	} \
	void hashmap_add_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) * map, keyval_t(key_t, val_t) elt) { \
		size_t slot = elt.key & (map->array.capacity - 1); \
		size_t chain = dynarray_at(map->array, slot); \
		size_t index = nodelist_insert_fn(keyval_t(key_t, val_t))( \
		    &map->list, chain, elt); \
		if (chain == 0) \
			map->array.size += 1, \
			    dynarray_at(map->array, slot) = index; \
	} \
	void hashmap_del_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) * map, key_t key) { \
		size_t slot = key & (map->array.capacity - 1); \
		size_t chain = dynarray_at(map->array, slot); \
		nodelist_t(keyval_t(key_t, val_t)) proxy = map->list; \
		proxy.head = chain; \
		for (size_t prev = 0, head = chain; head; \
		     prev = head, head = nodelist_link(map->list, head)) { \
			if (nodelist_at(map->list, head).key == key) { \
				nodelist_remove_fn(keyval_t(key_t, val_t))( \
				    &proxy, prev); \
				dynarray_at(map->array, slot) = proxy.head; \
				map->list = proxy; \
			} \
		} \
	} \
	val_t hashmap_lookup_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) map, key_t key) { \
		size_t slot = key & (map.array.capacity - 1); \
		size_t chain = dynarray_at(map.array, slot); \
		for (size_t head = chain; head; \
		     head = nodelist_link(map.list, head)) { \
			if (nodelist_at(map.list, head).key == key) { \
				return nodelist_at(map.list, head).val; \
			} \
		} \
		return (val_t){0}; \
	} \
	dynarray_define(size_t) nodelist_define(keyval_t(key_t, val_t))

#define hashmap_deduced(key_t, val_t) \
	static_assert(hashmap_type(sizeof, (keyval_t(key_t, val_t)){0}) == \
			  sizeof(keyval_t(key_t, val_t)), \
		      STRING(key_t) ":" STRING(val_t) " hashmap");

#endif // !GEN_HASHMAP
