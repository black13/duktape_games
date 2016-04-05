#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
concat: foobarquux
still here
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='TypeError: thread required, found 123 (stack index -1)'
*** test_3 (duk_safe_call)
==> rc=1, result='TypeError: thread required, found none (stack index 123)'
*** test_4 (duk_safe_call)
==> rc=1, result='TypeError: thread required, found none (stack index -2147483648)'
===*/

static duk_ret_t test_1(duk_context *ctx) {
	duk_context *new_ctx;

	duk_set_top(ctx, 0);
	(void) duk_push_thread(ctx);
	new_ctx = duk_require_context(ctx, -1);

	duk_push_string(new_ctx, "foo");
	duk_push_string(new_ctx, "bar");
	duk_push_string(new_ctx, "quux");
	duk_concat(new_ctx, 3);
	printf("concat: %s\n", duk_get_string(new_ctx, -1));

	/* This duk_pop() makes the new thread unreachable (assuming there
	 * is no other reference to it), so new_ctx is no longer valid
	 * afterwards.
	 */
	duk_pop(ctx);

	printf("still here\n");
	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_context *new_ctx;

	/* non-thread value */
	duk_set_top(ctx, 0);
	duk_push_int(ctx, 123);
	new_ctx = duk_require_context(ctx, -1);
	printf("new_ctx is NULL: %d\n", (new_ctx == NULL ? 1 : 0));
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	duk_context *new_ctx;

	/* invalid index */
	duk_set_top(ctx, 0);
	duk_push_int(ctx, 123);
	new_ctx = duk_require_context(ctx, 123);
	printf("new_ctx is NULL: %d\n", (new_ctx == NULL ? 1 : 0));
	return 0;
}

static duk_ret_t test_4(duk_context *ctx) {
	duk_context *new_ctx;

	/* invalid index */
	duk_set_top(ctx, 0);
	duk_push_int(ctx, 123);
	new_ctx = duk_require_context(ctx, DUK_INVALID_INDEX);
	printf("new_ctx is NULL: %d\n", (new_ctx == NULL ? 1 : 0));
	return 0;
}

// void test(duk_context *ctx) {
	// (test_1);
	// (test_2);
	// (test_3);
	// (test_4);
// }

TEST_F(FooTest, test_require_context)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1,test_2,test_3,test_4};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
