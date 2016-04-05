#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
top: 18
index 0, boolean: 0
index 1, boolean: 0
index 2, boolean: 1
index 3, boolean: 0
index 4, boolean: 0
index 5, boolean: 1
index 6, boolean: 0
index 7, boolean: 1
index 8, boolean: 0
index 9, boolean: 1
index 10, boolean: 1
index 11, boolean: 1
index 12, boolean: 0
index 13, boolean: 1
index 14, boolean: 0
index 15, boolean: 1
index 16, boolean: 0
index 17, boolean: 1
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='Error: invalid stack index 3'
*** test_3 (duk_safe_call)
==> rc=1, result='Error: invalid stack index -2147483648'
===*/

static duk_ret_t test_1(duk_context *ctx) {
	duk_idx_t i, n;

	duk_set_top(ctx, 0);
	duk_push_undefined(ctx);
	duk_push_null(ctx);
	duk_push_true(ctx);
	duk_push_false(ctx);
	duk_push_int(ctx, 0);
	duk_push_int(ctx, 1);
	duk_push_nan(ctx);
	duk_push_number(ctx, INFINITY);
	duk_push_string(ctx, "");
	duk_push_string(ctx, "foo");
	duk_push_object(ctx);
	duk_push_thread(ctx);
	duk_push_fixed_buffer(ctx, 0);
	duk_push_fixed_buffer(ctx, 1024);
	duk_push_dynamic_buffer(ctx, 0);
	duk_push_dynamic_buffer(ctx, 1024);
	duk_push_pointer(ctx, (void *) NULL);
	duk_push_pointer(ctx, (void *) 0xdeadbeef);

	n = duk_get_top(ctx);
	printf("top: %ld\n", (long) n);
	for (i = 0; i < n; i++) {
		printf("index %ld, boolean: %d\n", (long) i, (int) duk_to_boolean(ctx, i));
	}

	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_to_boolean(ctx, 3);
	printf("index 3 OK\n");
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_to_boolean(ctx, DUK_INVALID_INDEX);
	printf("index DUK_INVALID_INDEX OK\n");
	return 0;
}

TEST_F(FooTest, test_to_boolean)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1,test_2,test_3};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
