// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

// testing the various stuff in this project
#include "../headers/dbj_lambda_lists.h"
#include "../headers/policy_classes.h"
#include "../headers/no_inheritance.h"
#include "../headers/dbj_tokenizer.h"
#include "../headers/dbj_atoms.h"
#include "../headers/dbj_lambda_lists.h"
#include "../dbj_any/dbj_any.h"
#include "../dbj_any/dbj_any_node.h"
#include "../dbjtree\dbj_tree_tests.h"
#include "../dbj_x/dbj_timer.h"
#include "../dbj_x/dbj_swapable_engines.h"

DBJ_TEST_SPACE_OPEN( local_tests )

DBJ_TEST_UNIT(" dbj swappable engines ") {

	using namespace dbj_samples;

	auto f1 = automobile_factory(engine_tag::old);
	auto r1 = f1.start();
	auto f2 = automobile_factory(engine_tag::next);
	auto r2 = f2.start();

	// should be barred
	// and it is: f1 = f2;
}

DBJ_TEST_UNIT(" timers ") {

	auto test = [&](dbj_samples::timer_kind which_ ) {
		auto timer_ = dbj_samples::create_timer(which_) ;

		auto stp = DBJ_TEST_ATOM(timer_.start());
		dbj_samples::sleep_seconds(1);
		return DBJ_TEST_ATOM(timer_.elapsed());
	};
	dbj::print("\nWIN32 Timer");
	auto elaps_1 = test(dbj_samples::timer_kind::win32 );
	dbj::print("\nModern Timer");
	auto elaps_2 = test(dbj_samples::timer_kind::modern );

	_ASSERTE( elaps_1 == elaps_2 );
}

	DBJ_TEST_UNIT(": famous dbj console ucrt crash")
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
		int result = _setmode(_fileno(stdout), _O_U16TEXT);
		if (result == -1)
			perror("Cannot set mode to:" DBJ_STRINGIFY(_O_U16TEXT));
		// both should display: кошка 日本
		// for any mode the second word is not displayed
		wprintf(L"\nwprintf() result: %s\n", specimen);
		// for any mode the following crashes the UCRT (aka Universal CRT)
		// printf("\nprintf() result: %S\n",specimen);
	}

	DBJ_TEST_UNIT(": constexpr strings") {
	// this all happens at compile time
	constexpr dbj::str_const my_string = "Hello, world!";
	static_assert(my_string.size() == 13, "");
	static_assert(my_string[4] == 'o', "");
	constexpr dbj::str_const my_other_string = my_string;
	//  constexpr char x = world[5]; // Does not compile because index is out of range!

	constexpr dbj::c_line<80, '='> L80;
	}

	DBJ_TEST_UNIT(": inheritance") {

		constexpr dbj::c_line<80, '-'> Line80; // compile time

		auto measure = [&Line80](auto object, const char * msg = "") -> void {
			dbj::print("\n", Line80,
				"\n", msg, "\nType name:\t", typeid(object).name(),
				"\nSpace requirements in bytes",
				"\nType:\t\t", sizeof(decltype(object)),
				"\nInstance:\t", sizeof(object),
				"\nAllocation:\t", alignof(decltype(object))
			);
		};

		dbj_samples::philology::HelloWorld<> hello{};
		dbj_samples::philology::HelloWorld2<> hello2{};

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

	DBJ_TEST_UNIT(": documents") {

		using IOperation = dbj_samples::docops::IOperation;
		/* OPTIONAL: configure the docops to use online operations
		*/
		auto ot = dbj_samples::docops::operations_type(IOperation::type::online);
		dbj_samples::documents::TextDoc text;
		dbj_samples::documents::opendoc(text, "world oyster");
	}

	DBJ_TEST_UNIT(": tokenizer_test") {

		const char * sentence = "abra % ka % dabra";
		dbj_samples::fm::tokenizer tok(sentence, "%");
		dbj::print("Input sentence: ", sentence);
		for (auto w : tok) {
			dbj::print("\ntok[", w, "] = [", tok[w], "]");
		}
		dbj::print("\n");
	}

DBJ_TEST_SPACE_CLOSE(local_tests)
