// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

DBJ_TEST_SPACE_OPEN(narf)

template<typename T>
struct actual final {
	using type = std::decay_t< std::remove_pointer_t< T > >;
	constexpr static T value{};
};

DBJ_TEST_UNIT( dbj_narf)
{
	// no can do --> dbj::narf::make({ 1,2,3 })
	// as that argument is temporary

	int iar[]{1,2,3};

 auto DBJ_UNUSED(naf) = 
    DBJ_TEST_ATOM(
		// yes can do
		// iar is not temporary 
		// but only in the scope 
		//of this function
		dbj::narf::make( iar )
	);

 DBJ_TEST_ATOM(actual< decltype(naf) >::value);

}

DBJ_TEST_UNIT( a_bit_more_arh_narf_dancing ) {


	// narf to temporary
	// no can do --> auto narf_ = dbj::narf::make({ "A B C NARF" });
	// so
	char charr[]{ "A B C NARF" };
	auto narf_ = dbj::narf::make(charr);
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


DBJ_TEST_SPACE_CLOSE
