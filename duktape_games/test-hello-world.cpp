#include "duk_test_util.h"
/*===
Hello world from Ecmascript!
Hello world from C!
===*/

static duk_ret_t test(duk_context *ctx) {
	duk_push_string(ctx, "print('Hello world from Ecmascript!');");
	duk_eval(ctx);
	printf("Hello world from C!\n");
	return 0;
}TEST_F(FooTest,test_hello_world)
{

	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = { test };

	for (auto f : t_vector)
	{
		auto pf = *(f.target<duk_safe_call_function>());
		TEST_SAFE_CALL(duk_instance.get(), pf);
	}
}
