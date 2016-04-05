#include "duk_test_util.h"
/*
 *  If a function modifies its own 'magic' value while it is executing,
 *  the change is visible immediately.
 *
 *  This is not necessarily stable behavior so user code should not rely
 *  on this.  The current behavior is a side effect of the way the current
 *  call stack state keeping is implemented.  This test case just documents
 *  the current behavior.
 */

/*===
*** test_1 (duk_safe_call)
current magic (on entry): 345
current magic (after set): 456
final top: 1
==> rc=0, result='undefined'
===*/

static duk_ret_t my_func(duk_context *ctx) {
	printf("current magic (on entry): %ld\n", (long) duk_get_current_magic(ctx));

	duk_push_current_function(ctx);
	duk_set_magic(ctx, -1, 456);

	printf("current magic (after set): %ld\n", (long) duk_get_current_magic(ctx));

	return 0;
}

static duk_ret_t test_1(duk_context *ctx) {
	duk_push_c_function(ctx, my_func, 2 /*nargs*/);
	duk_set_magic(ctx, -1, 345);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_call(ctx, 2);

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

// void test(duk_context *ctx) {
	// (test_1);
// }

TEST_F(FooTest, test_magic_modify_during_call)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1};
    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
