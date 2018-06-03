#pragma once
/*
DBJ++ 2018
Stuff in here *might* be needed for finding the light
in the dark underworld of compile time
*/
namespace dbj::compile_time {
	/*
	what is this good for?
	*/
	template <typename T> struct identificator { typedef  T type ; };
	template <typename T> using identity = typename identificator<T>::type;

	template<typename T, std::size_t N, 
		typename outype = std::size_t const (&)[N]	>
	inline constexpr outype && indexer(T(&incoming)[N])
	{
		std::size_t indexes[N]{ };
		for (size_t j = 0; j != N; ++j) indexes[j] = j;
		return std::move(indexes);
	}

	// compile time string handling
	template <std::size_t N>
	inline constexpr 	char	at(char const (&s)[N], std::size_t i) {
			return i >= N ? '\0' : s[i];
	}


	// recursive compile time c string len
	inline constexpr std::size_t c_strlen(char const* str, std::size_t count = 0)
	{
		return ('\0' == str[0]) ? count : c_strlen(str + 1, count + 1);
	}

}// dbj::compile_time

#ifdef DBJ_TESTING_ONAIR

#ifndef DBJ_TEST_UNIT
#define DBJ_TEST_UNIT(x) DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, x))
#endif

namespace {
	using namespace dbj::compile_time;
	/* */
	DBJ_TEST_UNIT(": dbj::compile_time")
	{
		// just the type no instances whatsoever
		using v_of_x = std::vector<identity<struct X>>;
		using v_of_x = std::vector<struct X>;

		struct X {};
		constexpr X decisions[]{ X{}, X{}, X{}, X{} };
		decltype(auto) indices = indexer(decisions);

		const char name_[]{ "Anbra" };
	//	constexpr auto c_ = at(name_, 0);
	}
	/* */
}
#endif

/*
Copyright 2018 by dbj@dbj.org

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
