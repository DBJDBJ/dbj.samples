#pragma once
// license is at eof
#include "dbj_util.h"
#pragma region enable_if helpers
namespace dbj {
	/*
	templates are zealous eaters of types
	thus overloading and templates do not mix easily
	this iso C++ comitee invenetedstd::enable_if<>
	which in turn makes you either "be somewhere else"
	or scramble for false cover of macros
	here we do not use macros. we use modern C++ to deliver
	helpers when using templates woth or without overloading

	EIF stands for enable if
	DT  stands for Decay Type
	*/
	namespace {
		// we usualy decay the types here before using them
		// see http://en.cppreference.com/w/cpp/types/decay
		template< typename T>
		using DT = std::decay_t<T>; // since C++14

		template< bool pred >
		using EIF = typename std::enable_if_t< pred, int >;
	}
	/* first we define dbj::is_ function for every std::is_ */
	template< typename T>
	static constexpr bool is_object() { return std::is_object_v< DT<T>>; }

	template< typename T>
	static constexpr bool is_integral() { return std::is_integral_v< DT<T>>; }

	template< typename T>
	static constexpr bool is_floating() { return std::is_floating_point< DT<T> >::value; }

	/* bellow are dbj::require_ templates we use through EIF<> */
	template< typename T>
	using require_integral = EIF< std::is_integral_v< DT<T> > >;

	template< typename T>
	using require_floating = EIF< std::is_floating_point< DT<T> >::value >;

	template< typename T>
	using require_object = EIF< std::is_object_v< DT<T> > >;

}
#pragma endregion enable_if helpers


#define DBJVERSION __DATE__ __TIME__
#pragma message( "-----> Compiled: " __FILE__ ", Version: " DBJVERSION)
#pragma comment( user, "(c) 2017 by dbj@dbj.org | Version: " DBJVERSION ) 
#undef DBJVERSION
/*
Copyright 2017 by dbj@dbj.org

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
