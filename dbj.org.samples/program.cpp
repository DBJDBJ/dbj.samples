#include "stdafx.h"

#if 0
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

#include <dbj_string_compare.h>
#include "headers/no_inheritance.h"

#include <dbj_native_arr_ref.h>
#endif

#pragma warning( push )
#pragma warning( disable: 4100 )
// https://msdn.microsoft.com/en-us/library/26kb9fy0.aspx 

/// <summary>
/// just execute all the registered tests
/// in no particulat order
/// </summary>
static void program_start (
	const int argc, 
	const wchar_t *argv[], 
	const wchar_t *envp[]
) {
	dbj::testing::execute(argc, argv, envp);
}

#if 0

static dbj::c_line<80, '-'> L80;

static bool fundamental(
	const int argc,	const wchar_t *argv[],const wchar_t *envp[]) 
{
	using namespace std;

	auto print = [&](auto & x)
	{
		DBJ_TEST_ATOM(x);
		using x_type = remove_reference_t< decltype(x) >;
		static_assert(
			is_array<x_type>(), "argument must be an native array reference"
			);
		size_t j{ 0 }; 
		for (auto && e : x)
		{ dbj::print("\n [", j++, L"] : " , e ); }
		return "done";
	};

	DBJ_TEST_ATOM(argv[0]);
	// pointer to array
	typedef  const wchar_t *(*ARP)[1];
	// ref to array
	typedef  const wchar_t *(&ARF)[1];
	// output is 
	// just the first letter of the
	// full path
	DBJ_TEST_ATOM( print(*(ARP)(argv)) );
	// output is 
	// garbage
	DBJ_TEST_ATOM( print((ARF)(argv))  );
	return true;
}
#endif

#ifdef UNICODE
int wmain(const int argc, const wchar_t *argv[], const wchar_t *envp[])
#else
#error "What could be the [t]reason this is not UNICODE build?"
int main(int argc, char* argv[], char *envp[])
#endif
{
	try {
		program_start(argc, argv, envp);
	} catch (...) {
		using namespace dbj::win::con;
		dbj::print(
		painter_command::bright_red,
		__FUNCSIG__ "  Unknown exception caught! ",
		painter_command::text_color_reset
		);
	}
	return  EXIT_SUCCESS;
}

#pragma warning( pop ) // 4100
