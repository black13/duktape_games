#include "duk_test_util.h"
/*===
result: foo; 123; true
===*/

static duk_ret_t test(duk_context *ctx) {
	duk_push_string(ctx, "; ");
	duk_push_string(ctx, "foo");
	duk_push_int(ctx, 123);
	duk_push_true(ctx);
	duk_join(ctx, 3);

	printf("result: %s\n", duk_get_string(ctx, -1));
	duk_pop(ctx);
	return 0;
}

TEST_F(FooTest,test_join)
{
	run_tests({ test });
}

