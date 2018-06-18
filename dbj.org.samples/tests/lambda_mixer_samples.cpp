// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

#include "../dbj_x/dbj_swapable_engines.h"
#include "../dbj_x/dbj_polymorph.h"
#include "../dbj_any/dbj_any.h"


DBJ_TEST_SPACE_OPEN(lambada_mixer)

DBJ_TEST_UNIT(" dbj micro log test") {

	dbj::log::print(L"\nHello, asre you ok", L"with two, or", " three arguments?");
	dbj::log::instance().flush();
	dbj::log::print("\nDBJ")(" micro")(" log!");
	dbj::log::instance().flush();

	std::byte dumzy[[maybe_unused]]{};

}

template< typename T>
inline auto any_mixer(T specimen_)
{
	using specimen_type = T;

	specimen_type why_do_we_need_this{ specimen_ };

	struct any_front final {

		typedef specimen_type value_type;

		value_type val() const {

			if (!this->any_.has_value()) 
				return value_type{};

			try {
				return std::any_cast<value_type>(any_);
			}
			catch (std::bad_any_cast & bac)
			{
				DBJ::TRACE("\n\n%s\n\n", bac);
				throw bac;
			}
		}

		value_type val(value_type new_val_) {
			this->any_ = new_val_;
			return this->val();
		}

		// not necessary for the mechanism 
		operator std::string() const {
			return std::string{ "std::any holding " }.append(typeid(value_type).name());
		}

		// private:
		mutable std::any any_{};
	};
	return any_front{ why_do_we_need_this };
};

DBJ_TEST_UNIT(" lambada mixed any") {
	auto any_int = any_mixer(42);
	auto any_str = any_mixer(std::string{ "string" });

	auto ina = DBJ_TEST_ATOM(any_int);
	auto ins = DBJ_TEST_ATOM(any_str);

	auto intv = DBJ_TEST_ATOM(any_int.val());
	auto strv = DBJ_TEST_ATOM(any_str.val());

}

enum class engine_tags : char { legacy = 'L', contemporary = 'C', hybrid = 'H' };

enum class wheel_tags : char { alloy = 'A', chrome_vanadium = 'C', steel = 'S' };

/*
car_base_facade -- not good using the term "base" it implies there might be some hiddent inheritance somewhere
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
	constexpr int knobs_required(const W & wheel_) const
	{
		return wheel_.fix_points();
	}

	template<typename W, typename E>
	constexpr const char * type_tag(const W & wheel_, const E & engine_) const
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

namespace engines {
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

inline auto car_assembly_line = [](auto engine_, auto wheels_)
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
		auto tag() { return front.type_tag(wheels, engine); }
		auto knobs() { return 4 * front.knobs_required(wheels); };

		operator std::string() { return this->tag(); }
	};

	return finished_car_{};
};

DBJ_TEST_UNIT(" polymorph but not inheritor")
{
	// obtain the car
	auto diesel_car = DBJ_TEST_ATOM(
		car_assembly_line(engines::diesel{}, wheels::alloy{})
	);
	auto petrol_car = DBJ_TEST_ATOM(
		car_assembly_line(engines::petrol{}, wheels::chrome_vanadium{})
	);

	auto rv1 = DBJ_TEST_ATOM(diesel_car.start());
	auto rv2 = DBJ_TEST_ATOM(diesel_car.tag());
	auto rv3 = DBJ_TEST_ATOM(diesel_car.knobs());

	auto rv4 = DBJ_TEST_ATOM(petrol_car.start());
	auto rv5 = DBJ_TEST_ATOM(petrol_car.tag());
	auto rv6 = DBJ_TEST_ATOM(petrol_car.knobs());

	auto ht1 = DBJ_TEST_ATOM(typeid(diesel_car).hash_code());
	auto ht2 = DBJ_TEST_ATOM(typeid(petrol_car).hash_code());

	auto same = DBJ_TEST_ATOM(ht1 == ht2);

	dbj::print("\ndiesel and petrol car are ");

	if (!std::is_same_v< decltype(diesel_car), decltype(petrol_car) >) {
		dbj::print(" NOT ");
	}

	dbj::print(" the same type...");

	// problem?
}

DBJ_TEST_UNIT(" dbj swappable engines ") {

	auto car_1 =
		car_factory::assembly_line(
			car_factory::engine_tag::old);

	car_1.start();

	auto car_2 =
		car_factory::assembly_line(
			car_factory::engine_tag::next);

	car_2.start();

	// should be barred
	// and it is: f1 = f2;
}

DBJ_TEST_SPACE_CLOSE(lambada_mixer)
