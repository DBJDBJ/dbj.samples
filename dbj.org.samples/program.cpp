#include "stdafx.h"

int main()
{
	try {
		dbj::testing::execute();
	} catch (dbj::testing::Exception & x ) {
		print("\n\nException thrown from the testing unit\n\n%\n\n", x.what());
	}
	return 0;
}

