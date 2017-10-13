#include "stdafx.h"

template <typename T> T identity(T); 

int main()
{
#if DBJ_TESTING_EXISTS

	try {
		dbj::testing::execute();
	}
	catch (...) {
		printex("Unknown exception caught");
	}
	
#endif
	return 0;
}

