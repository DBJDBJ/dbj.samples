
#include "stdafx.h"
#ifdef DBJ_TESTING_EXISTS
// dbj++ tests
#include <test\dbj_commander_test.h>
#include <test\dbj_console_test.h>
#include <test\dbj_various_tests.h>
#include <test\dbj_crt_testing.h>
#include <test\dbj_defval_testing.h>

#include "dbj_traits_test.h"

#include "dbjtree\dbj_tree_tests.h"

#include <fcntl.h>
#include <io.h>

#pragma region "More tests"
namespace {
	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": famous dbj console ucrt crash")) 
		{
		// кошка 日本
		constexpr wchar_t specimen[] =
			{ L"\x043a\x043e\x0448\x043a\x0430 \x65e5\x672c\x56fd" };
		dbj::print("\n", specimen, "\n");
		/*
		<fcntl.h>
		_O_U16TEXT, _O_U8TEXT, or _O_WTEXT 
		to enable Unicode mode
		_O_TEXT to "translated mode" aka ANSI
		_O_BINARY sets binary (untranslated) mode,
		*/
		int result = _setmode(_fileno(stdout), _O_U8TEXT);
		if ( result == -1 )
			perror("Cannot set mode to:" DBJ_STRINGIFY(_O_U8TEXT) );
		// both should display: кошка 日本
		// for any mode the second word is not displayed
		wprintf(L"\nwprintf() result: %s\n",specimen);
		// for any mode the following crashes the UCRT (aka Universal CRT)
		// printf("\nprintf() result: %S\n",specimen);
	}

	// https://hackernoon.com/a-tour-of-c-17-if-constexpr-3ea62f62ff65
	struct X {
		X & xbegin() { return *this;  }
		X & end()   { return *this;  }
	};

	template<class T>
	constexpr auto supportsAPI(T x) 
		-> decltype( & T::end, & T::begin, std::true_type{})
	{
		return {};
	}
	constexpr auto supportsAPI(...) -> std::false_type {
		return {};
	}
	namespace detail {

		template<class T>
		constexpr auto has_begin(T x) -> decltype(std::declval<T>().begin(), bool )
		{
			return true;
		}

		constexpr auto has_begin(...) -> bool {
			return false;
		}

		template<class T>
		void begin(T x) {
			if constexpr(has_begin(T{})) {
				// only gets compiled if the condition is true
				x.begin();
			}
		}
	}


	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": lambda constexpr combinations")) {

		// decltype(std::declval<X>().begin()) dumsy ;

		auto itdoes = detail::has_begin(std::string{});
		detail::begin(X());
	}
#pragma region constexpr strings
	
	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": constexpr strings")) {
		// this all happens at compile time
		constexpr dbj::str_const my_string = "Hello, world!";
		static_assert(my_string.size() == 13, "");
		static_assert(my_string[4] == 'o', "");
		constexpr dbj::str_const my_other_string = my_string;
		//  constexpr char x = world[5]; // Does not compile because index is out of range!
		
		constexpr dbj::c_line<80, '='> L80;
	}
#pragma endregion 


#pragma region LAMBDA LISP

	DBJ_TEST_CASE( dbj::FILELINE(__FILE__, __LINE__, ": lambda lists") ) {
		
		/* 
		the key abstraction  
		*/
		auto list = [&](auto ...xs) {
			/*return lambda */
			return [=](
				/* single argument is lambda */
				auto proc_lambda
				) { 
				    /* lambda given is called with variadic arguments of the list() */
				    return proc_lambda(xs...); 
			   };
		};

		/*
		   to understand it all: 

		   auto l123 = list(1,2,3) // returns lambda --> [=](auto proc_lambda) { return proc_lambda(xs...); };

		   head(l123) //  head() is given that lambda as argument
		   inside head() calls into that lambda -- see it now above once more
		   thus head() does -- l123() 
		   agument to l123() is lambda defined in the call to it as argument
		   (!critical detail here!) that lambda receives xs... that is saved on the 
		   argument stack of the list() call previously -- see the list() above

		   now try to follow all of this in the debuger
		  */
		auto head = [&](auto list_lambda) {
			return list_lambda(
				/*
				this is given as proc_lambda() to the result lambda od the list()
				arguments are the original variadic arguments  of the list()
				*/
				[=](auto first, auto ...rest) { 
				/* 
				we return list of one element so that users of head() and tail()
				can use  then both in a symetrical manner
				see the usage of print() for example
				*/
				return list(first); 
			    }
			);
		};

		// returns list() lambda
		auto tail = [&](auto list_lambda) {
			return list_lambda([=](auto first, auto ...rest) { return list(rest...); });
		};

		auto length = [&](auto list_lambda) {
			return list_lambda([=](auto ...z) { return sizeof...(z); });
		};

		/* dbj added */
		auto list_to_tuple = [&](auto list_lambda) {
			// 
			return list_lambda(
				[=](auto ...args) { 
					return std::make_tuple(args...);
			    }
			);
		};

		// usage
		auto my_list = list(1, '2', "3", false, 13.0f); // return lambda() internal to list()
		auto my_head = head(my_list); // returns list of one element -- list(1)
		auto my_tail = tail(my_list); // returns list('2', "3", false, 13.0f)
		/*
		 after playing with lambda lists we print what we need to print
		 we use list_to_tuple() because dbj::print() knows how to print touples
		 but not lambda lists
		 */
		dbj::print("\nlist: ", list_to_tuple(my_list ));
		dbj::print("\nhead: ", list_to_tuple(my_head ));
		dbj::print("\ntail: ", list_to_tuple(my_tail ));

		// dbj::print("\n", DBJ_NV( length(list()) ));

	}
