#include "duk_test_util.h"
/*===
duk_create_heap_default() succeeded
concat test: 'foobarquux'
destroyed successfully
still here
===*/

static duk_ret_t  test(duk_context *ctx) {
	duk_context *new_ctx;

	/*
	 *  Create a new heap with default handlers
	 *
	 *  (wrapper heap is not used in this testcase)
	 */

	new_ctx = duk_create_heap_default();
	if (new_ctx) {
		printf(" duk_create_heap_default() succeeded\n");

		duk_push_string(new_ctx, "foo");
		duk_push_string(new_ctx, "bar");
		duk_push_string(new_ctx, "quux");
		duk_concat(new_ctx, 3);
		printf(" concat test: '%s'\n", duk_get_string(new_ctx, -1));
	} else {
		printf(" context allocation failed\n");
	}

	duk_destroy_heap(new_ctx);
	new_ctx = NULL;
	printf(" destroyed successfully\n");

	/*
	 *  NULL free is a no-op
	 */

	duk_destroy_heap(NULL);
	printf(" still here\n");
	return 0;
}

TEST_F(FooTest,test_heap_lifecycle_basic)
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = { test };

	for (auto f : t_vector)
	{
		auto pf = *(f.target<duk_safe_call_function>());
		TEST_SAFE_CALL(duk_instance.get(), pf);
	}
}
