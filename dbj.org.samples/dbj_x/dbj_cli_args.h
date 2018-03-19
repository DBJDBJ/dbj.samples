#pragma once

#include <string>
#include <algorithm>
#include <dbj_traits.h>

/// <summary>
/// The Command Line encapsulation aka TCL-ENC
/// by dbj.org
/// (c) statement is on the bottom of this file
/// <remarks>
/// Perhaps interesting in this design is the decision 
/// not to succumb to C like cli 
/// which is based on char ** (or wchar_t ** in case of Windows). 
/// Internaly to represent the command line we will  use 
/// <code>
///  std::array< std::string, size_t > ;
/// </code>
/// when dealing with the actual cli we will transform asap to  this
/// thus internaly we do not have to deal with raw pointers.
/// </remarks>
/// </summary>
namespace dbj::cli {

	template<size_t N>
	using command_line_data_type = std::array< std::string, N >;

	/// <summary>
	/// make cli data from "any" type
	/// while in reality we know the only two types
	/// can be char and wchar_t
	/// </summary>
	template<size_t ARGC, typename T>
	inline auto make_command_line_data(const T * const argw[]);

	/// <summary>
	/// make command_line_data_type from wchar_t * argw[argc]
	/// </summary>
	template<size_t ARGC>
	inline auto make_command_line_data(const wchar_t * const argw[])
		-> command_line_data_type<ARGC>
	{
		command_line_data_type<ARGC>  rezult{};
		size_t j = 0;
		std::for_each(argw, argw + ARGC,
			[&]( const wchar_t * arg) {
			const std::wstring trans{ arg };
			rezult[j++] = std::string(trans.begin(), trans.end());
		}
		);
		return rezult;
	}

	/// <summary>
	/// make command_line_data_type from char * argv[argc]
	/// </summary>
	template<size_t ARGC>
	inline auto make_command_line_data(const char * const argv[])
		-> command_line_data_type<ARGC>
	{
		command_line_data_type<ARGC>  rezult{};
		size_t j = 0;
		std::for_each(argv, argv + ARGC,
			[&](const char * arg) {
				rezult[j++] = std::string(arg);
		}
		);
		return rezult;
	}

	/// <summary>
	/// reveal the actual type of T
	/// </summary>
	template< typename T> struct actual_type {
		typedef typename std::decay_t< T > value_type  ;
		 enum { is_pointer_ = false, single_pointer = false, double_pointer = false  };
		//static constexpr bool pointer{ std::is_pointer<T>::value } ;
	};
	template< typename T> struct actual_type<T*> {
		typedef typename std::decay_t< T > value_type;
		enum { is_pointer_ = true, single_pointer = true, double_pointer = false };
		//static constexpr bool pointer{ std::is_pointer<T>::value };
	};
	template< typename T> struct actual_type<T**> {
		typedef typename std::decay_t< T > value_type;
		enum { is_pointer_ = true, single_pointer = false, double_pointer = true };
		//static constexpr bool pointer{ std::is_pointer<T>::value };
	};

	inline auto show_actual_type = [] ( auto actual) {
		using at = decltype(actual);
		using actual_value_type = at::value_type;
		dbj::print("\nactual type of argument:", typeid(actual_value_type).name(),
			"\nis actual type a pointer:", (at::is_pointer_ ? "true" : "false"),
			"\nis actual type a single pointer:", (at::single_pointer ? "true" : "false"),
			"\nis actual type a bouble pointer:", (at::double_pointer ? "true" : "false")
		);
	};

	/// <summary>
	/// transform argv or argw to command_line_data_type
	/// if on windows best to use like so:
	/// <code>
	/// auto cli = make_command_line_data<argc>(__targv);
	/// </code>
	/// base your cli proc code on command_line_data_type
	/// instead of raw pointers 
	/// command_line_data_type is standard c++ range
	/// </summary>
	/// <param name="args">__argv or __argw</param>
	/// <param name="ARGC">__argc</param>
	/// <returns>
	/// instance of command_line_data_type<argc>
	/// </returns>
	template<size_t ARGC, typename T>
	inline auto command_line_data ( const T (&args)[ARGC] )
		 -> command_line_data_type<ARGC>
	{
		using namespace std;

		// T is actually a pointer here
		// we need a type of the value it points to
		using args_type = typename actual_type< T >::value_type;

#ifdef _DEBUG
		show_actual_type(actual_type< T >{});
#endif

		command_line_data_type<ARGC> rezult{};
		size_t j = 0;

		if  constexpr ( is_same_v<args_type, char> ) {
			for_each(args, args + ARGC,
				[&](const char * arg) {
				rezult[j++] = string(arg);
			}
			);
			return rezult;
		}
		
		if  constexpr (is_same_v<args_type, wchar_t>) {
			for_each(args, args + ARGC,
				[&](const wchar_t * arg) {
				const wstring wide{ arg };
				rezult[j++] = string(wide.begin(), wide.end());
			}
			);
			return rezult;
		}
#pragma message ( __FILE__ " :: WARNING! Type is not found to be char or wchar_t at compile time")
			throw runtime_error("only char and wchar_t may be types of cli arguments");
		return rezult;
	};

#ifdef DBJ_TESTING_EXISTS
	DBJ_TEST_UNIT(" dbj command line data test ") {

constexpr 
	const wchar_t * wcli_emulated[] { 
		L"c:/one/two/three/app.exe", L"/?", L"-k ", L"{000-1111-222-333}" 
};

constexpr
	const char * cli_emulated[]{
	"c:/one/two/three/app.exe",	"/?", "-k ","{000-1111-222-333}"
};

constexpr 
	const size_t argc = _countof(cli_emulated);

	auto cliw = make_command_line_data<argc>(wcli_emulated);
	auto cliv = make_command_line_data<argc>(cli_emulated);

	// auto clit = make_command_line_data<argc>(__targv);
		

	auto cli = command_line_data(wcli_emulated);

	}
#endif 
} // dbj::cli

/* standard suffix for every dbj.org header */
#pragma comment( user, __FILE__ "(c) " __DATE__ " by dbj@dbj.org | Version: " __DATE__ __TIME__ )
/*
Copyright 2017,2018 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
