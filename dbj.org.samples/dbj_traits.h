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
Referee<T> requires T to be an object type or a function type.
*/
template<typename T, class Enable = void >
struct Referee {};

template<typename  T >	Referee<T> make(const T &);

template<typename T>
struct Referee<T, typename std::enable_if<std::is_class<T>::value>::type >
{
	using self_type = Referee;
	using reftype = std::reference_wrapper<T>;
	using T_type = T;
	const T dflt{};
	reftype reference = std::ref(dflt);

	Referee(const T & o) : reference(std::ref(o)) {}
protected:
	// friend Referee make(const T & o = T{}) { return Referee(o); }
	Referee() {}
};


struct lama final {	const char * name = "Alpaka" ;};

DBJ_TEST_CASE("Testing dbj traits" ) {
	using lamaref = Referee<lama>;
		lamaref lref(lama());
		dbj::io::printex("\n\n", __func__, "\nReferee<bool> typeid:\t\t", typeid(lamaref).name());
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
