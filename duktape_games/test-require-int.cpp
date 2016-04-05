#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
number: 123.000000 -> int: 123
number: 123.456000 -> int: 123
number: nan -> int: 0
number: -inf -> int: DUK_INT_MIN
number: inf -> int: DUK_INT_MAX
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='TypeError: number required, found null (stack index 0)'
*** test_3 (duk_safe_call)
==> rc=1, result='TypeError: number required, found none (stack index 0)'
*** test_4 (duk_safe_call)
==> rc=1, result='TypeError: number required, found none (stack index -2147483648)'
===*/

static duk_ret_t test_1(duk_context *ctx) {
	duk_idx_t i;

	duk_set_top(ctx, 0);
	duk_push_int(ctx, 123);
	duk_push_number(ctx, 123.456);
	duk_push_nan(ctx);
	duk_push_number(ctx, -INFINITY);
	duk_push_number(ctx, INFINITY);

	for (i = 0; i < 5; i++) {
		duk_int_t ival = duk_require_int(ctx, i);
		duk_double_t dval = duk_get_number(ctx, i);

		printf("number: %lf -> int: ", (double) dval);
		if (ival == DUK_INT_MIN) {
			printf("DUK_INT_MIN");
		} else if (ival == DUK_INT_MAX) {
			printf("DUK_INT_MAX");
		} else {
			printf("%ld", (long) ival);
		}
		printf("\n");
	}
	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_null(ctx);
	printf("int: %ld\n", (long) duk_require_int(ctx, 0));
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	duk_set_top(ctx, 0);
	printf("int: %ld\n", (long) duk_require_int(ctx, 0));
	return 0;
}

static duk_ret_t test_4(duk_context *ctx) {
	duk_set_top(ctx, 0);
	printf("int: %ld\n", (long) duk_require_int(ctx, DUK_INVALID_INDEX));
	return 0;
}


TEST_F(FooTest,test_require_int)
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = { test_1, test_2, test_3, test_4 };

	for (auto f : t_vector)
	{
		auto pf = *(f.target<duk_safe_call_function>());
		TEST_SAFE_CALL(duk_instance.get(), pf);
	}
}