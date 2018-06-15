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
#include "../dbj_x/dbj_polymorph.h"

DBJ_TEST_SPACE_OPEN(lambada_mixer)

template< typename T>
inline auto any_mixer ( T specimen_ ) 
{
	using specimen_type = T; 

	struct any_front final {

		typedef specimen_type value_type;
		
		value_type val() const {
			return std::any_cast<value_type>(any_);
		}

		value_type val(value_type new_val_ ) {
			this->any_ = new_val_;
			return this->val();
		}

		// not necessary for the mechanism 
		operator std::string () const {
			return { typeid(this->any_).name() };
		}

	private:
		std::any any_{};
	};
		return any_front{};
};

DBJ_TEST_UNIT(" lambada mixed any") {
	auto any_int = any_mixer(42);
	auto any_str = any_mixer(std::string{"string"} );

	DBJ_TEST_ATOM(any_int);
	DBJ_TEST_ATOM(any_str);

	DBJ_TEST_ATOM(any_int.val());
	DBJ_TEST_ATOM(any_str.val());

}

DBJ_TEST_SPACE_CLOSE(lambada_mixer)

DBJ_TEST_SPACE_OPEN(local_tests)

// return the default instance of the
// type obtained at runtime
auto maker_of = [&](auto && prototype_)
{
	using the_type = std::decay_t<decltype(prototype_)>;
	return  []() -> the_type { return  the_type{}; };
};

enum class engine_tags : char { legacy = 'L', contemporary = 'C', hybrid = 'H' };

enum class wheel_tags : char { alloy = 'A', chrome_vanadium = 'C', steel = 'S' };

/*
   care_base_facade -- not good using the term "base" it implies there might be some hiddent inheritance somewhere
   car_interface -- even worse ;)
*/
struct car_front final {
//the 'start()' message
template<typename T>
constexpr bool start(const T & engine_) const
{
	return engine_.start(); 
}

template<typename W>
constexpr int knobs_required (const W & wheel_) const
{
	return wheel_.fix_points();
}

template<typename W, typename E>
constexpr const char * type_tag(const W & wheel_, const E & engine_ ) const
{
	static char car_tag[]{ (char)engine_.uid(), '-', (char)wheel_.uid(), (char)0 };
	return car_tag;
}

};

namespace wheels {
	// wheels
	struct alloy final {
		constexpr wheel_tags uid() const { return wheel_tags::alloy; }
		constexpr int fix_points() const { return 3; }
	};

	struct chrome_vanadium final {
		constexpr wheel_tags uid() const { return wheel_tags::chrome_vanadium; }
		constexpr int fix_points() const { return 6; }
	};

	struct steel final {
		constexpr wheel_tags uid() const { return wheel_tags::steel; }
		constexpr int fix_points() const { return 4; }
	};
} // wheels

namespace engines{
	// engines
	struct diesel final {
		constexpr engine_tags uid() const { return engine_tags::legacy; }
		constexpr bool start() const { return true; }
	};

	struct petrol final {
		constexpr engine_tags uid() const { return engine_tags::contemporary; }
		constexpr bool start() const { return true; }
	};

	struct hybrid final {
		constexpr engine_tags uid() const { return engine_tags::hybrid; }
		constexpr bool start() const { return true; }
	};
} // engines

inline auto car_assembly_line = []( auto engine_, auto wheels_ ) 
{
// assemble and use the car perhaps this way
/*
car_facade base{ };

auto starter   =   [=]() { return base.start(engine_);  };
auto identty  =    [=]() { return base.type_tag(wheels_,engine_);  };
auto knobs    =    [=]() { return 4 * base.knobs_required(wheels_);  };

auto tuple_of_methods = std::make_tuple( starter, identty,  knobs );

auto rv1 = std::get< 0 >(tuple_of_methods)();
auto rv2 = std::get< 1 >(tuple_of_methods)();
auto rv3 = std::get< 2 >(tuple_of_methods)();

or this way ...
*/
using ET = decltype (engine_);
using WT = decltype (wheels_);

struct finished_car_ final {

	ET engine{};
	WT wheels{};

	car_front front{};

	auto start() { return front.start(engine); }
	auto tag  () { return front.type_tag(wheels, engine); }
	auto knobs() { return 4 * front.knobs_required(wheels);  };

	operator std::string() { return this->tag();  }
};

return finished_car_{};
};

