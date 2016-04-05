#include "duktape.h"
#include <iostream>
#include <tuple>
#include <memory>
#include <functional>
#include <boost/test/unit_test.hpp>
#include <vector>

typedef std::unique_ptr<duk_context,std::function<void(duk_context*)>> duk_smart_ptr;
typedef std::function<duk_ret_t(duk_context*)> duk_test_function_ptr;
typedef std::function<duk_ret_t(duk_context *)> test_function;
typedef std::vector<test_function> test_function_vector;

void run_tests(const test_function_vector & vectors);