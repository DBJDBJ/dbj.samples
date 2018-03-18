#pragma once

#include <string>
#include <algorithm>

/// <summary>
/// The Command Line encapsulation aka TCL-ENC
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

#ifdef DBJ_TESTING_EXISTS
	DBJ_TEST_UNIT(" dbj command line data test ") {

		constexpr 
			const wchar_t * wcliemu[] { 
				L"c:/one/two/three/app.exe", 
				L"/?", 	L"-k ", 
				L"{000-1111-222-333}" 
		};

		constexpr
			const char * cliemu[]{
			"c:/one/two/three/app.exe",
			"/?", "-k ",
			"{000-1111-222-333}"
		};

		constexpr 
			const size_t argc = _countof(wcliemu);

		auto cliw = make_command_line_data<argc>(wcliemu);
		auto cliv = make_command_line_data<argc>(cliemu);
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
