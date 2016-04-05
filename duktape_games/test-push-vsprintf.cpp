#include "duk_test_util.h"
/*
 *  Try different format string lengths.
 */

/*===
*** test_basic (duk_safe_call)
length sum: 2147713027.000000
==> rc=0, result='undefined'
===*/

static char buf[65536 + 1024];
static double len_sum = 0.0;

static void test_2(duk_context *ctx, int fmt_len, va_list ap) {
	int i;

	for (i = 0; i < fmt_len; i++) {
		buf[i] = 'x';
	}
	buf[fmt_len + 0] = '%';
	buf[fmt_len + 1] = 'd';
	buf[fmt_len + 2] = '\0';

	duk_push_vsprintf(ctx, buf, ap);
	len_sum += (double) duk_get_length(ctx, -1);  /* trivial "checksum" */
	duk_pop(ctx);
}

static void test_1(duk_context *ctx, int fmt_len, ...) {
	va_list ap;

	va_start(ap, fmt_len);
	test_2(ctx, fmt_len, ap);
	va_end(ap);
}

static duk_ret_t test_basic(duk_context *ctx) {
	duk_size_t fmt_len;

	/* Note: don't reuse 'ap' in multiple calls, so the fmt_len loop
	 * is here.
	 */

	for (fmt_len = 0; fmt_len <= 65536; fmt_len++) {
		test_1(ctx, fmt_len, 123, 234, 345);
	}

	/* Length sequence is 3, 4, ..., 65539 ->
	 * (65539 + 3)/2 * (65539 - 3 + 1) = 2147713027
	 *
	 * (See test-push-sprintf for comments.)
	 */
	printf("length sum: %lf\n", len_sum);
	return 0;
}

// void test(duk_context *ctx) {
	// (test_basic);
// }

TEST_F(FooTest, test_push_vsprintf)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_basic};
    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
