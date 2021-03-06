#include "duk_test_util.h"
/*===
result: foo123true
===*/

void test(duk_context *ctx) {
	duk_push_string(ctx, "foo");
	duk_push_int(ctx, 123);
	duk_push_true(ctx);
	duk_concat(ctx, 3);

	printf("result: %s\n", duk_get_string(ctx, -1));
	duk_pop(ctx);
}

BOOST_AUTO_TEST_CASE(test-concat)
{
	run_tests({ test_string, test_lstring });
}
