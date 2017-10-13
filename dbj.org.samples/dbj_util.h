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

__interface msvc keyword
explained here: https://docs.microsoft.com/en-us/cpp/cpp/interface
*/
#ifndef _MSC_VER
#error This code requires Visual C++ 
#endif // !_MSC_VER
#if _MSC_VER < 1911
#error This code requires Visual C++ 14.1 or better
#endif

#define implements public

#include <dbjio.h>
using dbj::io::print;

#include <memory>
#include <string>
#include <tuple>

namespace dbj {

	namespace util {
		using namespace std;

		// http://en.cppreference.com/w/cpp/experimental/to_array
		namespace {
			template <class T, size_t N, size_t... I>
			/*constexpr*/ inline array<remove_cv_t<T>, N>
				to_array_impl(T(&a)[N], index_sequence<I...>)
			{
				return { { a[I]... } };
			}
		}

		/*
		Transform "C array" into std::array
		*/
		template <class T, std::size_t N>
		constexpr array<remove_cv_t<T>, N> to_array(T(&a)[N])
		{
			return to_array_impl(a, make_index_sequence<N>{});
		}


		namespace xprmntl {

			namespace detail {
				template <class T, class Tuple, size_t... I>
				constexpr T make_from_tuple_impl(Tuple&& t, index_sequence<I...>)
				{
					return T(get<I>(forward<Tuple>(t))...);
				}
			} // namespace detail

			template <class T, class Tuple>
			constexpr T make_from_tuple(Tuple&& t, T * = 0)
			{
				return detail::make_from_tuple_impl<T>(forward<Tuple>(t),
					make_index_sequence<tuple_size_v<decay_t<Tuple>>>{});
			}

		} // namespace 

	
		template<typename F, typename... Targs>
		inline
			void repeater(F callback, Targs... args)
		{
			// since initializer lists guarantee sequencing, this can be used to
			// call a function on each element of a pack, in order:
			char dummy[sizeof...(Targs)] = { (callback(args), 0)... };
		}

#if DBJ_TESTING_EXISTS
		auto funit = []() {
			auto fun = [](auto arg) -> void { cout << (arg);  };
			repeater(fun, "XYZ", true, 1, 0.2f);
				auto tupler = []() { return make_tuple(1, 2, 3); };
				// auto[ a, b, c] = tupler();
			auto whatever = make_from_tuple< decltype(tupler()) >(tupler()) ;
			printex(whatever);
		};

		DBJ_TEST_UNIT(funit);
#endif
	} // util
} // dbj
#define DBJVERSION __DATE__ __TIME__
// #pragma message("-------------------------------------------------------------")
#pragma message( "============> Compiled: " __FILE__ ", Version: " DBJVERSION)
// #pragma message("-------------------------------------------------------------")
#pragma comment( user, "(c) " DBJVERSION " dbj@dbj.org" ) 
#undef DBJVERSION
