#pragma once

#ifndef UNICODE
#error __FILE__ requires UNICODE builds
#endif

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// #define STRICT
#include <windows.h>
// #include <tchar.h>

namespace dbj {
	namespace win32 {
#pragma region "dbj win32 strings"
		using CHAR_T = wchar_t;
		using STRING = std::basic_string<CHAR_T>;
		using long_string_pointer = LPWSTR;

		/* Find the length of S, but scan at most MAXLEN characters.  If no '\0'
		terminator is found within the first MAXLEN characters, return MAXLEN.
		*/
#ifndef __cplusplus
		/*
		Inspired by: https://opensource.apple.com/source/bash/bash-80/bash/lib/sh/strnlen.c
		*/
		DBJ_INLINE size_t strnlen(const CHAR_T *s, size_t maxlen)
		{
			const CHAR_T *e = {};
			size_t n = {};

			for (e = s, n = 0; *e && n < maxlen; e++, n++)
				;
			return n;
		}
#else
		/* modern C++ version*/
		template< size_t N>
		DBJ_INLINE size_t strnlen(const CHAR_T(&s)[N], size_t maxlen)
		{
			return min(N, maxlen);
		}
#endif

		template< size_t N>
		inline std::basic_string<wchar_t> widener( const char (& charar ) [N])
		{
			return std::basic_string<wchar_t>(std::begin(charar), std::end(charar));
		}
#pragma endregion "dbj win32 strings"

			//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
			DBJ_INLINE auto getLastErrorMessage(
				const STRING & prompt = STRING{}, DWORD errorMessageID = ::GetLastError()
			)
			{
				//Get the error message, if any.
				if (errorMessageID == 0)
					return STRING(); //No error message has been recorded

				long_string_pointer messageBuffer = nullptr;

				size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (long_string_pointer)&messageBuffer, 0, NULL);

				STRING message(messageBuffer, size);

				//Free the buffer.
				::LocalFree(messageBuffer);

				if (! prompt.empty() )
					return STRING(prompt).append(message);

				return message;
			}

			template <size_t N>
			DBJ_INLINE auto getLastErrorMessage(
				const char (& prompt)[N] , DWORD errorMessageID = ::GetLastError()
			)
			{
				return getLastErrorMessage(widener(prompt), errorMessageID);
			}

			namespace sysinfo {

				constexpr auto INFO_BUFFER_SIZE = 32767;
				static CHAR_T  infoBuf[INFO_BUFFER_SIZE] = {};
				static DWORD  bufCharCount = INFO_BUFFER_SIZE;
				// 
				DBJ_INLINE auto computer_name() {
					bufCharCount = INFO_BUFFER_SIZE;
					if (!GetComputerName(infoBuf, &bufCharCount))
						throw getLastErrorMessage(__func__);
					return STRING(infoBuf, bufCharCount);
				}

				DBJ_INLINE auto user_name() {
					bufCharCount = INFO_BUFFER_SIZE;
					if (!GetUserName(infoBuf, &bufCharCount))
						throw getLastErrorMessage(__func__);
					return STRING(infoBuf, bufCharCount);
				}

				DBJ_INLINE auto system_directory() {
					bufCharCount = INFO_BUFFER_SIZE;
					if (!GetSystemDirectory(infoBuf, INFO_BUFFER_SIZE))
						throw getLastErrorMessage(__func__);
					return STRING(infoBuf, strnlen(infoBuf, INFO_BUFFER_SIZE - 1));
				}

				DBJ_INLINE auto windows_directory() {
					bufCharCount = INFO_BUFFER_SIZE;
					if (!GetWindowsDirectory(infoBuf, INFO_BUFFER_SIZE))
						throw getLastErrorMessage(__func__);
					return STRING(infoBuf, strnlen(infoBuf, INFO_BUFFER_SIZE - 1));
				}
			} // sysinfo
#if DBJ_TESTING_EXISTS
			namespace {
				using namespace sysinfo;
				DBJ_TEST_CASE("dbj win32 tests"){
					dbj::io::printex(
						"\nComputer name:\t\t", computer_name(),
						"\nUser name:\t\t", user_name(),
						"\nSystem directory:\t\t", system_directory(),
						"\nWindows directory:\t\t", windows_directory()
					);
				};
			}
#endif // DBJ_TESTING_EXISTS
	} // win32
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
