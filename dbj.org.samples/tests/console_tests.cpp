// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

#include <dbj_console_ops.h>


// **********************************************************************************************
// DBJ_TEST_SPACE_OPEN(console)
// **********************************************************************************************

/// <summary>
/// https://en.cppreference.com/w/cpp/language/types
/// </summary>
void fundamental_types_to_console()
{
	using dbj::console::out;
	using dbj::console::PRN ;
	using namespace std;

	PRN.printf("\n");
	out((nullptr_t) nullptr);
	PRN.printf("\n");
	out(true);

	// std chars
	PRN.printf("\n");
	out((signed char) 'X');
	PRN.printf("\n");
	out((unsigned char) 'X');
	PRN.printf("\n");
	out((char) 'X');
	PRN.printf("\n");
	out((wchar_t) 'X');
	PRN.printf("\n");
	out((char16_t) 'X');
	PRN.printf("\n");
	out((char32_t) 'X');
	// integer types
	PRN.printf("\n");
	out((short    int) 'X');
	PRN.printf("\n");
	out((unsigned short int) 'X');
	PRN.printf("\n");
	out((int) 'X');
	PRN.printf("\n");
	out((unsigned       int) 'X');
	PRN.printf("\n");
	out((long    int) 'X');
	PRN.printf("\n");
	out((unsigned long  int) 'X');
	PRN.printf("\n");
	out((long     long  int) 'X');
	PRN.printf("\n");
	out((unsigned long  long  int) 'X');
	// float types
	PRN.printf("\n");
	out((float) 42.99);
	PRN.printf("\n");
	out((double) 42.99);
	PRN.printf("\n");
	out((long double) 42.99);
	// nan float's
	PRN.printf("\n");
	out((float)			nanf("1"));
	PRN.printf("\n");
	out((double)		nan ("1"));
	PRN.printf("\n");
	out((long double)	nanl ("1"));
}

template< typename T>
void string_tester ( T * init_str_literal)
{
	using dbj::console::out;
	using dbj::console::PRN;
	using namespace std;

	// DBJ_TYPE_REPORT_FUNCSIG;

	auto actual_tests = [&](auto && std_string) 
	{
		using dbj::console::out;
		using dbj::console::PRN;
		using namespace std;
		// print to the out() overloads
		PRN.wchar_to_console(L"\n");
		out(std_string);
		PRN.wchar_to_console(L"\n");
		out(std_string.c_str());
		// straight to the Printer
		wstring wstr_ = dbj::range_to_wstring(std_string);
		PRN.wchar_to_console(L"\n");
		PRN.wchar_to_console(wstr_.c_str());
		// straight to the IConsole implementation
		PRN.wchar_to_console(L"\n");
		PRN.cons()->out(wstr_.data(), wstr_.data() + wstr_.size());
	};

	if constexpr (dbj::SameTypes<T, char>) {
		actual_tests(string{ init_str_literal });
	}
	else
		if constexpr (dbj::SameTypes<T, wchar_t>) {
			actual_tests(wstring{ init_str_literal });
		}
		else
			if constexpr (dbj::SameTypes<T, char16_t>) {
				actual_tests(u16string{ init_str_literal });
			}
			else
				if constexpr (dbj::SameTypes<T, char32_t>) {
					actual_tests(u32string{ init_str_literal });
				}
				else {
					DBJ_UNHANDLED_("unknown argument type inside string_tester");
				}

};
/// <summary>
/// C++ has inhertied from C the special status of strings
/// which is unfortunate
/// </summary>
void strings_to_console() 
{
	auto narrow = "narrow string" ;
	auto wide	= L"wide string" ;
	auto u16	= u"u16string string" ;
	auto u32	= U"u32string string" ;

	string_tester(narrow);
	string_tester(wide);
	string_tester(u16);
	string_tester(u32);

}

// **********************************************************************************************


template< 
	typename arh_value_type, size_t N, 
	typename arh_type = dbj::arr::ARH<arh_value_type, N>,
	typename ... Args
>
void arh_test ( Args ... args)
{
	using dbj::console::out;
	using dbj::console::PRN;
	using namespace std;

	auto	arg_list = { args ... };
	using	arg_list_type = decltype(arg_list);

	static_assert(
		dbj::SameTypes< arh_type::value_type, arg_list_type::value_type >,
		"dbj::ARH::value_type must be the same to the type of each aggregate init list element"
		);

	PRN.char_to_console("\n");
	out(arh_type::ARR{ { args... } });
	// native arrays out
	PRN.char_to_console("\n");
	out(arh_type::to_arp(arh_type::ARR{ { args... } }));
	PRN.char_to_console("\n");
	out(arh_type::to_arf(arh_type::ARR{ { args... } }));

};

void compound_types_to_console() 
{
	using dbj::console::out;
	using dbj::console::PRN;
	using namespace std;
	// array of fundamental types
	arh_test<int,9>(1, 2, 3, 4, 5, 6, 7, 8, 9);
	// arrays of class types
	arh_test<string, 3>( string{ "ONE" }, string{ "TWO" }, string{ "THREE" });
	// pointers to fundamental types
	arh_test<const wchar_t *, 3>(L"ONE", L"TWO", L"THREE");
	// pointers out -- function pointer 
	PRN.printf("\n");
	out(std::addressof(out<int>));
}
// DBJ_TEST_UNIT("console")
void dbj_test_console()
{
	strings_to_console();
	// fundamental_types_to_console();
	// compound_types_to_console();
}

// DBJ_TEST_SPACE_CLOSE
