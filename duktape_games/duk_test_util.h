#ifndef DUK_TEST_UTIL_H_
#define DUK_TEST_UTIL_H_

#include "duktape.h"
#include <iostream>
#include <tuple>
#include <memory>
#include <functional>
#include <vector>
#include <gtest\gtest.h>

typedef std::unique_ptr<duk_context,std::function<void(duk_context*)>> duk_smart_ptr;
typedef std::function<duk_ret_t(duk_context*)> duk_test_function_ptr;
typedef std::function<duk_ret_t(duk_context *)> test_function;
typedef std::vector<test_function> test_function_vector;

void run_tests(const test_function_vector & vectors);



#ifdef __cplusplus
extern "C" {
#endif
	void  TEST_SAFE_CALL(duk_context *ctx, duk_safe_call_function func);
	void  PCALL(duk_context *ctx, duk_safe_call_function func);
#ifdef __cplusplus
}
#endif

enum GTestColor {
	COLOR_DEFAULT,
	COLOR_RED,
	COLOR_GREEN,
	COLOR_YELLOW
};

class FooTest : public ::testing::Test 
{
protected:
	FooTest();
	virtual ~FooTest();
	virtual void SetUp();
	virtual void TearDown();
};

void ColoredPrintf(GTestColor color, const char* fmt, ...);
#endif