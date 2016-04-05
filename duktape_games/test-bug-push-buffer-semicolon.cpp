#include "duk_test_util.h"
/*
 *  https://githubom/svaarala/duktape/issues/500
 */

/*===
still here
===*/

void test(duk_context *ctx) {
	(void) (duk_push_buffer(ctx, 123, 0), "dummy");
	printf("still here\n");
}

BOOST_AUTO_TEST_CASE( test_bug_push_buffer_semicolon)
{
	run_tests({ test});
}
