#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
0 bytes:
16 bytes: 0 0 0 0 0 123 0 0 0 0 0 0 0 0 0 65
Stole buffer: buf-is-NULL=0, sz=16
0 bytes:
buf[0] = 0
buf[1] = 0
buf[2] = 0
buf[3] = 0
buf[4] = 0
buf[5] = 123
buf[6] = 0
buf[7] = 0
buf[8] = 0
buf[9] = 0
buf[10] = 0
buf[11] = 0
buf[12] = 0
buf[13] = 0
buf[14] = 0
buf[15] = 65
==> rc=0, result='undefined'
===*/

static void dump_buffer(duk_context *ctx) {
	unsigned char *p;
	duk_size_t i, sz;

	p = (unsigned char *) duk_require_buffer(ctx, -1, &sz);
	printf("%ld bytes:", (long) sz);
	for (i = 0; i < sz; i++) {
		printf(" %d", (int) p[i]);
	}
	printf("\n");
}

static duk_ret_t test_1(duk_context *ctx) {
	unsigned char *p;
	unsigned char *buf;
	duk_size_t sz;
	int i;

	duk_push_dynamic_buffer(ctx, 0);
	dump_buffer(ctx);

	duk_resize_buffer(ctx, -1, 16);
	p = (unsigned char *) duk_require_buffer(ctx, -1, NULL);
	p[5] = (unsigned char) 123;
	p[15] = (unsigned char) 65;
	dump_buffer(ctx);

	buf = (unsigned char*)duk_steal_buffer(ctx, -1, &sz);
	printf("Stole buffer: buf-is-NULL=%d, sz=%d\n",
	       (int) (buf == NULL ? 1 : 0), (int) sz);

	/* Buffer is now zero length and could be reused. */
	dump_buffer(ctx);

	/* No effect on allocation returned by duk_steal_buffer(). */
	duk_set_top(ctx, 0);

	for (i = 0; i < (int) sz; i++) {
		printf("buf[%d] = %d\n", i, (int) buf[i]);
	}

	/* Caller must free. */
#if 1  /* If disabled, valgrind detects a leak. */
	duk_free(ctx, (void *) buf);
#endif

	return 0;
}

TEST_F(FooTest, test_steal_buffer)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
