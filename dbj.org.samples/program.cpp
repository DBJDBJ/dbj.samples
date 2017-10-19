#include "stdafx.h"

int main(int argc, char* argv[])
{
	dbj::testing::execute();
	return true;
}

#ifdef DBJ_TESTING_EXISTS
namespace {
	using dbj::io::print;
	using dbj::io::printex;

	/*
	dbj crt caters for char, wchar_t, char16_t, char32_t
	for details please see https://docs.microsoft.com/en-us/cpp/cpp/char-wchar-t-char16-t-char32-t
	*/
	template< typename T, size_t N>
	DBJ_INLINE void strlen_strnlen_test( 
		const T (&prompt)[N]) 
	{
		printex("\n\nTesting array of type ", typeid(T).name() , " and of length ", N, "\t" );
		printex("\n\t", DBJ_NV(dbj::countof(prompt)));
		// char type arrays are using dbj.org "zero time" versions     
		printex("\n\t" , DBJ_NV(dbj::strlen(prompt)));
		printex("\n\t", DBJ_NV(dbj::strnlen(prompt, BUFSIZ)));

		// testing for the T * support 
		auto pointer_tester = [](auto cptr ) {
			// cptr becomes pointer due to the standard decay of C++ array to pointer
			using pointer_to_array = decltype(cptr);

			printex("\n\nTesting the support for the ", typeid(pointer_to_array).name(), " pointer to the same array\n");
			// using UCRT strlen
			printex("\n\t", DBJ_NV(dbj::strlen(cptr)));
			// using UCRT strnlen note: std has no strnlen ...
			printex("\n\t", DBJ_NV(dbj::strnlen(cptr, BUFSIZ)));
		};
		
			pointer_tester(prompt);
	}

	DBJ_TEST_CASE("dbj crt") {
		print("\n(c) % by dbj.org, MSVC version: %", __YEAR__ , __VERSION__);
		
			char	 promptA[]  =  "0123456789";
			wchar_t  promptW[]  = L"0123456789";
			char16_t prompt16[] = u"0123456789";
			char32_t prompt32[] = U"0123456789";

			strlen_strnlen_test(promptA);
			strlen_strnlen_test(promptW);
			strlen_strnlen_test(prompt16);
			strlen_strnlen_test(prompt32);
	}

#if 0
	struct X final { };

	DBJ_TEST_CASE("dbj traits") {
		using xref = dbj::traits::Referee<X>;
		printex("\nFrom inside [", __func__, "]\t", DBJ_NV(typeid(xref).name()));
	}

	DBJ_TEST_CASE("dbj inheritance") {

		auto print_line = [](bool new_line = true, const size_t len_ = 80, const char chr_ = '-') {
			const std::string line_(len_, chr_);
			printex("\n", line_.data());
		};

		auto measure = [&print_line](auto object, const char * msg = "") -> void {
			print_line();
			printex("\n", msg, "\nType name:\t", typeid(object).name(),
				"\nSpace requirements in bytes",
				"\nType:\t\t", sizeof(decltype(object)),
				"\nInstance:\t", sizeof(object),
				"\nAllocation:\t", alignof(decltype(object))
			);
		};

		dbj::philology::HelloWorld<> hello{};
		dbj::philology::HelloWorld2<> hello2{};

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
	};


	DBJ_TEST_CASE("dbj util")
	{
		auto f = []() -> std::tuple<int, int> // this function returns multiple values
		{
			int x = 5;
			return std::make_tuple(x, 7); // return {x,7}; in C++17
		};

		// heterogeneous tuple construction
		int n = 1;
		auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
		n = 7;
		auto[v1, v2, v3, v4, v5] = t;
		std::cout << "The value of t is " << "("
			<< v1 << ", " << v2 << ", "
			<< v3 << ", " << v4 << ", "
			<< v5 << ")\n";

		// function returning multiple values
		auto[a, b] = f();
		std::cout << a << " " << b << "\n";
	}

	DBJ_TEST_CASE("dbj util testing") 
	{
		using namespace dbj::util;
		auto fun = [](auto arg) -> void { cout << (arg);  };
		repeater(fun, "XYZ", true, 1, 0.2f);
		auto tupler = []() { return make_tuple(1, 2, 3); };
		// auto[ a, b, c] = tupler();
		auto whatever = make_from_tuple< decltype(tupler()) >(tupler());
		printex(whatever);
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

		using dbj::io::print;
		const char * sentence = "abra % ka % dabra";
		dbj::fm::tokenizer tok(sentence, "%");
		print("Input sentence: [%]", sentence);
		for (auto w : tok) {
			print("\ntok[%] = [%]", w, tok[w]);
		}
		print("\n\n");
	}
#endif // 0
}
#endif
// EOF