#include "duk_test_util.h"
/*
 *  Supporting testcase for test-dev-return-types.js: cover return types
 *  which can't be exercised from Ecmascript code.
 */

/*===
*** test_basic_implicit (duk_safe_call)
inside func
result: undefined
final top: 0
==> rc=0, result='undefined'
*** test_basic_explicit (duk_safe_call)
inside func
result: 123
final top: 0
==> rc=0, result='undefined'
*** test_endfin_return (duk_safe_call)
finally
result: 321
final top: 0
==> rc=0, result='undefined'
inside func
result: undefined
inside func
result: undefined
inside func
result: 123
inside func
result: 123
finally
result: 321
finally
result: 321
final top: 0
===*/

/* Normal call from a duk_safe_call() wrapper, implicit return value. */
static duk_ret_t test_basic_implicit(duk_context *ctx) {
	duk_eval_string(ctx,
		"(function () {\n"
		"    print('inside func');\n"
		"})");
	duk_call(ctx, 0);
	printf("result: %s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

/* Normal call from a duk_safe_call() wrapper, explicit return value. */
static duk_ret_t test_basic_explicit(duk_context *ctx) {
	duk_eval_string(ctx,
		"(function () {\n"
		"    print('inside func');\n"
		"    return 123;\n"
		"})");
	duk_call(ctx, 0);
	printf("result: %s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}

/* Ecmascript finally captures return and the return is propagated
 * onwards after finally finishes.
 */
static duk_ret_t test_endfin_return(duk_context *ctx) {
	duk_eval_string(ctx,
		"(function () {\n"
		"    try {\n"
		"        return 321;\n"
		"    } finally {\n"
		"        print('finally');\n"
		"    }\n"
		"    print('never here');\n"
		"})");
	duk_call(ctx, 0);
	printf("result: %s\n", duk_safe_to_string(ctx, -1));
	duk_pop(ctx);

	printf("final top: %ld\n", (long) duk_get_top(ctx));
	return 0;
}
//
//void test(duk_context *ctx) {
//	TEST_SAFE_CALL(test_basic_implicit);
//	TEST_SAFE_CALL(test_basic_explicit);
//	TEST_SAFE_CALL(test_endfin_return);
//
//	/* Top level unprotected call + return with implicit value. */
//	duk_eval_string(ctx,
//		"(function () {\n"
//		"    print('inside func');\n"
//		"})");
//	duk_call(ctx, 0);
//	printf("result: %s\n", duk_safe_to_string(ctx, -1));
//	duk_pop(ctx);
//
//	/* Top level protected call + return with explicit value. */
//	duk_eval_string(ctx,
//		"(function () {\n"
//		"    print('inside func');\n"
//		"})");
//	(void) duk_pcall(ctx, 0);
//	printf("result: %s\n", duk_safe_to_string(ctx, -1));
//	duk_pop(ctx);
//
//	/* Top level unprotected call + return with explicit value. */
//	duk_eval_string(ctx,
//		"(function () {\n"
//		"    print('inside func');\n"
//		"    return 123;\n"
//		"})");
//	duk_call(ctx, 0);
//	printf("result: %s\n", duk_safe_to_string(ctx, -1));
//	duk_pop(ctx);
//
//	/* Top level protected call + return with explicit value. */
//	duk_eval_string(ctx,
//		"(function () {\n"
//		"    print('inside func');\n"
//		"    return 123;\n"
//		"})");
//	duk_pcall(ctx, 0);
//	printf("result: %s\n", duk_safe_to_string(ctx, -1));
//	duk_pop(ctx);
//
//	/* ENDFIN + RETURN case directly from top level, unprotected call. */
//	duk_eval_string(ctx,
//		"(function () {\n"
//		"    try {\n"
//		"        return 321;\n"
//		"    } finally {\n"
//		"        print('finally');\n"
//		"    }\n"
//		"    print('never here');\n"
//		"})");
//	duk_call(ctx, 0);
//	printf("result: %s\n", duk_safe_to_string(ctx, -1));
//	duk_pop(ctx);
//
//	/* ENDFIN + RETURN case directly from top level, protected call. */
//	duk_eval_string(ctx,
//		"(function () {\n"
//		"    try {\n"
//		"        return 321;\n"
//		"    } finally {\n"
//		"        print('finally');\n"
//		"    }\n"
//		"    print('never here');\n"
//		"})");
//	duk_pcall(ctx, 0);
//	printf("result: %s\n", duk_safe_to_string(ctx, -1));
//	duk_pop(ctx);
//
//	printf("final top: %ld\n", (long) duk_get_top(ctx));
//}

TEST_F(FooTest,test_dev_return_types)
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });

	test_function_vector t_vector;
	
	t_vector.emplace_back(test_basic_implicit);
	t_vector.emplace_back(test_basic_explicit);
	t_vector.emplace_back(test_endfin_return);

	auto safe_call = std::bind(duk_safe_call, duk_instance.get(), std::placeholders::_1, 0, 1);

	//f1(*(fn0.target<duk_safe_call_function>())); // 1 is bound by _2, 2 is bound by _1, 1001 is unused
	for (auto f : t_vector)
	{
		safe_call(*(f.target<duk_safe_call_function>()));
	}


	/* Top level unprotected call + return with implicit value. */
	duk_eval_string(duk_instance.get(),
		"(function () {\n"
		"    print('inside func');\n"
		"})");
	duk_call(duk_instance.get(), 0);
	printf("result: %s\n", duk_safe_to_string(duk_instance.get(), -1));
	duk_pop(duk_instance.get());

	/* Top level protected call + return with explicit value. */
	duk_eval_string(duk_instance.get(),
		"(function () {\n"
		"    print('inside func');\n"
		"})");
	(void)duk_pcall(duk_instance.get(), 0);
	printf("result: %s\n", duk_safe_to_string(duk_instance.get(), -1));
	duk_pop(duk_instance.get());

	/* Top level unprotected call + return with explicit value. */
	duk_eval_string(duk_instance.get(),
		"(function () {\n"
		"    print('inside func');\n"
		"    return 123;\n"
		"})");
	duk_call(duk_instance.get(), 0);
	printf("result: %s\n", duk_safe_to_string(duk_instance.get(), -1));
	duk_pop(duk_instance.get());

	/* Top level protected call + return with explicit value. */
	duk_eval_string(duk_instance.get(),
		"(function () {\n"
		"    print('inside func');\n"
		"    return 123;\n"
		"})");
	duk_pcall(duk_instance.get(), 0);
	printf("result: %s\n", duk_safe_to_string(duk_instance.get(), -1));
	duk_pop(duk_instance.get());

	/* ENDFIN + RETURN case directly from top level, unprotected call. */
	duk_eval_string(duk_instance.get(),
		"(function () {\n"
		"    try {\n"
		"        return 321;\n"
		"    } finally {\n"
		"        print('finally');\n"
		"    }\n"
		"    print('never here');\n"
		"})");
	duk_call(duk_instance.get(), 0);
	printf("result: %s\n", duk_safe_to_string(duk_instance.get(), -1));
	duk_pop(duk_instance.get());

	/* ENDFIN + RETURN case directly from top level, protected call. */
	duk_eval_string(duk_instance.get(),
		"(function () {\n"
		"    try {\n"
		"        return 321;\n"
		"    } finally {\n"
		"        print('finally');\n"
		"    }\n"
		"    print('never here');\n"
		"})");
	duk_pcall(duk_instance.get(), 0);
	printf("result: %s\n", duk_safe_to_string(duk_instance.get(), -1));
	duk_pop(duk_instance.get());

	printf("final top: %ld\n", (long)duk_get_top(duk_instance.get()));

}
