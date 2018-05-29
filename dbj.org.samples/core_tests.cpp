
#include "stdafx.h"
/*
DBJ Test unit signature

DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": Test Title")) {

	dbj::print("\nINSIDE TEST UNIT\n");

}
*/

#ifdef DBJ_TESTING_EXISTS

namespace {

	DBJ_TEST_UNIT(": dbj math float_to_integer test ") {

		using namespace dbj::math::float_to_integer;

		auto test = [](float val_) {
			dbj::print("\n\nInput:\t", DBJ_NV(val_));
			dbj::print("\n", DBJ_NV(nearest(val_)));
			dbj::print("\n", DBJ_NV(drop_fractional_part(val_)));
			dbj::print("\n", DBJ_NV(next_highest(val_)));
			dbj::print("\n", DBJ_NV(integer(val_)));
		};

		test(123.00f);
		test(123.500f);
		test(123.4901f);
		test(123.501f);
	}

	DBJ_TEST_UNIT(": dbj dbj_util_test ") {

		int intarr[]{ 1,1,2,2,3,4,5,6,6,6,7,8,9,9,0,0 };
		auto ret1 = dbj::util::remove_duplicates(intarr);
		std::string as2[16]{
			"abra", "ka", "dabra", "babra",
			"abra", "ka", "dabra", "babra",
			"abra", "ka", "dabra", "babra",
			"abra", "ka", "dabra", "babra",
		};

		auto ad = dbj::util::remove_duplicates(as2);
		char carr[] = { 'c','a','b','c','c','c','d', 0x0 };
		auto rez [[maybe_unused]] = dbj::util::remove_duplicates(carr);
		auto see_mee_here = carr;
		auto doesit1 = dbj::util::starts_with("abra ka dabra", "abra");
		auto doesit2 = dbj::util::starts_with(L"abra ka dabra", L"abra");
	};

}

#endif // DBJ_TESTING_EXISTS