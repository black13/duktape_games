/*
 *  Duktape 1.2.1: duk_is_dynamic_buffer() and duk_is_fixed_buffer() does a
 *  NULL pointer dereference when the index is invalid.
 */

/*===
*** test_1 (duk_safe_call)
duk_is_dynamic_buffer(-1): 0
duk_is_fixed_buffer(-1): 0
final top: 0
==> rc=0, result='undefined'
===*/
#include "duk_test_util.h"

static duk_ret_t test_1(duk_context *ctx) {
	/* These would segfault or at least cause valgrind issues. */
	printf("duk_is_dynamic_buffer(-1): %d\n", (int) duk_is_dynamic_buffer(ctx, -1));
	printf("duk_is_fixed_buffer(-1): %d\n", (int) duk_is_fixed_buffer(ctx, -1));

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

//void test(duk_context *ctx) {
//	TEST_SAFE_CALL(test_1);
//}

TEST_F(FooTest,test_bug_is_fixed_dynamic_buffer_nullptr)
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });

	test_function_vector t_vector;
	t_vector.emplace_back(test_1);
	//t_vector.emplace_back(test_2);

	auto safe_call = std::bind(duk_safe_call, duk_instance.get(), std::placeholders::_1, 0, 1);

	//f1(*(fn0.target<duk_safe_call_function>())); // 1 is bound by _2, 2 is bound by _1, 1001 is unused
	for (auto f : t_vector)
	{
		safe_call(*(f.target<duk_safe_call_function>()));
	}

	//std::for_each(t_vector.begin(), t_vector.end(), [](auto & t){});
	return;
}