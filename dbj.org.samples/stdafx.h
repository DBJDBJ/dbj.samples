#pragma once
/*
Copyright 2017,2018 by dbj@dbj.org

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
// #define DBJ_TESTING_ONAIR
#include <dbj++.h> 

#ifdef DBJ_TESTING_ONAIR
#include <dbj++tests.h> 
#endif

#include <fcntl.h>
#include <io.h>
#include <functional>
#include <any>
/*
The header <typeinfo> must be included before using typeid (if the header is not included, 
every use of the keyword typeid makes the program ill-formed.)
*/
#include <typeinfo>
// dbj++ does not use cout and/or wcout
// #include <iostream>
// #include <array>





