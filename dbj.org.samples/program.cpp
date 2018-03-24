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

#include "dbj_x\dbj_cli_args.h"

/// <summary>
/// just execute all the registered tests
/// in no particulat order
/// </summary>
#ifdef UNICODE
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
#else
int main(int argc, char* argv[], char *envp[])
#endif
{
	dbj::testing::execute();
	return  EXIT_SUCCESS;
}
