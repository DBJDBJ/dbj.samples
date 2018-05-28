#include "stdafx.h"

#ifdef DBJ_TESTING_EXISTS

DBJ_TEST_UNIT(": dbj any + dbj narf ") {

	auto naf =  DBJ_TEST_ATOM( dbj::narf::make({ 1,2,3 })  );
};

#endif // DBJ_TESTING_EXISTS