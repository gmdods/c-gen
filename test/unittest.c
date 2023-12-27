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
