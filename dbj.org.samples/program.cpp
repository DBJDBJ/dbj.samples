#include "stdafx.h"
#if 0
/*
  VS2017 Community edition (latest as of 2017-10-20), C++17 build
  Global space is poluted with 6 variables form corecrt_math.h

C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\ucrt\corecrt_math.h 
From line 980
_CRT_NONSTDC_DEPRECATE(_j0) _Check_return_ _ACRTIMP double __cdecl j0(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_j1) _Check_return_ _ACRTIMP double __cdecl j1(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_jn) _Check_return_ _ACRTIMP double __cdecl jn(_In_ int _X, _In_ double _Y);
_CRT_NONSTDC_DEPRECATE(_y0) _Check_return_ _ACRTIMP double __cdecl y0(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_y1) _Check_return_ _ACRTIMP double __cdecl y1(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_yn) _Check_return_ _ACRTIMP double __cdecl yn(_In_ int _X, _In_ double _Y);
*/
/* 
   if there is no namespace arround users declarations/definitions:

   error C2365: 'j0': redefinition; previous definition was 'function'
   ...
   error C2365: 'yn': redefinition; previous definition was 'function'
 */
namespace {
	constexpr auto j0 = true;
	constexpr auto j1 = true;
	constexpr auto jn = true;
	constexpr auto y0 = true;
	constexpr auto y1 = true;
	constexpr auto yn = true;
}

int main(int argc, char* argv[])
{
	/*
	if there is namespace arround users declarations/definitions above:

	error C2872: 'j0': ambiguous symbol
	...
	error C2872: 'yn': ambiguous symbol
	*/
		std::cout << j0 << j1 << jn << y0 << y1 << yn ;
	return true;
}
#endif

namespace dbj {

#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	auto	wargv_ = (__wargv);
	auto	argv_  = (__argv );
	auto	argc_  = (__argc );

#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	constexpr auto wargv_pointer = std::addressof(wargv_);

	class cli_type final {
		cli_type() = delete;
		cli_type(const cli_type &) = delete;
		cli_type & operator = (const cli_type &) = delete;
	public:
		using wvec_type = std::vector<std::wstring>;
		using nvec_type = std::vector<std::string >;
		wvec_type wvec{};
		nvec_type nvec{};

		static cli_type & object ( ) {
			static cli_type data_{};
			return data_;
		};

		/*
		decide at compile time what the return type is going 
		to be at runtime ... but how?
		 even with if constexpr bellow all the returns have to return same type
		 thus: void make()
		*/
		constexpr static void make() {
			if constexpr (wargv_pointer) {
				wchar_t ** first =  wargv_ ;
				wchar_t ** last = first + argc_;
				cli_type::object().wvec = wvec_type(first, last);
			}
			else {
				char ** first =  argv_ ;
				char ** last = first + argc_;
				cli_type::object().nvec = nvec_type(first, last);
			}
		}

	} ;
}
int main(int argc, char* argv[])
{
	using namespace dbj::win;
	con::switch_console( con::CODE::page_1252 );
	con::setfont(L"Lucida Console");
	// con::setfont(L"Arial");

	dbj::print("\ndbj")("\tprint")("\tis")("\tfluent\n");

	dbj::cli_type::make();

		//dbj::print( "\ndbj::cli.wvec", (dbj::cli.wvec) );
		//dbj::print( "\ndbj::cli.nvec", (dbj::cli.nvec) );

	dbj::testing::execute();
	return true;
}
