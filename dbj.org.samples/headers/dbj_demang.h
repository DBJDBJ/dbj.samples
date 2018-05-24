
/* (c) 2018 by dbj@dbj.org -- licence is at eof
usage:
std::string demangled_name = dbj::demang< int (&) [42] >() ;
Wandbox sample:
https://wandbox.org/permlink/WL3vd6D7segvVA8s
This is both windows and linux code.
*/
#pragma once
#ifndef _WIN32
#include <cxxabi.h>
#endif

namespace dbj {
#ifdef _WIN32
	template < typename T>
	const std::string demang() noexcept
	{
		return { typeid(T).name() };
	}
#else // __linux__
	template < typename T>
	std::string demang()
	{
		// delete malloc'd memory
		struct free_ {
			void operator()(void* p) const { std::free(p); }
		};
		// custom smart pointer for c-style strings allocated with std::malloc
		using ptr_type = std::unique_ptr<char, free_>;

		// special function to de-mangle names
		int error{};
		ptr_type name{ abi::__cxa_demangle(typeid(T).name(), 0, 0, &error) };

		if (!error)        return { name.get() };
		if (error == -1)   return { "memory allocation failed" };
		if (error == -2)   return { "not a valid mangled name" };
		// else if(error == -3)
		return { "bad argument" };
	}
#endif // __linux__
} // dbj
  /*
  Copyright 2017-2018 by dbj@dbj.org

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