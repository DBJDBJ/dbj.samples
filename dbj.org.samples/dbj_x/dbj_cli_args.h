#pragma once

#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <map>

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
	/// we develop only unicode windows app's
	/// this dats type we use everyhwere
	/// to provide CLI interface implementation
	/// </summary>
	using data_type = std::vector< std::wstring >;

	/// <summary>
	/// transform argv or argw to data_type
	/// base your cli proc code on data_type
	/// instead of raw pointers 
	/// data_type is standard c++ range
	/// </summary>
	/// <param name="args">__argv or __argw</param>
	/// <param name="ARGC">__argc</param>
	/// <returns>
	/// instance of data_type
	/// </returns>
	inline auto command_line_data (size_t ARGC, wchar_t **  args )
		 -> data_type
	{
		_ASSERTE(*args != nullptr);
			return data_type{ args, args + ARGC } ;
	};

	struct app_env_struct final {

		std::size_t cli_args_count{};
		data_type cli_data{};
		std::size_t env_vars_count{};

		using env_kv_data_type = std::map<std::wstring,std::wstring>;
		env_kv_data_type env_vars{};
	};

	inline auto app_env_initor = []() {

		wchar_t **  warg = (__wargv);
		std::size_t argc = (__argc);
		wchar_t **  wenv = (_wenviron);

		/// <summary>
		///  we are here *before* main so __argv or __argw might be still empty
		///  TODO: this is perhaps naive implementation?
		/// </summary>
		while ( *warg == nullptr) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		_ASSERTE( *warg != nullptr);

		while (*wenv == nullptr) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		_ASSERTE(*wenv != nullptr);

		data_type
			warg_data{ command_line_data(argc, warg) };

		// calculate the count of env vars first
		auto count_to_null = [](auto ** list_) constexpr->size_t {
			size_t rez{ 0 };
			_ASSERTE(*list_ != nullptr);
			for (; list_[rez] != NULL; ++rez) {};
			return rez;
		};

		size_t evc = count_to_null(wenv);

		data_type
			wenvp_data{ command_line_data( evc, wenv) };
		app_env_struct::env_kv_data_type
			wenvp_map{};

		//transform env vars to k/v map
		// each envar entry is 
		// L"key=value" format
		for (auto kv : wenvp_data) {
			auto delimpos = kv.find(L"=");
			auto key = kv.substr(0, delimpos);
			auto val = kv.substr(delimpos + 1);
			wenvp_map[key] = val;
		}

		return app_env_struct { argc, warg_data , evc, wenvp_map };
	};

	inline static app_env_struct APP_ENV_STRUCT = app_env_initor();

#ifdef DBJ_TESTING_EXISTS
	DBJ_TEST_UNIT(" dbj command line data test ") {
		print(
			APP_ENV_STRUCT.cli_data
		);
		print("\nEnv vars found");
		int counter = 0;
		std::wstring key, val;
		for (const auto & kv : APP_ENV_STRUCT.env_vars)
		{
			key = kv.first.data();
			val = kv.second.data();
			print("\n[", counter++ , "] Key: ", key, "\t = \tValue: ", val);
		}
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
