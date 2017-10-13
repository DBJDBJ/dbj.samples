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

inspirators are mentioned in the comments bellow
*/

#include "stdafx.h"

namespace dbj {
	namespace dbg {

		/*
		Tame the full path filenames  in __FILE__
		https://stackoverflow.com/questions/8487986/file-macro-shows-full-path/8488201#8488201
		#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
		*/
		DBJ_INLINE constexpr auto nicer_filename(const char * filename) {
			return (strrchr(filename, '\\') ? strrchr(filename, '\\') + 1 : filename);
		}

		template <typename T>
		inline
			constexpr
			auto sizeof_array(const T& iarray) {
			return (sizeof(iarray) / sizeof(iarray[0]));
		}

#ifdef BUFSIZ
		constexpr static int TRACEMAXSTRING = BUFSIZ * 2;
#else
		constexpr static int TRACEMAXSTRING = 1024 * 2;
#endif

		// https://stackoverflow.com/questions/451749/is-there-a-trace-statement-for-basic-win32-c
		inline void trace(const char * filename, const unsigned int line, const char* const format, ...)
		{
			std::vector<char> buffer(TRACEMAXSTRING, '\x0');
			auto nicer_name = nicer_filename(filename);
			DBJ_ASSERT(nicer_name);
			_snprintf_s(&buffer[0], buffer.size(), TRACEMAXSTRING - 1, "%s(%d): ", nicer_name, line);

			_RPT0(_CRT_WARN, buffer.data());

			buffer.resize(TRACEMAXSTRING);
			va_list args;
			va_start(args, format);
			int nBuf;
			nBuf = _vsnprintf_s(
				&buffer[0],
				buffer.size(),
				TRACEMAXSTRING - 1,
				format,
				args);
			va_end(args);

			_RPT0(_CRT_WARN, buffer.data());
		}
	}
}
#define DBJ_TRACE( format, ...)  dbj::dbg::trace( __FILE__, __LINE__, format, __VA_ARGS__ )

// #define DBJ_TRACE((void)0)

#define DBJVERSION __DATE__ __TIME__
// #pragma message("-------------------------------------------------------------")
#pragma message( "============> Compiled: " __FILE__ ", Version: " DBJVERSION)
// #pragma message("-------------------------------------------------------------")
#pragma comment( user, "(c) 2017 by dbj@dbj.org | Version: " DBJVERSION ) 
#undef DBJVERSION
