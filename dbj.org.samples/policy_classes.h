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
using namespace std;

namespace dbj {
	namespace philology {
		/*
		 interfaces to writing and language policies
		*/
		__interface IWriterPolicy {
			void write(string);
		};
		__interface ILanguagePolicy {
			string translate(string);
		};
		// classes to be used
		// as default policies		
		struct WideWriterPolicy : implements IWriterPolicy {
			void write(string msg) { print("%", msg); }
		};
		struct LanguagePolicyEnglish : implements ILanguagePolicy {
			string translate(string msg) { return msg; }
		};
		// german is also here as some folks will 
		// want to make it a default language
		struct LanguagePolicyGerman : implements ILanguagePolicy {
			string translate(string msg) { return msg; }
		};

		template <
			/* make English to be compiled as the default language */
			typename LanguagePolicy = LanguagePolicyEnglish,
			/* compile wide string writer as the defualt output policy */
			typename OutputPolicy = WideWriterPolicy
		>
			class HelloWorld : private OutputPolicy, private LanguagePolicy
		{
			using OutputPolicy::write;
			using LanguagePolicy::translate;

		public:
			// Behaviour method
			void run(string message) 
			{
				// Two policy methods
				write(translate(message));
			}
		};

/*
No inhertiance! No name lookup issues.
*/
template <
	typename LanguagePolicy = LanguagePolicyEnglish,
	typename OutputPolicy = WideWriterPolicy >
	class HelloWorld2
{
public:
	// Behaviour method
	void run( string msg)
	{
		// not made before this point; if ever.
		static LanguagePolicy language{};
		static OutputPolicy printer{};
		// Two policy methods
		printer.write( 
			language.translate(msg)
		);
	}
};
        
inline void print_line( bool new_line = true, const size_t len_ = 80, const char chr_ = '-')  {
	const string line_( len_, chr_ );
		print("\n%",line_.data());
    }

auto dumsy = dbj::testing::add(
	[]() -> void {

			auto measure = []( auto object, const char * msg = "" ) -> void {
				print_line();
#if 0
				print( "\n%\nType name:\t%"
					  "\nSpace requirements in bytes"
					  "\nType:\t\t%"
					  "\nInstance:\t%"
					  "\nAllocation:\t%",
					msg,
					typeid(object).name(),
					sizeof(decltype(object)),
					sizeof(object),
					alignof(decltype(object))
				);
				// printext bellow is easier to read
#endif
				printex("\n", msg,"\nType name:\t", typeid(object).name(),
					"\nSpace requirements in bytes",
					"\nType:\t\t" , sizeof(decltype(object)),
					"\nInstance:\t", sizeof(object),
					"\nAllocation:\t",	alignof(decltype(object))
				);

			};

		HelloWorld<> hello{};
		HelloWorld2<> hello2{};

		print("\nBEFORE RUN\n");
			measure(hello);
			measure(hello2);
			print_line();
			hello.run("\nHelloWorld -- Default policies");
			hello2.run("\nHelloWorld2 -- No inheritance");
			print_line();
			print("\nAFTER RUN\n");
			measure(hello);
			measure(hello2);
		}
);

	} // namespace philology 
} //namespace dbj 
