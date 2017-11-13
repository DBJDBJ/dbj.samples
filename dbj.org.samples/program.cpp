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
      //www.highprogrammer.com/alan/windev/visualstudio.html
	  // Statements like:
	  // #pragma message(Reminder "Fix this problem!")
	  // Which will cause messages like:
	  // C:\Source\Project\main.cpp(47): Reminder: Fix this problem!
	  // to show up during compiles. Note that you can NOT use the
	  // words "error" or "warning" in your reminders, since it will
	  // make the IDE think it should abort execution. You can double
	  // click on these messages and jump to the line in question.

#define Stringize( L )     #L 
#define MakeString( M, L ) M(L)
#define Expander(x) (x)
#define $Line MakeString( Stringize, __LINE__ )
#define $Year MakeString( Stringize, ((__DATE__+7)) )
#define Reminder __FILE__ "(" $Line ") :DBJ Reminder: "

int main(int argc, char* argv[])
{
	#pragma message(Reminder "Use this macro!") 
	dbj::testing::execute();
	return true;
}

/*
try this

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define LOCATION __FILE__ "(" TOSTRING(__LINE__) ") : "

from: https://stackoverflow.com/questions/30438911/how-to-get-a-visual-studio-macro-value-into-a-pre-processor-directive
*/