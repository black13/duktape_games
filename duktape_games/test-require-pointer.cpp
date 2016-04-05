#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
pointer: 0xdeadbeef
pointer: (nil)
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='TypeError: pointer required, found null (stack index 0)'
*** test_3 (duk_safe_call)
==> rc=1, result='TypeError: pointer required, found none (stack index 0)'
*** test_4 (duk_safe_call)
==> rc=1, result='TypeError: pointer required, found none (stack index -2147483648)'
===*/

static duk_ret_t test_1(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_pointer(ctx, (void *) 0xdeadbeef);
	duk_push_pointer(ctx, (void *) NULL);
	printf("pointer: %p\n", duk_require_pointer(ctx, 0));
	printf("pointer: %p\n", duk_require_pointer(ctx, 1));
	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_null(ctx);
	printf("pointer: %p\n", duk_require_pointer(ctx, 0));
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	duk_set_top(ctx, 0);
	printf("pointer: %p\n", duk_require_pointer(ctx, 0));
	return 0;
}

static duk_ret_t test_4(duk_context *ctx) {
	duk_set_top(ctx, 0);
	printf("pointer: %p\n", duk_require_pointer(ctx, DUK_INVALID_INDEX));
	return 0;
}

TEST_F(FooTest,test_require_pointer)
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1,test_2,test_3,test_4};

	for (auto f : t_vector)
	{
		auto pf = *(f.target<duk_safe_call_function>());
		TEST_SAFE_CALL(duk_instance.get(), pf);
	}
}
