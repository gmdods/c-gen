#ifndef UNITTEST_MAIN
#include "unittest.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "dynarray_t.h"

#endif /* ifndef UNITTEST_MAIN */

unittest("init deinit") {
	dynarray_t(int) array = dynarray_init(int)(1);
	ensure(array.ptr != NULL);
	array.ptr[0] = 42;
	dynarray_deinit(int)(&array);
	ensure(array.ptr == NULL);
}
