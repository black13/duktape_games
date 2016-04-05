#include "duk_test_util.h"
/*===
top: 0
top: 1
top: 0
value: 123
top: 0
===*/

static duk_ret_t test(duk_context *ctx) {
	printf("top: %ld\n", (long) duk_get_top(ctx));
	duk_push_heap_stash(ctx);
	printf("top: %ld\n", (long) duk_get_top(ctx));

	duk_push_int(ctx, 123);
	duk_put_prop_string(ctx, -2, "myvalue");
	duk_pop(ctx);
	printf("top: %ld\n", (long) duk_get_top(ctx));

	duk_push_heap_stash(ctx);
	duk_get_prop_string(ctx, -1, "myvalue");
	printf("value: %ld\n", (long) duk_get_int(ctx, -1));
	duk_pop(ctx);
	duk_pop(ctx);
	printf("top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}TEST_F(FooTest, test_heap_stash)
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = { test };

	for (auto f : t_vector)
	{
		auto pf = *(f.target<duk_safe_call_function>());
		TEST_SAFE_CALL(duk_instance.get(), pf);
	}
}
