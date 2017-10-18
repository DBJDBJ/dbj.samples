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

	DBJ_TEST_CASE("dbj crt") {
		printf("\n(c) %s by dbj.org\t\tcompiler is GCC [%ud]\n", __YEAR__, __VERSION__);

		char prompt[] = "0123456789";
		char * promptPTR = &prompt[0];

		// char arrays are using dbj.org versions     
		printf("\nfor char array [%s]\tstrlen(): [%zu]\t countof(): %zu", prompt, dbj::strlen(prompt), dbj::countof(prompt));
		printf("\nfor char array [%s]\tstrlen(): [%zu]\t countof(): %zu", prompt, dbj::strnlen(prompt, BUFSIZ), dbj::countof(prompt));

		// using UCRT strlen for char *
		printf("\nchar pointer strlen  of [%s] is: %zu", prompt, ::strlen(promptPTR));
		// using UCRT strnlen for char *
		// note: std has no  strnlen ... yet
		printf("\nchar pointer strnlen of [%s] is: %zu", prompt, ::strnlen(promptPTR, BUFSIZ));

	}


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
}
#endif
// EOF