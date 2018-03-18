#pragma once

#include <cstdlib>
#include <iostream>
#include <vector>

#if 0
// in case dbj++ is not present
namespace dbj {
	namespace {
		auto out_ = [](auto val_) {
			std::cout << val_;
		};
	}
	auto print = [](auto val_ , auto... param)
	{
		out_(val_);
		if constexpr (sizeof...(param) > 0) {
			char dummy[sizeof...(param)] = {
				(out_(/* std::forward<decltype(param)> */ (param)), 0)...
			};
		}
		return dbj::print;
	};
}
#endif

namespace dbj {

	template<typename T, typename = std::enable_if_t< std::is_pointer<T>::value> >
	constexpr inline bool 
		is_null(const T tp = 0)
	{
		return (tp != nullptr);
	}


#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	wchar_t **		wargv_	= std::as_const(__wargv	);
	char    **		argv_	= std::as_const(__argv	);
	unsigned int	argc_	= std::as_const(__argc	);

#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	namespace {
		
		using wvecT = std::vector<std::wstring>;
		using nvecT = std::vector<std::string >;

		// wargv_ !=  nullptr
		auto decide(std::true_type tt) {
			return wvecT{ wargv_, wargv_ + argc_ };
		}
		// wargv_ ==  nullptr
		auto decide( bool ft) {
			return nvecT{ argv_, argv_ + argc_ };
		}
	}

	auto cli_data = []() {
		
			auto cli_vector = decide( 
				wargv_  
				? std::true_type{}
				: false
			);
			return cli_vector;
	};

}

#if 0

/*
https://stackoverflow.com/questions/47452748/how-to-decide-on-auto-return-type-at-run-time/47457592#47457592
*/
auto command_line_data = [&]() {
	if constexpr (wargv_pointer) {
		return std::vector<std::wstring>(wargv_, wargv_ + argc_);
	}
	else {
		return std::vector<std::string>(argv_, argv_ + argc_);
	}
};

// http://en.cppreference.com/w/cpp/language/auto
auto does_not_compile_msvc = [](bool wide)
{
	if (wide)
		return std::vector<std::string>();

	return std::vector<std::wstring>();
};

auto msvc_does_not_compile_too = [](auto _string)
{
	using string_type = decltype(_string);

	if constexpr (std::is_same<string_type, std::string>::value)
		return std::vector<std::string>;
	else {
		return std::vector<std::wstring>;
	}
};
/*
string and wstring have no common type 
so '::type' will be undefined here

using string_type = std::common_type<std::wstring , std::string>::type;
*/

#endif