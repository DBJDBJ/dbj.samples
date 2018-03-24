#pragma once

#include <string>
#include <algorithm>
#include <thread>
#include <chrono>

#include <dbj_traits.h>

#include "dbj_actual_type.h"


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

	/// <summary>
	/// this dats type we use everyhwere
	/// to provide CLI interface implementation
	/// </summary>
	using command_line_data_type = std::vector< std::wstring_view >;

	/// <summary>
	/// transform argv or argw to command_line_data_type
	/// base your cli proc code on command_line_data_type
	/// instead of raw pointers 
	/// command_line_data_type is standard c++ range
	/// </summary>
	/// <param name="args">__argv or __argw</param>
	/// <param name="ARGC">__argc</param>
	/// <returns>
	/// instance of command_line_data_type
	/// </returns>
	inline auto command_line_data = [] (size_t ARGC, auto  args )
		 -> command_line_data_type
	{
		using namespace std;
		using argument_type = decltype(args);
		using args_type = typename actual_type< argument_type >::value_type;

		static_assert(
			actual_type< argument_type >::double_pointer,
			" dbj command_line_data() argument must be a double pointer"
			);

		if (args == nullptr) {
			DBJ::TRACE(" dbj command_line_data() argument found to be null ptr?");
			return {}; // empty rezult
		}

#ifdef _DEBUG
		dbj::show_actual_type< argument_type >();
#endif

		if  constexpr ( is_same_v<args_type, wchar_t> ) 
		{
			return command_line_data_type{ args, args + ARGC } ;
		}
		
		// have to make vector<wstring_view>
		if  constexpr (is_same_v<args_type, char>) 
		{
			command_line_data_type rezult{};
			vector< string > temporum{ args, args + ARGC };
			for (auto narrow : temporum ) {
				rezult.push_back(wstring{ narrow.begin(), narrow.end() });
			  }
			return rezult;
		}

		DBJ::TRACE("only char and wchar_t may be types of cli arguments");
		return {}; // empty rezult
	};

#ifdef DBJ_TESTING_EXISTS
	DBJ_TEST_UNIT(" dbj command line data test ") {

		char ** varg = (__argv);
		wchar_t **  warg = (__wargv);
		std::size_t argc = (__argc);

	/// <summary>
	///  we are here *before* main so __argv or __argw might be still empty
	///  TODO: this is perhaps naive implementation?
	/// </summary>
	while ((varg == nullptr) && (warg == nullptr)) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	command_line_data_type 
		varg_data{ command_line_data(argc, varg) } , 
		warg_data{ command_line_data(argc, warg) };

	DBJ_ASSERT(
		(varg_data.size() > 0) || (warg_data.size() > 0)
	);

	// here we return and use the one of the two 
	// which is not empty

	// now we repeat the same logic 
	// for the environment string table aka envp
	wchar_t **  wenv = (_wenviron);
	char ** venv = (_environ);
	//  but we need to 
	// calculate the count of env vars first

	auto count_to_null = []( auto ** list_ ) constexpr -> size_t {
		size_t rez{0};
		if (list_ == nullptr) return {}; // empty
		for (; list_[rez] != NULL; ++rez) {};
		return rez;
	};

	size_t envp_count{};
	if (wenv) {	envp_count = count_to_null(wenv) ;}
	if (venv) { envp_count = count_to_null(venv) ;}

	command_line_data_type
		envp_data{ command_line_data(envp_count, venv) },
		wenvp_data{ command_line_data(envp_count, wenv) };

	DBJ_ASSERT(
		(envp_data.size() > 0) || (wenvp_data.size() > 0)
	);

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
