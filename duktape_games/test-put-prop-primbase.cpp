#include "duk_test_util.h"
/*===
*** test_put (duk_safe_call)
==> rc=1, result='TypeError: cannot write property 'foo' of 0'
*** test_put (duk_pcall)
==> rc=1, result='TypeError: cannot write property 'foo' of 0'
===*/

duk_ret_t test_put(duk_context *ctx) {
	duk_ret_t rc;

	/* In Ecmascript, '(0).foo = "bar"' should work and evaluate to "bar"
	 * in non-strict mode, but cause an error to be thrown in strict mode
	 * (E5.1, Section 8.7.2, exotic [[Put]] variant, step 7.
	 */

	duk_push_int(ctx, 0);
	duk_push_string(ctx, "foo");
	duk_push_string(ctx, "bar");
	rc = duk_put_prop(ctx, -3);

	printf("put rc=%d\n", (int) rc);

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}


TEST_F(FooTest, test_put_prop_primbase)
{
    duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });
	test_function_vector t_vector = {test_put};  /* outside: strict (non_strict before 0.12.0) */

    for (auto f : t_vector)
    {
        auto pf = *(f.target<duk_safe_call_function>());
		PCALL(duk_instance.get(), pf);
    }
}
