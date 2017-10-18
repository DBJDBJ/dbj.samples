#pragma once

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
// #pragma message("-------------------------------------------------------------")
#pragma message( "============> Compiled: " __FILE__ ", Version: " DBJVERSION)
// #pragma message("-------------------------------------------------------------")
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
