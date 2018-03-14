#include "stdafx.h"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	constexpr decltype(auto) see_me = dbj::YEAR;
	(void)see_me;
#endif

	dbj::testing::execute();
	return 1;
}
