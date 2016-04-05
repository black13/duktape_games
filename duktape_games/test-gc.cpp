#include "duk_test_util.h"
/*===
still here
===*/

void test(duk_context *ctx) {
	duk_gc(ctx, 0);
	duk_gc(ctx, 0);
	duk_gc(ctx, 0);
	printf("still here\n");
}

BOOST_AUTO_TEST_CASE( test-gc
