#include "duk_test_util.h"

void runme(duk_smart_ptr & ptr,std::function<duk_ret_t(duk_context*)> & f)
{
	//std::unique_ptr<duk_context,std::function<void(duk_context*)>> ptr(duk_create_heap_default(), [](duk_context* mtx) { duk_destroy_heap(mtx); });

	f(ptr.get());
}

#ifdef __cplusplus
extern "C" {
#endif

void  TEST_SAFE_CALL(duk_context *ctx,duk_safe_call_function func)
{
	duk_ret_t _rc;
	//printf(\"*** %s (duk_safe_call)\n", #func);
	fflush(stdout);
	_rc = duk_safe_call(ctx, (func), 0, 1);
	printf("==> rc=%d, result='%s'\n", (int)_rc, duk_safe_to_string(ctx, -1));
	fflush(stdout);
	duk_pop(ctx);

}
void  PCALL(duk_context *ctx,duk_safe_call_function func)
{
	duk_ret_t _rc;
	//printf(\"*** %s (duk_pcall)\n", #func);
	fflush(stdout);
	duk_push_c_function(ctx, (func), 0);
	_rc = duk_pcall(ctx, 0);
	printf("==> rc=%d, result='%s'\n", (int)_rc, duk_safe_to_string(ctx, -1));
	fflush(stdout);
	duk_pop(ctx);

}
#ifdef __cplusplus
}
#endif