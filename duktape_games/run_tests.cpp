#include "duk_test_util.h"


void run_tests(const test_function_vector & vectors)
{
	duk_smart_ptr duk_instance(duk_create_heap_default(), [](duk_context* f) { duk_destroy_heap(f); });

	test_function_vector t_vector;
	//t_vector.emplace_back(test_to_buffer_1);
	//t_vector.emplace_back(test_2);

	auto safe_call = std::bind(duk_safe_call, duk_instance.get(), std::placeholders::_1, 0, 1);

	//f1(*(fn0.target<duk_safe_call_function>())); // 1 is bound by _2, 2 is bound by _1, 1001 is unused
	for (auto f : vectors)
	{
		safe_call(*(f.target<duk_safe_call_function>()));
	}
}