#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='TypeError: null required, found undefined (stack index 0)'
*** test_3 (duk_safe_call)
==> rc=1, result='TypeError: null required, found none (stack index 0)'
*** test_4 (duk_safe_call)
==> rc=1, result='TypeError: null required, found none (stack index -2147483648)'
===*/

static duk_ret_t test_1(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_null(ctx);
	duk_require_null(ctx, 0);
	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_undefined(ctx);
	duk_require_null(ctx, 0);
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_require_null(ctx, 0);
	printf("require 0 OK\n");
	return 0;
}

static duk_ret_t test_4(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_require_null(ctx, DUK_INVALID_INDEX);
	printf("require DUK_INVALID_INDEX OK\n");
	return 0;
}

TEST_F(FooTest,test_require_null)
{
	run_tests({test_1,test_2,test_3,test_4});
}
