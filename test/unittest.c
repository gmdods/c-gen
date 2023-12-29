#include <stdio.h>
#ifndef UNITTEST_MAIN
#include "unittest.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "gen_t.h"

#endif /* ifndef UNITTEST_MAIN */

unittest("dynarray : init reserve add deinit") {
	dynarray_t(int) array = dynarray_init(int, 1);
	ensure(array.ptr != NULL);
	ensure(array.size == 0);
	ensure(array.capacity == 1);

	dynarray_add(&array, 0);
	dynarray_at(array, 0) = 42;
	dynarray_reserve(&array, 10);
	ensure(array.ptr != NULL);

	for (size_t i = 0; i != 10; ++i) {
		dynarray_add(&array, i + 1);
	}
	ensure(array.size == 11);

	ensure(dynarray_at(array, 0) == 42);
	for (size_t i = 0; i != 10; ++i) {
		ensure(dynarray_at(array, i + 1) == (int) (i + 1));
	}

	dynarray_deinit(&array);
	ensure(array.ptr == NULL);
}

unittest("nodelist") {
	nodelist_t(uint) list = nodelist_init(uint, 8);
	ensure(list.array.ptr != NULL);
	const size_t nodes = 10;
	for (size_t i = 1; i != nodes; ++i) {
		nodelist_cons(&list, 4 * i + 1);
	}
	ensure(list.array.size == nodes - 1);
	nodelist_insert(&list, 1, 1);

	size_t index = 0;

	index = nodes;
	for (size_t head = list.head; head; head = nodelist_link(list, head)) {
		if (1 == nodelist_at(list, head))
			ensure(index == 1);
		else
			ensure(4 * (--index) + 1 == nodelist_at(list, head));
	}
	ensure(index == 1);
	nodelist_reserve(&list, 20);

	nodelist_remove(&list, 0);
	nodelist_remove(&list, 1);
	nodelist_remove(&list, 3);

	index = nodes;
	index -= 1; // removed node
	for (size_t head = list.head; head; head = nodelist_link(list, head)) {
		index -= 1 + (index == 3); // removed node
		ensure(4 * index + 1 == nodelist_at(list, head));
	}
	ensure(index == 1);

	index = 0;
	for (size_t freelist = list.freelist; freelist;
	     freelist = nodelist_link(list, freelist), ++index) {
		ensure(!nodelist_at(list, freelist));
	}
	ensure(index == 3);

	nodelist_deinit(&list);
	ensure(list.array.ptr == NULL);
}

unittest("hashmap") {
	nodelist_t(keyval_t) list = nodelist_init(keyval_t, 8);
	dynarray_t(size_t) array = dynarray_init(size_t, 16);
	{
		keyval_t map1 = {.key = 3, .val = true};
		size_t slot = map1.key & 0xF;
		size_t index = nodelist_cons(&list, map1);
		array.size += 1;
		dynarray_at(array, slot) = index;
	}
	{
		keyval_t map2 = {.key = 19, .val = true};
		size_t slot = map2.key & 0xF;
		size_t chain = dynarray_at(array, slot);
		size_t index = nodelist_insert(&list, chain, map2);
		array.size += 1;
		if (chain == 0) dynarray_at(array, slot) = index;
	}
	{
		uint key = 3;
		size_t slot = key & 0xF;
		size_t chain = dynarray_at(array, slot);
		bool val = false;
		for (size_t head = chain; head;
		     head = nodelist_link(list, head)) {
			if (nodelist_at(list, head).key == key) {
				val = true;
				break;
			}
		}
		ensure(val);
	}
}
