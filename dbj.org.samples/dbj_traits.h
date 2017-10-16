#pragma once
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

namespace dbj {
	namespace traits {

		template<class... T> struct type_list { std::tuple<T...> args = std::make_tuple<T...>(); };

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

template<typename T>
inline std::reference_wrapper<T> Referee()
{
	static T value = {};
	return std::ref(value);
}

		auto dummy = dbj::testing::add(
				"Testing dbj traits" , []() -> void {
				class X {};
				auto THX = Referee<X>(); // thx();

				dbj::io::printex("\n\n", __func__,
					"\nReferee<X> typeid:\t\t", typeid(THX).name()
				);
			}
		);
	} // traits
} // dbj

#define DBJVERSION __DATE__ __TIME__
// #pragma message("-------------------------------------------------------------")
#pragma message( "============> Compiled: " __FILE__ ", Version: " DBJVERSION)
// #pragma message("-------------------------------------------------------------")
#pragma comment( user, "(c) 2017 by dbj@dbj.org | Version: " DBJVERSION ) 
#undef DBJVERSION
