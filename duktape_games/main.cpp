
#include <iostream>
#include <tuple>
#include <memory>
#include <functional>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

#include "duk_test_util.h"

#if 0
static duk_ret_t my_adder(duk_context *ctx) {
	duk_idx_t i, n;
	double res = 0.0;

	duk_push_this(ctx);
	printf(" this binding: '%s'\n", duk_to_string(ctx, -1));
	duk_pop(ctx);

	n = duk_get_top(ctx);
	for (i = 0; i < n; i++) {
		if (!duk_is_number(ctx, i)) {
			duk_error(ctx, DUK_ERR_TYPE_ERROR, "argument %ld is not a number", (long)i);
		}
		res += duk_get_number(ctx, i);
	}

	duk_push_number(ctx, res);
	return 1;
}

static duk_ret_t test_1(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_c_function(ctx, my_adder, 3 /*nargs*/);
	duk_push_int(ctx, 10);
	duk_push_int(ctx, 11);
	duk_push_int(ctx, 12);
	duk_push_int(ctx, 13);  /* clipped */
	duk_push_int(ctx, 14);  /* clipped */
	duk_call(ctx, 5);

	printf(" result=%s\n", duk_to_string(ctx, -1));
	duk_pop(ctx);

	printf(" final top: %ld\n", (long)duk_get_top(ctx));
	return 0;
}

static duk_ret_t test_2(duk_context *ctx) {
	duk_set_top(ctx, 0);

	duk_push_c_function(ctx, my_adder, 3 /*nargs*/);
	duk_push_int(ctx, 10);
	duk_push_int(ctx, 11);
	duk_push_int(ctx, 12);
	duk_push_int(ctx, 13);  /* clipped */
	duk_push_int(ctx, 14);  /* clipped */
	duk_call(ctx, 5);

	printf(" result=%s\n", duk_to_string(ctx, -1));
	duk_pop(ctx);

	printf(" final top: %ld\n", (long)duk_get_top(ctx));
	return 0;
}


int main()
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });

	test_function_vector t_vector;
	t_vector.emplace_back(test_1);
	t_vector.emplace_back(test_2);

	auto safe_call = std::bind(duk_safe_call, duk_instance.get(),std::placeholders::_1, 0, 1);
	
	//f1(*(fn0.target<duk_safe_call_function>())); // 1 is bound by _2, 2 is bound by _1, 1001 is unused
	for (auto f : t_vector)
	{
		auto pf = *(f.target<duk_safe_call_function>());
		duk_safe_call(duk_instance.get(),, 0, 1)
		//safe_call(*(f.target<duk_safe_call_function>()));
	}

	//f1(test_1);
	return 0;
}

#endif

FooTest::FooTest() {
}

FooTest::~FooTest() {
}

void FooTest::SetUp() {
}

void FooTest::TearDown() {
}

