
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

#pragma region LAMBDA LISP

	DBJ_TEST_CASE( dbj::FILELINE(__FILE__, __LINE__, ": lambda lists") ) {
		/* the key abstraction 
		   returns lambda that will call the access lambda on varargs given to list lambda
		 */
		auto list = [&](auto ...xs) {
			return [=](auto proc_lambda) { return proc_lambda(xs...); };
		};

		/*
		   to understand it all: 

		   auto l123 = list(1,2,3) // returns --> [=](auto proc_lambda) { return proc_lambda(xs...); };

		   head(l123) //  head() calls the lambda (see above) 
		                     // thus head() calls into the returned lambda from list()
							 // head calls lambda l123() 
							 // with internal lambda as agument to l123() now that
							 // (!critical detail!) proc lambda receives xs... that is saved on the 
							 // argument stack of the list() call
		*/
		auto head = [&](auto list_lambda) {
			return list_lambda(
				[=](auto first, auto ...rest) { return first; }
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
		auto print = [](auto list_lambda) {
			// we call here what list, head or tail, have returned
			return list_lambda(
				[=](auto ...args) { 
				   dbj::print_sequence( args...);
			    }
			);
		};

		// usage
		auto my_list = list(1, '2', "3", false, 13.0f);
		auto my_head = head(my_list);
		auto my_tail = tail(my_list);

		dbj::print("\nlist: ");
			print(my_list);
		dbj::print("\nhead: ", my_head );
		dbj::print("\ntail: ");
			print(my_tail);

		// dbj::print("\n", DBJ_NV( length(list()) ));

	}
#pragma region LAMBDA LISP

DBJ_TEST_CASE("dbj inheritance") {

	auto print_line = [](bool new_line = true, const size_t len_ = 80, const char chr_ = '-') {
		const std::string line_(len_, chr_);
		dbj::print("\n", line_.data());
	};

	auto measure = [&print_line](auto object, const char * msg = "") -> void {
		print_line();
		dbj::print("\n", msg, "\nType name:\t", typeid(object).name(),
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
	print_line();
	hello.run("\nHelloWorld -- Default policies");
	hello2.run("\nHelloWorld2 -- No inheritance");
	print_line();
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