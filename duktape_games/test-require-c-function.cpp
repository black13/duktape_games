#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
duk_require_c_function == my_func: 1
final top: 1
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='TypeError: nativefunction required, found none (stack index 3)'
*** test_3 (duk_safe_call)
==> rc=1, result='TypeError: nativefunction required, found none (stack index -2147483648)'
===*/

static duk_ret_t my_func(duk_context *ctx) {
	return 0;
}

static duk_ret_t test_1(duk_context *ctx) {
	duk_c_function funcptr;

	duk_set_top(ctx, 0);
	duk_push_c_function(ctx, my_func, 1 /*nargs*/);
	funcptr = duk_require_c_function(ctx, -1);
	printf("duk_require_c_function == my_func: %d\n", (funcptr == my_func ? 1 : 0));

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_c_function funcptr;

	duk_set_top(ctx, 0);
	duk_push_c_function(ctx, my_func, 1 /*nargs*/);
	funcptr = duk_require_c_function(ctx, 3);
	printf("index 3 -> NULL: %d\n", (funcptr == NULL ? 1 : 0));

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	duk_c_function funcptr;

	duk_set_top(ctx, 0);
	duk_push_c_function(ctx, my_func, 1 /*nargs*/);
	funcptr = duk_require_c_function(ctx, DUK_INVALID_INDEX);
	printf("index DUK_INVALID_INDEX -> NULL: %d\n", (funcptr == NULL ? 1 : 0));

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

// void test(duk_context *ctx) {
	// (test_1);
	// (test_2);
	// (test_3);
// }

TEST_F(FooTest, test_require_c_function)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1,test_2,test_3};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
