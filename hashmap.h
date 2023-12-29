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
#define hashmap_at_fn(kv_t) CONCAT(hashmap_at_, kv_t)

#define hashmap_init(key_t, val_t, sz, sl) \
	(hashmap_t(key_t, val_t)) { \
		.array = dynarray_init(size_t, sl), \
		.list = nodelist_init(keyval_t(key_t, val_t), sz).pool \
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
#define hashmap_at(map, key) (*hashmap_associated(hashmap_at_fn, map, key))

#define hashmap_declare(key_t, val_t) \
	typedef struct { \
		key_t key; \
		val_t val; \
	} keyval_t(key_t, val_t); \
	nodelist_declare(keyval_t(key_t, val_t)) hashmap_t(key_t, val_t) { \
		dynarray_t(size_t) array; \
		nodelist_pool_t(keyval_t(key_t, val_t)) list; \
	}; \
	void hashmap_deinit_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) *); \
	void hashmap_add_fn(keyval_t(key_t, val_t))(hashmap_t(key_t, val_t) *, \
						    keyval_t(key_t, val_t)); \
	void hashmap_del_fn(keyval_t(key_t, val_t))(hashmap_t(key_t, val_t) *, \
						    key_t); \
	val_t hashmap_lookup_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t), key_t); \
	val_t * hashmap_at_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) *, key_t);

#define hashmap_define(key_t, val_t) \
	void hashmap_deinit_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) * map) { \
		if (!map) return; \
		dynarray_deinit_fn(size_t)(&map->array); \
		dynarray_deinit_fn(node_t(keyval_t(key_t, val_t)))( \
		    &map->list.array); \
		*map = (hashmap_t(key_t, val_t)){0}; \
	} \
	void hashmap_add_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) * map, keyval_t(key_t, val_t) elt) { \
		size_t slot = \
		    hashmap_hash(elt.key) & (map->array.capacity - 1); \
		nodelist_t(keyval_t(key_t, val_t)) node = { \
		    .head = dynarray_at(map->array, slot), .pool = map->list}; \
		map->array.size += (node.head == 0); \
		nodelist_cons_fn(keyval_t(key_t, val_t))(&node, elt); \
		map->list = node.pool; \
		dynarray_at(map->array, slot) = node.head; \
	} \
	void hashmap_del_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) * map, key_t key) { \
		size_t slot = hashmap_hash(key) & (map->array.capacity - 1); \
		nodelist_t(keyval_t(key_t, val_t)) node = { \
		    .head = dynarray_at(map->array, slot), .pool = map->list}; \
		for (size_t prev = 0, head = node.head; head; \
		     prev = head, head = nodelist_link(node, head)) { \
			if (nodelist_at(node, head).key == key) { \
				map->array.size -= (prev == 0); \
				nodelist_remove_fn(keyval_t(key_t, val_t))( \
				    &node, prev); \
				map->list = node.pool; \
				dynarray_at(map->array, slot) = node.head; \
			} \
		} \
	} \
	val_t hashmap_lookup_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) map, key_t key) { \
		size_t slot = hashmap_hash(key) & (map.array.capacity - 1); \
		nodelist_t(keyval_t(key_t, val_t)) node = { \
		    .head = dynarray_at(map.array, slot), .pool = map.list}; \
		for (size_t head = node.head; head; \
		     head = nodelist_link(node, head)) { \
			if (nodelist_at(node, head).key == key) { \
				return nodelist_at(node, head).val; \
			} \
		} \
		return (val_t){0}; \
	} \
	val_t * hashmap_at_fn(keyval_t(key_t, val_t))( \
	    hashmap_t(key_t, val_t) * map, key_t key) { \
		size_t slot = hashmap_hash(key) & (map->array.capacity - 1); \
		nodelist_t(keyval_t(key_t, val_t)) node = { \
		    .head = dynarray_at(map->array, slot), .pool = map->list}; \
		for (size_t head = node.head; head; \
		     head = nodelist_link(node, head)) { \
			if (nodelist_at(node, head).key == key) { \
				return &nodelist_at(node, head).val; \
			} \
		} \
		map->array.size += (node.head == 0); \
		nodelist_cons_fn(keyval_t(key_t, val_t))( \
		    &node, (keyval_t(key_t, val_t)){.key = key}); \
		map->list = node.pool; \
		dynarray_at(map->array, slot) = node.head; \
		return &nodelist_at(node, node.head).val; \
	} \
	nodelist_define(keyval_t(key_t, val_t))

#define hashmap_deduced(key_t, val_t) \
	static_assert(hashmap_type(sizeof, (keyval_t(key_t, val_t)){0}) == \
			  sizeof(keyval_t(key_t, val_t)), \
		      STRING(key_t) ":" STRING(val_t) " hashmap");

#endif // !GEN_HASHMAP
