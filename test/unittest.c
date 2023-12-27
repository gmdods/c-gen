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
	ensure(array.size == 1);
	array.ptr[0] = 42;
	array.size = 0;
	dynarray_reserve(&array, 10);
	ensure(array.ptr != NULL);

	for (size_t i = 0; i != 10; ++i) {
		dynarray_add(&array, i + 1);
	}
	ensure(array.size == 10);

	dynarray_deinit(&array);
	ensure(array.ptr == NULL);
}

unittest("composite type") {
	dynarray_t(node_t) list = dynarray_init(node_t, 8);
	list.size = 0;
	ensure(list.ptr != NULL);
	dynarray_add(&list, (node_t){0});
	size_t tail = 0;

	for (size_t i = 0; i != 10; ++i) {
		list.ptr[tail].index = list.size;
		tail = list.size;
		dynarray_add(&list, (node_t){.elt = 4 * i + 1});
	}
	ensure(list.size == 11);

	for (size_t head = 0, i = 0; (head = list.ptr[head].index) != 0; ++i) {
		ensure(4 * i + 1 == list.ptr[head].elt);
	}

	dynarray_deinit(&list);
	ensure(list.ptr == NULL);
}
