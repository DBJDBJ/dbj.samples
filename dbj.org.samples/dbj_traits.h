#pragma once
// license is at eof
#include "dbj_util.h"
#pragma region enable_if helpers
namespace dbj {
	/*
	templates are zealous eaters of types
	thus overloading and templates do not mix easily
	this iso C++ comiteer invenetedstd::enable_if<>
	which in turn makes you either "be somewhere else"
	or scramle for false cover of macros
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
	using require_integral = EIF< std::is_integral_v< DT<T>> >;

	template< typename T>
	using require_floating = EIF< std::is_floating_point< DT<T> >::value >;

	template< typename T>
	using require_object = EIF< std::is_object_v< DT<T>> >;

}
#pragma endregion enable_if helpers
namespace dbj {
	namespace traits {

		template<class... T> struct type_list { std::tuple<T...> args = std::make_tuple<T...>(); };

		template<typename T> struct tname { const char * name = typeid(T).name; };

/* usage:
    using mp_trait = MemberPointer_traits<decltype(&T::begin)>
    mp_trait::type if exists
	is the type of the pointer to the method T::begin
*/
template<typename>
struct MemberPointer_traits {};

template<class T, class U>
struct MemberPointer_traits<U T::*> {
	using member_type = U;
};
/*
Referee<T> requires T to be of an class type
*/
template<typename T, typename std::enable_if<std::is_class<T>::value>::type * = 0 >
struct Referee
{
	const T  & value_default{};
	using self_type = Referee;
	using value_reftype = std::reference_wrapper< std::remove_cv_t<T> >;
	using value_type = T;
	value_reftype reference = std::cref(value_default);
};


struct lama final {	const char * name = "Alpaka" ;};

//#define DBJ_NV( symbol) "[", STR(symbol), "] :-> [" , symbol , "]"

DBJ_TEST_CASE("Testing dbj traits" ) {
	using lamaref = Referee<lama>;
		dbj::io::printex("\n", __func__, "\t", DBJ_NV(typeid(lamaref).name()));
	}

} // traits
} // dbj

#define DBJVERSION __DATE__ __TIME__
#pragma message( "--------------------> Compiled: " __FILE__ ", Version: " DBJVERSION)
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
