#include "duk_test_util.h"
/*===
*** test_1 (duk_safe_call)
top=4
duk_safe_to_string[0] = '123'
top=4
duk_safe_to_string[1] = 'toString result'
top=4
duk_safe_to_string[2] = 'Error: toString error'
top=4
duk_safe_to_string[3] = 'Error'
top=4
duk_safe_to_lstring_null[0] = '123'
top=4
duk_safe_to_lstring_null[1] = 'toString result'
top=4
duk_safe_to_lstring_null[2] = 'Error: toString error'
top=4
duk_safe_to_lstring_null[3] = 'Error'
top=4
duk_safe_to_lstring[0] = '123', len 3
top=4
duk_safe_to_lstring[1] = 'toString result', len 15
top=4
duk_safe_to_lstring[2] = 'Error: toString error', len 21
top=4
duk_safe_to_lstring[3] = 'Error', len 5
top=4
duk_safe_to_lstring[0] = '123', len 3
top=4
duk_safe_to_lstring[1] = 'toString result', len 15
top=4
duk_safe_to_lstring[2] = 'Error: toString error', len 21
top=4
duk_safe_to_lstring[3] = 'Error', len 5
==> rc=0, result='undefined'
===*/

static void init_test_values(duk_context *ctx) {
	duk_set_top(ctx, 0);

	/* Simple */
	duk_push_int(ctx, 123);

	/* Object with toString() */
	duk_eval_string(ctx, "({ toString: function () { return 'toString result'; } })");

	/* toString() throws an error */
	duk_eval_string(ctx, "({ toString: function () { throw new Error('toString error'); } })");

	/* toString() throws an error which cannot be string coerced */
	duk_eval_string(ctx, "({ toString: function () { var e = new Error('cannot string coerce me');"
	                     "                           e.toString = function () { throw new Error('coercion error'); };"
	                     "                           throw e; } })");

	/* XXX: add an infinite loop and timeout case */
}

static int test_1(duk_context *ctx) {
	duk_idx_t i, n;

	/* duk_safe_to_string() */
	init_test_values(ctx);
	n = duk_get_top(ctx);
	for (i = 0; i < n; i++) {
		printf("top=%ld\n", (long) duk_get_top(ctx));
		printf("duk_safe_to_string[%ld] = '%s'\n", (long) i, duk_safe_to_string(ctx, i));
	}

	/* duk_safe_to_lstring() with NULL arg */
	init_test_values(ctx);
	n = duk_get_top(ctx);
	for (i = 0; i < n; i++) {
		const char *str;
		printf("top=%ld\n", (long) duk_get_top(ctx));
		str = duk_safe_to_lstring(ctx, i, NULL);
		printf("duk_safe_to_lstring_null[%ld] = '%s'\n", (long) i, str);
	}

	/* duk_safe_to_lstring() */
	init_test_values(ctx);
	n = duk_get_top(ctx);
	for (i = 0; i < n; i++) {
		const char *str;
		duk_size_t len;
		printf("top=%ld\n", (long) duk_get_top(ctx));
		str = duk_safe_to_lstring(ctx, i, &len);
		printf("duk_safe_to_lstring[%ld] = '%s', len %lu\n", (long) i, str, (unsigned long) len);
	}

	/* duk_safe_to_lstring() with negative stack indices */
	init_test_values(ctx);
	n = duk_get_top(ctx);
	for (i = 0; i < n; i++) {
		const char *str;
		duk_size_t len;
		printf("top=%ld\n", (long) duk_get_top(ctx));
		str = duk_safe_to_lstring(ctx, -4 + i, &len);
		printf("duk_safe_to_lstring[%ld] = '%s', len %lu\n", (long) i, str, (unsigned long) len);
	}

	return 0;
}

TEST_F(FooTest, test_safe_to_string)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_1};

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
        TEST_SAFE_CALL(duk_instance.get(), pf);
    }
}
