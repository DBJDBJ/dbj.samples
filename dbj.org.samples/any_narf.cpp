

#include "stdafx.h"
#ifdef DBJ_TESTING_EXISTS
#include <dbj_native_arr_ref.h>
#include "dbj_any/dbj_any.h"

DBJ_TEST_UNIT(": dbj any + dbj narf ") {

	auto naf = dbj::narf::make({ 1,2,3 });
};

#endif // DBJ_TESTING_EXISTS