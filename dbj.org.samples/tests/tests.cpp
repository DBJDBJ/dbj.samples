// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// testing the various stuff in this project
#include "../stdafx.h"
#include "../headers/dbj_lambda_lists.h"
#include "../headers/policy_classes.h"
#include "../headers/no_inheritance.h"
#include "../headers/dbj_tokenizer.h"
#include "../headers/dbj_atoms.h"
#include "../headers/dbj_lambda_lists.h"
#include "../dbj_any/dbj_any_node.h"
#include "../dbjtree\dbj_tree_tests.h"
#include "../dbj_x/dbj_timer.h"

DBJ_TEST_SPACE_OPEN(local_tests)

template< typename T>
void array_analyzer (const T & specimen) {

	static char const * name{ dbj::tt::name_<T>() }; //safe?
	constexpr bool is_array = std::is_array_v<T>;
	if constexpr (is_array == true)
	{
		constexpr size_t number_of_dimension = std::rank_v<T>;
		constexpr size_t first_extent = std::extent_v<T>;
		std::wprintf(L"\n%S is %s", name, L"Array" );
		std::wprintf(L"\n%-20S number of dimension is %zu", name, number_of_dimension);
		std::wprintf(L"\n%-20S size along the first dimension is %zu", name, first_extent);
	}
	else {
		std::wprintf(L"\n%S is %s", name, L"Not an Array" );
	}
};

#define DBJ_IS_ARR(x) try_array( std::addressof(x) )

template<typename T>
constexpr auto try_array( T * specimen )  -> size_t {
	return  std::extent_v< T >  ;
}

template<typename T>
constexpr auto probe_array(T specimen)   -> size_t {
	return  std::extent_v< T >;
}

DBJ_TEST_UNIT(_array_stays_array)
{
	static int ia[]{ 1,2,3,4,5,6,7,8,9,0 };
	auto s0 = try_array(ia);
	auto s1 = DBJ_IS_ARR( ia );
	auto s2 = probe_array(ia);
	auto s3 = probe_array(std::addressof(ia));
}

DBJ_TEST_UNIT(_GetGeoInfoEx_)
{
	auto DBJ_UNUSED(us_data) = dbj::win32::geo_info(L"US");
	auto DBJ_UNUSED(rs_data) = dbj::win32::geo_info(L"RS");

	dbj::console::print(us_data);
	dbj::console::print(rs_data);
}
/*
auto reporter = [&](const char * prompt = "", const void * this_ptr = nullptr ) {
	char address_str[128]{0};
	int retval = std::snprintf(address_str, 128, "%p", this_ptr);
	_ASSERTE( retval > 0);
	dbj::console::print("\n[", address_str, "]\t", prompt );
};
*/


typedef enum class CODE : UINT {
	page_1252 = 1252u,   // western european windows
	page_65001 = 65001u // utf8
	// page_1200 = 1200,  // utf16?
	// page_1201 = 1201   // utf16 big endian?
} CODE_PAGE;

	DBJ_TEST_UNIT(_famous_dbj_console_ucrt_crash)
	{
		// кошка 日本
		constexpr wchar_t specimen[] =
		{ L"\x043a\x043e\x0448\x043a\x0430 \x65e5\x672c\x56fd" };

		dbj::console::print("\n", specimen, "\n");

		// 1252u or 65001u
		if (::IsValidCodePage(65001u)) {
			auto scocp_rezult = ::SetConsoleOutputCP(65001u);
			_ASSERTE(scocp_rezult != 0);
		}
		/*
		<fcntl.h>
		_O_U16TEXT, _O_U8TEXT, or _O_WTEXT
		to enable Unicode mode
		_O_TEXT to "translated mode" aka ANSI
		_O_BINARY sets binary (untranslated) mode,
		*/
		int result = _setmode(_fileno(stdout), _O_U8TEXT);
		_ASSERTE(result != -1);

		// should display: кошка 日本
		// for any mode the second word is not displayed ?
		auto fwp_rezult = fwprintf( stdout, L"\nwfprintf() displays: %s\n", specimen);
		// for any mode the following crashes the UCRT (aka Universal CRT)
		// fprintf( stdout, "\nprintf() result: %S\n",specimen);
	}


	DBJ_TEST_UNIT(_inheritance_) {

		using dbj::console::print;
		constexpr static  dbj::c_line<80, '-'> Line80; // compile time

		auto measure = [&](auto object, const char * msg = "") -> void {
			using dbj::console::print;
			print("\n", Line80,
				"\n", msg, "\nType name:\t", typeid(object).name(),
				"\nSpace requirements in bytes",
				"\nfor Type:\t\t", sizeof(decltype(object)),
				"\nfor Instance:\t", sizeof(object),
				"\nfor Allocation:\t", alignof(decltype(object))
			);
		};

		dbj::samples::philology::HelloWorld<> hello{};
		dbj::samples::philology::HelloWorld2<> hello2{};

		print("\n", Line80);
		print("\nBEFORE RUN\n");
		measure(hello);
		measure(hello2);
		print("\n", Line80);
		hello.run("\nHelloWorld -- Default policies");
		hello2.run("\nHelloWorld2 -- No inheritance");
		print("\n", Line80);
		print("\nAFTER RUN\n");
		measure(hello);
		measure(hello2);
		print("\n", Line80);
	};

	DBJ_TEST_UNIT( _documents_ ) {

		using IOperation = dbj::samples::docops::IOperation;
		/* OPTIONAL: configure the docops to use online operations
		*/
		auto ot = dbj::samples::docops::operations_type(IOperation::type::online);
		dbj::samples::documents::TextDoc text;
		dbj::samples::documents::opendoc(text, "world oyster");
	}

DBJ_TEST_SPACE_CLOSE
