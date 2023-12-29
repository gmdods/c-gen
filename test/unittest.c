#include <stdio.h>
#ifndef UNITTEST_MAIN
#include "unittest.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "gen_t.h"

#endif /* ifndef UNITTEST_MAIN */

unittest("init reserve add deinit") {
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
		ensure(dynarray_at(array, i+ 1) == (int) (i + 1));
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
	ensure(list.array.size == nodes);
	nodelist_insert(&list, 1, 1);

	size_t index = 0;

	index = nodes;
	for (size_t head = 0; (head = nodelist_link(list, head));) {
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
	for (size_t head = 0; (head = nodelist_link(list, head));) {
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
