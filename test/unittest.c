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
	array.ptr[0] = 42;
	dynarray_reserve(&array, 10);
	ensure(array.ptr != NULL);

	for (size_t i = 0; i != 10; ++i) {
		dynarray_add(&array, i + 1);
	}
	ensure(array.size == 10);

	dynarray_deinit(&array);
	ensure(array.ptr == NULL);
}

unittest("nodelist") {
	nodelist_t(uint) list = nodelist_init(uint, 8);
	ensure(list.array.ptr != NULL);
	for (size_t i = 0; i != 10; ++i) {
		nodelist_add(&list, 4 * i + 1);
	}
	ensure(list.array.size == 11);

	for (size_t head = 0, i = 0; (head = list.array.ptr[head].index) != 0; ++i) {
		ensure(4 * i + 1 == list.array.ptr[head].elt);
	}

	nodelist_deinit(&list);
	ensure(list.array.ptr == NULL);
}
