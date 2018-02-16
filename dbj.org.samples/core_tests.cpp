
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
/*
DBJ Test unit signature

DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": Test Title")) {

	dbj::print("\nINSIDE TEST UNIT\n");

}
*/
#ifdef DBJ_TESTING_EXISTS
#pragma region DBJ_TESTING_EXISTS

DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": dbj std array helpers")) {

	dbj::print("\nEntering dbj::arr::test()\n");
	dbj::arr::test();
	dbj::print("\nExited   dbj::arr::test()\n");
}

#pragma endregion
#endif // DBJ_TESTING_EXISTS