#pragma region LAMBDA LISP

DBJ_TEST_CASE("dbj inheritance") {

	constexpr dbj::c_line<80, '-'> Line80 ; // compile time

	auto measure = [&Line80](auto object, const char * msg = "") -> void {
		dbj::print( "\n", Line80 ,
			"\n", msg, "\nType name:\t", typeid(object).name(),
			"\nSpace requirements in bytes",
			"\nType:\t\t", sizeof(decltype(object)),
			"\nInstance:\t", sizeof(object),
			"\nAllocation:\t", alignof(decltype(object))
		);
	};

	dbj::philology::HelloWorld<> hello{};
	dbj::philology::HelloWorld2<> hello2{};

	dbj::print("\nBEFORE RUN\n");
	measure(hello);
	measure(hello2);
	dbj::print("\n", Line80);
	hello.run("\nHelloWorld -- Default policies");
	hello2.run("\nHelloWorld2 -- No inheritance");
	dbj::print("\n", Line80);
	dbj::print("\nAFTER RUN\n");
	measure(hello);
	measure(hello2);
};


DBJ_TEST_CASE("dbj util")
{
	// this function returns multiple values
	auto f = []() -> std::tuple<int, int> 
	{
		return std::make_tuple(5, 7); 
		// returns {5,7}; in C++17
	};

#define DBJ_IL(T,...)  /*std::forward<std::initializer_list<T>>*/(std::initializer_list<T>{__VA_ARGS__})
	// basically forget about init lists passing
	// https://stackoverflow.com/questions/20059061/having-trouble-passing-multiple-initializer-lists-to-variadic-function-template/47159747#47159747
	// yes I know macro + variadic = horific
	// but in here it seems as a sort of a solution
	// but no can do for msvc
	// dbj::print(std::initializer_list<int>{ 1, 2, 3, 4, 5 });

	// heterogenous tuple construction
	int n = 1;
	auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
	// C++17 tuple unpacking , provided we know the tuple size
	// auto[v1, v2, v3, v4, v5] = t;
    // passing tuples is not a problem
	dbj::print("\n",t,"\n");

	// function returning multiple values
	dbj::print( "\nfunction returns multiple values " , f());
}

DBJ_TEST_CASE("dbj documents") {

	using IOperation = dbj::docops::IOperation;
	/* OPTIONAL: configure the docops to use online operations
	*/
	auto ot = dbj::docops::operations_type(IOperation::type::online);
	dbj::documents::TextDoc text;
	dbj::documents::opendoc(text, "world oyster");
}

DBJ_TEST_CASE("dbj tokenizer_test") {

	const char * sentence = "abra % ka % dabra";
	dbj::fm::tokenizer tok(sentence, "%");
	dbj::print("Input sentence: ", sentence);
	for (auto w : tok) {
		dbj::print("\ntok[",w,"] = [",tok[w],"]");
	}
	dbj::print("\n");
}
#pragma endregion

	struct Struct final {
		template<typename T>
		auto method( T arg ) const {
			return __FUNCSIG__;
		}
	};

	inline auto s_m() {
	}

	/*
	*/
	using  methodP = void (Struct::*)(); // methodP is a type
	using  fakeMP = void (std::string::*)();

	constexpr auto tname = [&](const auto & T) -> const char * { return typeid(decltype(T)).name(); };

	constexpr auto is_member = [&](auto member) {
		return std::is_member_function_pointer<decltype(member)>::value;
	};

	DBJ_TEST_CASE("dbj experimental traits") {
		auto mp1 = dbj::functional::mem_fn(&Struct::method<bool>);
		auto mp2 = is_member(&Struct::method<bool>);
		auto mp3 = std::is_invocable< decltype(is_member), decltype(&tname) >::value;
	}
}
#endif
// EOF