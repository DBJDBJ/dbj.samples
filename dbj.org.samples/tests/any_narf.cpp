// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

DBJ_TEST_SPACE_OPEN(narf)

DBJ_TEST_UNIT("dbj narf")
{
 auto DBJ_UNUSED(naf) = 
    DBJ_TEST_ATOM(
		dbj::narf::make(
			{ 1,2,3 }
		)
	);
}

DBJ_TEST_UNIT(" dbj a bit more arh narf dancing") {
	auto vof = [](auto t) {
		using actual_type = std::decay_t< std::remove_pointer_t< decltype(t) > >;
		return static_cast<actual_type>(t);
	};

	auto narf_ = dbj::narf::make({ "A B C NARF" });
	decltype(auto) narf_arf_ = dbj::narf::data(narf_);

	using CARH = dbj::arr::ARH<char, 255>;

	//CARH::ARR narf_to_arh
	//	= CARH::to_std_array( narf_arf_ );

	CARH::ARF native_arr_reference
		= CARH::to_arf(std::array<char, CARH::size>{ "CHAR ARR" });

	CARH::ARR std_aray
		= CARH::to_std_array(CARH::to_arf({ "CHAR ARR RIGHT SIZED" }));

	CARH::ARF literal_to_native_arr_reference
		= CARH::to_arf({ "CHAR ARR RIGHT SIZED" });
}


DBJ_TEST_SPACE_CLOSE(narf)
