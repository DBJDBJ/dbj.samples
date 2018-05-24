
#include "stdafx.h"
// core dbj++ tests
#define DBJ_TESTING_EXISTS
#include <dbj_testing.h>
#include <test\dbj_commander_test.h>
#include <test\dbj_console_test.h>
#include <test\dbj_various_tests.h>
#include <test\dbj_crt_testing.h>
#include <test\dbj_defval_testing.h>
#include <dbj_array.h>
#include <dbj_util.h> // also includes dbj_traits.h
/*
DBJ Test unit signature

DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": Test Title")) {

	dbj::print("\nINSIDE TEST UNIT\n");

}
*/
#ifdef DBJ_TESTING_EXISTS

DBJ_TEST_UNIT(": dbj utils") {

	dbj_util_test( dbj::print );
}

#endif // DBJ_TESTING_EXISTS