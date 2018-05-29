// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "stdafx.h"

auto && description = dbj::testing::FILELINE(__FILE__, __LINE__, " test_fun ");

void test_fun() {

	auto naf [[maybe_unused]] = DBJ_TEST_ATOM(dbj::narf::make({ 1,2,3 }));
};

namespace { 
  
	auto caller = [&]() -> bool 
	{
	auto && registration
      = * dbj::testing::add( description, test_fun , __COUNTER__ );
	
	std::string desc = registration.second;
	dbj::testing::testunittype func_p = registration.first;

		dbj::print("\nCalling ", desc );
		func_p();
		return true;
	};
	inline auto call_ = caller();
}
