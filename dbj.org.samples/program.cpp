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
   if the is no namespace arround uss declarations/definitions:

   or C2365: 'j0': redefinition; previous definition was 'function'
   ...
   or C2365: 'yn': redefinition; previous definition was 'function'
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
	if the is namespace arround uss declarations/definitions above:

	or C2872: 'j0': ambiguous symbol
	...
	or C2872: 'yn': ambiguous symbol
	*/
		std::cout << j0 << j1 << jn << y0 << y1 << yn ;
	return true;
}
#endif

extern "C" void quick_local_tests(decltype(dbj::print) & print);

int main(int argc, char* argv[])
{
	quick_local_tests(dbj::print);
	dbj::testing::execute();
	return 1;
}


