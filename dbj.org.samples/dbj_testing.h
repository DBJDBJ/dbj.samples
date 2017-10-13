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

#include <vector>

namespace dbj {
	namespace testing {
		// if not false on command line it will be compiled into existence
#ifndef DBJ_TESTING_EXISTS
#define DBJ_TESTING_EXISTS (1==1)
#endif // !DBJ_TESTING_EXISTS

		/*
		C++17
		if constexpr(dbj::testing::RUNING) {
		// testing code will be compiled into existence
		}
		*/
#if DBJ_TESTING_EXISTS
		constexpr bool RUNING = true;
#else
		constexpr bool RUNING = false;
#endif
	}
}

namespace dbj {
	namespace testing {
				using testunittype = void(*)(); 
#if DBJ_TESTING_EXISTS

				void null_unit() {}

				struct Exception : public std::runtime_error {
				public:
					typedef std::runtime_error _Mybase;

					explicit Exception(const std::string & _Message)
						: _Mybase(_Message.c_str())
					{	// construct from message string
					}

					explicit Exception(const char *_Message)
						: _Mybase(_Message)
					{	// construct from message string
					}
				};

				/*  
		  using testunittype = decltype(null); 
		  will not work when iterating over a vector of testunittype's 
		*/

#ifndef _DEBUG
		namespace {
#endif // !_DEBUG
			typename typedef  std::vector< testunittype > test_units_vector;
			inline test_units_vector & TUV() {
				static test_units_vector test_units_{};
					return test_units_;
			}
#ifdef _DEBUG
			namespace {
#endif 
			inline test_units_vector::iterator find( testunittype tunit_) {
				return std::find(TUV().begin(), TUV().end(), tunit_);
			}

			inline bool found( testunittype tunit_ ) {
				return (
					TUV().end() != find( tunit_ )
					);
			}

			inline void append(testunittype tunit_) {
				/* do not insert twice the same test unit */
				if (!found(tunit_))
					TUV().push_back(tunit_);
			}

			inline bool unit_execute (testunittype tunit_){
				try {
					tunit_();
					return true ;
				}
				catch (...) {
					throw dbj::testing::Exception("Unknown exception while executing testing unit");
				}
			}

			struct adder;

			struct adder {
				const bool operator ()(testunittype tunit_) const {
					/* do not insert twice the same test unit */
					append(tunit_);
					return true;
				}
				
				static  adder & instance() {
					static adder singleton_ = adder();
					return singleton_;
				}
			};
		}
			/*
			How to add tunit

			#if DBJ_TESTING_EXISTS
			  namespace { 
			    auto dummy = dbj::testing::add(
				     [](){ std::cout << "Test" ;}
				) ;
				}
            #endif
			
			anonymous namespace is optional but good practice
			#if DBJ_TESTING_EXISTS is also obviously
			*/
			adder & add = adder::instance();

			/* mandatory macros for certified pain admirers */

#define DBJ_TEST_UNIT( source_code ) namespace { auto dummy = dbj::testing::add( source_code ); }
/*  NOTE: one can not use macros inside a code used as macro argument
    This will not compile
			DBJ_TEST_UNIT(  [](){
				#if 1 
				auto x = []() {}; 
			    #endif
			});

  "... C and C++ languages explicitly prohibit forming preprocessor directives as the result of macro expansion. .."
  Therefore ...
*/
			__forceinline void _stdcall execute() {

#define PRINTER dbj::io::printex

				PRINTER("\n\nSTARTING tests----------------------------------------------------\n\n");
				for (auto tunit : TUV() )
				{
					try {
						PRINTER("\n\nTEST----------------------------------------------------", tunit, "\n\n");
						unit_execute(tunit);
						PRINTER("\n\nTEST OK----------------------------------------------", tunit, "\n\n");
					}
					catch (dbj::testing::Exception & x) {
					print("\n\nException thrown from the testing unit\n\n%\n\n", x.what());
				}
				}

				PRINTER("\n\nFINSIHED tests----------------------------------------------------\n\n");
#undef PRINTER 
			}
#else // DBJ_TESTING_EXISTS is false
		auto add = [](void (*)() ) { return true; };
		__forceinline void _stdcall execute() { }

#define DBJ_TEST_UNIT(code)

#endif // 
	} // testing
} // dbj

#define DBJVERSION __DATE__ __TIME__
  // #pragma message("-------------------------------------------------------------")
#pragma message( "============> Compiled: " __FILE__ ", Version: " DBJVERSION)
  // #pragma message("-------------------------------------------------------------")
#pragma comment( user, "(c) " __DATE__ " by dbj@dbj.org | Version: " DBJVERSION ) 
#undef DBJVERSION
