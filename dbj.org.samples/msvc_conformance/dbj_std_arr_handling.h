#pragma once

#include <dbj_array.h>

#ifdef DBJ_TESTING_ONAIR
	
namespace {
	/*
	   struct XX final {
			std::string name{ "struct X" };
		};

		namespace dbj::win::con {
			namespace {
				inline void out(XX x_) {
					dbj::win::con::out(x_.name.c_str());
				}
			}
		}

		DBJ_TEST_UNIT(": dbj print for UDT ")
		{
			dbj::print(XX{});
		}
		*/
	DBJ_TEST_UNIT(": test_dbj_std_arr_handling ")
	{
		{
			char arr_of_chars[]{ 'A','B','C' };
			char(&ref_to_arr_of_chars)[3] = arr_of_chars;
			(void)ref_to_arr_of_chars;
		}
		{
			std::array<char, 3> three_chars{ 'A','B','C' };
			[[maybe_unused]]
			const char(&uar)[3] = *(char(*)[3])three_chars.data();
			// (void)uar;
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

			auto rdr0 = A16::to_vector (arf);
		}
		{
			[[ maybe_unused ]]
			constexpr char char_array[]{"ABCDEFGH"};
			const auto std_arr = dbj::native_to_std_array(char_array);
			dbj::print(std_arr);

			dbj::print(std::make_tuple("A", true, 42.0f));
		}
	}
}

#endif
