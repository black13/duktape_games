#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
buffer: ptr-is-NULL=0, sz=1024
buffer
buffer: ptr-is-NULL=-1, sz=0
buffer
buffer: ptr-is-NULL=0, sz=1024
buffer
buffer: ptr-is-NULL=-1, sz=0
buffer
buffer: ptr-is-NULL=0, sz=12
buffer
==> rc=0, result='undefined'
*** test_2 (duk_safe_call)
==> rc=1, result='TypeError: buffer required, found null (stack index 0)'
*** test_3 (duk_safe_call)
==> rc=1, result='TypeError: buffer required, found none (stack index 0)'
*** test_4 (duk_safe_call)
==> rc=1, result='TypeError: buffer required, found none (stack index -2147483648)'
===*/

static duk_ret_t test_1(duk_context *ctx) {
	void *ptr;
	duk_size_t sz;
	int i;

	duk_set_top(ctx, 0);
	duk_push_fixed_buffer(ctx, 1024);
	duk_push_fixed_buffer(ctx, 0);
	duk_push_dynamic_buffer(ctx, 1024);
	duk_push_dynamic_buffer(ctx, 0);
	duk_eval_string(ctx, "(function () { return new Uint32Array(16).subarray(3, 6); })()");

	for (i = 0; i < 5; i++) {
		sz = (duk_size_t) 0xdeadbeefUL;
		ptr = duk_require_buffer_data(ctx, i, &sz);
		printf("buffer: ptr-is-NULL=%d, sz=%ld\n",
		       (sz == 0 ? -1 : (ptr == NULL ? 1 : 0)), (long) sz);

		/* NULL pointer */
		sz = (duk_size_t) 0xdeadbeefUL;
		ptr = duk_require_buffer_data(ctx, i, NULL);
		printf("buffer\n");
	}

	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	void *ptr;
	duk_size_t sz;

	duk_set_top(ctx, 0);
	duk_push_null(ctx);
	sz = (duk_size_t) 0xdeadbeefUL;
	ptr = duk_require_buffer_data(ctx, 0, &sz);
	printf("buffer ok: %p\n", ptr);  /* ok to print, should not be reached */
	return 0;
}

static duk_ret_t test_3(duk_context *ctx) {
	void *ptr;
	duk_size_t sz;

	duk_set_top(ctx, 0);
	sz = (duk_size_t) 0xdeadbeefUL;
	ptr = duk_require_buffer_data(ctx, 0, &sz);
	printf("buffer ok: %p\n", ptr);  /* ok to print, should not be reached */
	return 0;
}

static duk_ret_t test_4(duk_context *ctx) {
	void *ptr;
	duk_size_t sz;

	duk_set_top(ctx, 0);
	sz = (duk_size_t) 0xdeadbeefUL;
	ptr = duk_require_buffer_data(ctx, DUK_INVALID_INDEX, &sz);
	printf("buffer ok: %p\n", ptr);  /* ok to print, should not be reached */
	return 0;
}

TEST_F(FooTest, test_require_buffer_data)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1,test_2,test_3,test_4};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
