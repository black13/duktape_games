#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
==> rc=1, result='throw me'
===*/

int test_1(duk_context *ctx) {
	duk_push_string(ctx, "throw me");
	duk_throw(ctx);
	return 0;
}

TEST_F(FooTest, test_throw)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1};
    for (auto f : t_vector)
    {   
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
