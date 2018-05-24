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

#include <dbj_string_compare.h>
#include "headers/no_inheritance.h"

#include <dbj_native_arr_ref.h>


extern void quick_local_tests();

#pragma warning( push )
#pragma warning( disable: 4100 )
// https://msdn.microsoft.com/en-us/library/26kb9fy0.aspx 

/// <summary>
/// just execute all the registered tests
/// in no particulat order
/// </summary>
static void program_start
(const int argc, const wchar_t *argv[], const wchar_t *envp[])
{
	quick_local_tests();
	dbj::testing::execute();
}

std::wostream & operator << ( std::wostream & wos, const std::string & str ) {
	return wos << (const char *)str.c_str();
}

template<typename T>
constexpr wchar_t tv_print (T && v_, const wchar_t  prompt[] = L"" ) {
	using v_type = std::remove_reference_t< T >;
	std::wcout << prompt << L" { " <<  dbj::name< v_type >() << L" : " << (v_) << " } " ;
	return L' ';
};

#ifdef UNICODE
int wmain(const int argc, const wchar_t *argv[], const wchar_t *envp[])
#else
#error "What could be the [t]reason this is not UNICODE build?"
int main(int argc, char* argv[], char *envp[])
#endif
{
	using namespace std;
	auto print = [&](auto & x)
	{
	    tv_print(x,L"\n\ninside auto print = [](auto & x)\nx : ");
		using x_type = remove_reference_t< decltype(x) >;
	static_assert(
		is_array<x_type>() , "argument must be an native array reference"
	);

	size_t j{ 0 }; for (auto e : x) 
			std::wcout << L"\n [" << j++ << L"] : " << tv_print(e) ;
	};

	tv_print( argv[0], L"\n argv [0] : ");
   // pointer to array
	typedef  const char *(*ARP)[1];
	// ref to array
	typedef  const char *(&ARF)[1];
	// output is 
	// just the first letter of the
	// full path
	print( * (ARP)(argv));
	// output is 
	// garbage
	print((ARF)(argv));

	wcout.flush(); return  EXIT_SUCCESS;
}

#pragma warning( pop ) // 4100