DBJ_TEST_UNIT(" polymorph but not inheritor") 
{
	// obtain the car
	auto diesel_car = DBJ_TEST_ATOM(
		car_assembly_line(engines::diesel{}, wheels::alloy{} )
		);
	auto petrol_car = DBJ_TEST_ATOM(
		car_assembly_line(engines::petrol{}, wheels::chrome_vanadium{} )
		);

	auto rv1 = DBJ_TEST_ATOM(diesel_car.start());
	auto rv2 = DBJ_TEST_ATOM(diesel_car.tag());
	auto rv3 = DBJ_TEST_ATOM(diesel_car.knobs());

	auto rv4 = DBJ_TEST_ATOM(petrol_car.start());
	auto rv5 = DBJ_TEST_ATOM(petrol_car.tag());
	auto rv6 = DBJ_TEST_ATOM(petrol_car.knobs());

	auto ht1 = DBJ_TEST_ATOM( typeid(diesel_car).hash_code() );
	auto ht2 = DBJ_TEST_ATOM( typeid(petrol_car).hash_code() );

	auto same = DBJ_TEST_ATOM(ht1 == ht2);

	dbj::print("\ndiesel and petrol car are ");

	if (! std::is_same_v< decltype(diesel_car), decltype(petrol_car) >) {
		dbj::print(" NOT ");
	}

	dbj::print(" the same type...");

	// problem?
}

// typedef void(*recipe)(void);



// using full_rcp = std::function< decltype(string_rcp) > ;

DBJ_TEST_UNIT(" Damn! Why I have not thought of this before?! ") {

	using namespace std;

auto recipe_1	= maker_of( vector<int>{1,2,3} );
auto value_1	= recipe_1() ;

auto check = value_1.size();

	std::any	a1	= recipe_1;


}

auto reporter = [&](const char * prompt = "", const void * this_ptr = nullptr) {
	char address_str[128]{0};
	(void)::sprintf_s(address_str, "%p", this_ptr);
	return dbj::print("\n[", address_str, "]\t", prompt );
};

DBJ_TEST_UNIT(" dbj fundamental issues ") {
	struct X {
		X() noexcept {
			reporter("X constructed", this);
		}
		~X() {
			reporter("X destructed", this);
		}
		X(const X & x) noexcept {
			reporter("X copy constructed", this);
			bad = x.bad;
		}
		X & operator = (const X & x) {
			reporter("X copy assigned", this);
			bad = x.bad;
			return *this;
		}
		X(X && x) noexcept {
			reporter("X move constructed", this); std::swap(bad, x.bad);
		}
		X & operator = (X && x) noexcept {
			reporter("X move assigned", this);
			std::swap(bad, x.bad);
			return *this;
		}

		const char * bad = "INITIAL STATE" ;
	};

	auto make_invalid_x = []() 
		-> X & 
	{   
		X moved_from;
		X && survived = std::move(moved_from);
		return moved_from;
	};

	X & moved_from = make_invalid_x();
	X && survived = std::move(moved_from);
	auto & wot = moved_from.bad;
}

#if 0
DBJ_TEST_UNIT(" dbj swappable engines ") {

	using namespace car_factory;

	auto car_1 = 
	 car_assembly_line(
		engine_tag::old	);

	car_1.start();

	auto car_2 = 
	  car_assembly_line(
		engine_tag::next );

	car_2.start();

	// should be barred
	// and it is: f1 = f2;
}
#endif
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
