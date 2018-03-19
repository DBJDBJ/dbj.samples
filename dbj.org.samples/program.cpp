#include "stdafx.h"

#include "dbj_any/dbj_any.h"
#include  "dbj_x\dbj_compiletime.h"
#include  "dbj_x\dbj_ctstring.h"
#include  "dbj_x\dbj_fontlist.h"
#include  "dbj_x\dbj_micro_printf.h"
#include  "dbj_x\string_literals_with_universal_character_names.h"

#include "msvc_conformance\dbj_std_arr_handling.h"
#include "msvc_conformance\lambda_runtime_retval_test.h"
#include "msvc_conformance\new_nuggets.h"
#include "msvc_conformance\supports_api.h"

#include "dbj_x\dbj_argv_argw.h"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	constexpr decltype(auto) see_me = dbj::YEAR;
	(void)see_me;
#endif

	dbj::testing::execute();
	return 1;
}
