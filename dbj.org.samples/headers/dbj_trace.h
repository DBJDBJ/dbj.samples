#pragma once

#include "dbj_util.h"

namespace dbj {
	namespace dbg {

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

inspirators are mentioned in the comments bellow
*/
