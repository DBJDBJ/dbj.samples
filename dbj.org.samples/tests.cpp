
#include "stdafx.h"
#ifdef DBJ_TESTING_EXISTS
// dbj++ tests
#include <test\dbj_commander_test.h>
#include <test\dbj_console_test.h>
#include <test\dbj_various_tests.h>
#include <test\dbj_crt_testing.h>
#include <test\dbj_defval_testing.h>

#include "dbj_traits_test.h"


#pragma region "More tests"
namespace {

	DBJ_TEST_CASE("lambda lists") {
		auto list = [&](auto ...xs) {
			return [=](auto access) { return access(xs...); };
		};

		auto head = [&](auto xs) {
			return xs([=](auto first, auto ...rest) { return first; });
		};

		auto tail = [&](auto xs) {
			return xs([=](auto first, auto ...rest) { return list(rest...); });
		};

		auto length = [&](auto xs) {
			return xs([=](auto ...z) { return sizeof...(z); });
		};

		// usage

		dbj::print( DBJ_NV( length(list(1, '2', "3")) ));
	}

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