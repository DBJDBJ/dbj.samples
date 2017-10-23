#pragma once
// license at eof
/* std::enable_if<> tamed with template aliases
   insipred with https://codereview.stackexchange.com/questions/71946/use-of-macros-to-aid-visual-parsing-of-sfinae-template-metaprogramming
   NOTE: C++17 onwards is required
*/

#include <type_traits>
#include <dbjio.h>
#include <dbj_testing.h>

namespace dbj {


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

namespace util {	

		template<typename F, typename... Targs>
		inline
		void applicator(F callback, Targs... args)
		{
			// since initializer lists guarantee sequencing, this can be used to
			// call a function on each element of a pack, in order:
			char dummy[sizeof...(Targs)] = { (callback(args), 0)... };
		}

	} // util
} // dbj

  /* standard suffix for every other header here */
#pragma comment( user, __FILE__ "(c) 2017 by dbj@dbj.org | Version: " __DATE__ __TIME__ ) 

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
