/*===
*** test_encode (duk_safe_call)
base64 encode: Zm9v
top after: 2
==> rc=0, result='undefined'
*** test_decode (duk_safe_call)
base64 decode: test string
top after: 2
==> rc=0, result='undefined'
*** test_decode_invalid_char (duk_safe_call)
==> rc=1, result='TypeError: decode failed'
===*/
#include "duk_test_util.h"
static duk_ret_t test_encode(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_string(ctx, "foo");
	duk_push_int(ctx, 123);  /* dummy */
	printf("base64 encode: %s\n", duk_base64_encode(ctx, -2));
	printf("top after: %ld\n", (long) duk_get_top(ctx));  /* value + dummy */
	return 0;
}

static duk_ret_t test_decode(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_string(ctx, "dGVzdCBzdHJpbmc=");
	duk_push_int(ctx, 321);  /* dummy */
	duk_base64_decode(ctx, -2);  /* buffer */
	printf("base64 decode: %s\n", duk_to_string(ctx, -2));
	printf("top after: %ld\n", (long) duk_get_top(ctx));  /* value + dummy */
	duk_set_top(ctx, 0);
	return 0;
}

static duk_ret_t test_decode_invalid_char(duk_context *ctx) {
	duk_set_top(ctx, 0);
	duk_push_string(ctx, "dGVzdCBzdHJ@bmc=");
	duk_push_int(ctx, 321);  /* dummy */
	duk_base64_decode(ctx, -2);  /* buffer */
	printf("base64 decode: %s\n", duk_to_string(ctx, -2));
	printf("top after: %ld\n", (long) duk_get_top(ctx));  /* value + dummy */
	duk_set_top(ctx, 0);
	return 0;
}

//void test(duk_context *ctx) {
//	TEST_SAFE_CALL(test_encode);
//	TEST_SAFE_CALL(test_decode);
//	TEST_SAFE_CALL(test_decode_invalid_char);
//}


BOOST_AUTO_TEST_CASE( test_base64 )
{
	duk_smart_ptr ptr(duk_create_heap_default(), [](duk_context* mtx) { duk_destroy_heap(mtx); });

	duk_test_function_ptr f1(test_encode);
	duk_test_function_ptr f2(test_decode);
	duk_test_function_ptr f3(test_decode_invalid_char);
	//std::unique_ptr<duk_context,std::function<void(duk_context*)>> ptr(duk_create_heap_default(), [](duk_context* mtx) { duk_destroy_heap(mtx); });
	try
	{
		f1(ptr.get());
		f2(ptr.get());
		//f3(ptr.get());
	}
	catch(...)
	{
	}
	//f=test_1
    //TEST_DUK(test_1);
}