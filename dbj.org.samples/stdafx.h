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

#ifndef UNICODE
#error __FILE__ requires UNICODE builds
#endif

#ifndef _MSC_VER
#error This code requires Visual C++ 
#endif // !_MSC_VER
#if _MSC_VER < 1911
#error This code requires Visual C++ 14.1 or better
#endif
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <array>
#include <vector>
/*
dbj begins here
*/
#define implements public
// inline is the keyword, in C++ and C99.
#define DBJ_INLINE inline
#define DBJ_ASSERT /*_ASSERTE*/

#ifdef __cpp_lib_is_final
#define DBJ_FINAL final
#else
#define DBJ_FINAL
#endif
#include <dbjio.h>
using dbj::io::print;
using dbj::io::printex;

//
#include "dbj_testing.h"
#include "dbj_traits.h"
#include "policy_classes.h"
#include "no_inheritance.h"
#include "dbj_util.h"
#include "dbj_tuple_unpack.h"
#include "dbj_tokenizer.h"
#include "dbj_experimental.h"
#include "dbj_win32.h"
#include "dbj_trace.h"