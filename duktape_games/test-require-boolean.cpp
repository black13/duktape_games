#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
boolean: 1
boolean: 0
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='TypeError: boolean required, found null (stack index 0)'
*** test_3 (duk_safe_call)
==> rc=1, result='TypeError: boolean required, found none (stack index 0)'
*** test_4 (duk_safe_call)
==> rc=1, result='TypeError: boolean required, found none (stack index -2147483648)'
===*/

static duk_ret_t test_1(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_true(ctx);
	duk_push_false(ctx);
	printf("boolean: %d\n", (int) duk_require_boolean(ctx, 0));
	printf("boolean: %d\n", (int) duk_require_boolean(ctx, 1));
	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_null(ctx);
	printf("boolean: %d\n", (int) duk_require_boolean(ctx, 0));
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	duk_set_top(ctx, 0);
	printf("boolean: %d\n", (int) duk_require_boolean(ctx, 0));
	return 0;
}

static duk_ret_t test_4(duk_context *ctx) {
	duk_set_top(ctx, 0);
	printf("boolean: %d\n", (int) duk_require_boolean(ctx, DUK_INVALID_INDEX));
	return 0;
}

// void test(duk_context *ctx) {
	// (test_1);
	// (test_2);
	// (test_3);
	// (test_4);
// }

TEST_F(FooTest, test_require_boolean)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1,test_2,test_3,test_4};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
