#pragma once

#include <vector>
#include <array>

#include <dbj_array.h>

namespace dbj {

#ifdef DBJ_TESTING_EXISTS

	DBJ_TEST_UNIT(": test_dbj_std_arr_handling ")
	{
		{
			char arr_of_chars[]{ 'A','B','C' };
			char(&ref_to_arr_of_chars)[3] = arr_of_chars;
			(void)ref_to_arr_of_chars;
		}
		{
			std::array<char, 3> three_chars{ 'A','B','C' };

			const char(&uar)[3] = *(char(*)[3])three_chars.data();
			(void)uar;
		}
		{
			using A16 = dbj::arr::ARH<int, 16>;
			A16::ARR arr;
			std::generate(
				arr.begin(), arr.end(), [count = 0]() mutable -> int {
				return count++;
			});
			A16::ARP arp = A16::to_arp(arr); (void)arp;
			A16::ARF arf = A16::to_arf(arr);  (void)arf;

			auto rdr0 = dbj::arr::intrinsic_array_to_vector(arf);

			/*
			decltype(auto) bellow reveals the underlying type
			namely it transform int* to int(&)[]
			that is reference to c array inside std::array
			*/
			decltype(auto) arf2 = dbj::arr::internal_array_reference(arr);

			decltype(auto) rdr1 = dbj::arr::intrinsic_array_to_vector(arf2);

			decltype(auto) arf3 = arf2;
			auto rdr2 = dbj::arr::intrinsic_array_to_vector(arf3);
		}
		{
			[[ maybe_unused ]]
			constexpr char char_array[]{"ABCDEFGH"};
			const auto std_arr = dbj::to_array(char_array);
			dbj::print(std_arr);

			dbj::print( std::make_tuple( "A", true, 42.0f ) );
		}
	}
}

#endif
