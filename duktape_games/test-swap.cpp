#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
[ 123 234 345 456 567 ]
[ 123 234 345 456 567 ]
[ 123 456 345 234 567 ]
[ 123 456 567 234 345 ]
[ 123 456 567 234 345 ]
final top: 5
==> rc=0, result='undefined'
*** test_2a (duk_safe_call)
==> rc=1, result='Error: invalid stack index -3'
*** test_2b (duk_safe_call)
==> rc=1, result='Error: invalid stack index 6'
*** test_2c (duk_safe_call)
==> rc=1, result='Error: invalid stack index -2147483648'
*** test_2d (duk_safe_call)
==> rc=1, result='Error: invalid stack index -2147483648'
*** test_3a (duk_safe_call)
==> rc=1, result='Error: invalid stack index 0'
*** test_3b (duk_safe_call)
==> rc=1, result='Error: invalid stack index 2'
*** test_3c (duk_safe_call)
==> rc=1, result='Error: invalid stack index -2147483648'
===*/

static void dump_stack(duk_context *ctx) {
	duk_idx_t i, n;

	printf("[");
	n = duk_get_top(ctx);
	for (i = 0; i < n; i++) {
		printf(" %ld", (long) duk_get_int(ctx, i));
	}
	printf(" ]\n");
}

static duk_ret_t test_1(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_push_int(ctx, 345);
	duk_push_int(ctx, 456);
	duk_push_int(ctx, 567);  /* [ 123 234 345 456 567 ] */
	dump_stack(ctx);

	/* no-op swap */
	duk_swap(ctx, -1, -1);
	dump_stack(ctx);

	/* actual swap */
	duk_swap(ctx, 1, -2);  /* -> [ 123 456 345 234 567 ] */
	dump_stack(ctx);

	/* swap top */
	duk_swap_top(ctx, -3); /* -> [ 123 456 567 234 345 ] */
	dump_stack(ctx);

	/* no-op swap top */
	duk_swap_top(ctx, 4);
	dump_stack(ctx);

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_2a(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_swap(ctx, -1, -3);  /* second index out of bounds */

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_2b(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_swap(ctx, 6, 1);  /* first index out of bounds */

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_2c(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_swap(ctx, DUK_INVALID_INDEX, 0);  /* first index DUK_INVALID_INDEX */

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_2d(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_swap(ctx, 0, DUK_INVALID_INDEX);  /* second index DUK_INVALID_INDEX */

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_3a(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_swap_top(ctx, 0);  /* empty stack */

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_3b(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_swap_top(ctx, 2);  /* index out of bounds */

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_3c(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_int(ctx, 123);
	duk_push_int(ctx, 234);
	duk_swap_top(ctx, DUK_INVALID_INDEX);  /* index is DUK_INVALID_INDEX */

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

// void test(duk_context *ctx) {
	// ,test_1);
	// ,test_2a);
	// ,test_2b);
	// ,test_2c);
	// ,test_2d);
	// ,test_3a);
	// ,test_3b);
	// ,test_3c);
// }

TEST_F(FooTest, test_swap)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1,test_2a,test_2b,test_2c,test_2d,test_3a,test_3b,test_3c};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
