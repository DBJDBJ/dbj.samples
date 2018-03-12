#include "stdafx.h"

// #include "cppwinrt\dbj_web_syndication.h"

extern "C" void quick_local_tests(decltype(dbj::print) & print);

int main(int argc, char* argv[])
{
	quick_local_tests(dbj::print);
	dbj::testing::execute();
	return 1;
}
