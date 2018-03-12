#pragma once#

#include <vector>
#include <array>

#include <dbj_array.h>

namespace dbj {
#if 0 // moved to dbj++
	/*
	------------------------------------------------------------------------------------
	return array reference to the C array inside std::array
	usage:

	decltype(auto) iar =
	internal_array_reference(
	std::array<int,3>{1,2,3}
	) ;
	*/
	template<typename T, size_t N,
		typename ARR = std::array<T, N>, /* std::array */
		typename ART = T[N],    /* C array */
		typename ARF = ART & ,  /* reference to it */
		typename ARP = ART * >  /* pointer   to it */
		constexpr inline
		ARF
		internal_array_reference(const std::array<T, N> & arr)
	{
		return *(ARP) const_cast<typename ARR::pointer>(arr.data());
	}

/*
Array Handler

(c) 2018 by dbj.org
*/
template< typename T, size_t N >
struct ARH
{
	// std::array type
	typedef std::array<T, N> ARR;
	// inbuilt ARray type
	typedef T ART[N];
	// reference to ART
	typedef ART& ARF;
	// pointer to ART
	typedef ART* ARP;

	/*
	return pointer to the underlying array
	of an instance of std::array<T,N>
	*/
	static constexpr ARP
		to_arp(const ARR & arr)
	{
		return (ARP)const_cast<typename ARR::pointer>(arr.data());
	}

	/*
	return reference to the underlying array
	of an instance of std::array<T,N>
	*/
	static constexpr ARF
		to_arf(const ARR & arr)
	{
		return *(ARP) const_cast<typename ARR::pointer>(arr.data());
	}
};
#endif
#if 1 // TODO : is this moved to dbj++?
	namespace {
		/*
		array to vector
		*/
		template<typename Type, size_t N, typename VType = std::vector<Type> >
		inline constexpr VType 
			intrinsic_array_to_vector(const Type(&arr_)[N])
		{
			return VType{ arr_, arr_ + N };
		}
	}
#endif

#ifdef DBJ_TESTING_EXISTS

	DBJ_TEST_UNIT(": test_dbj_std_arr_handling ")
	{
		{
			char arr_of_chars[]{ 'A','B','C' };
			char(&ref_to_arr_of_chars)[3] = arr_of_chars;
			(void)ref_to_arr_of_chars;
		}
		std::array<char,3> three_chars{ 'A','B','C' };

		const char(&uar)[3] = *(char(*)[3])three_chars.data();
		(void)uar;

using A16 =  dbj::arr::ARH<int, 16>;
A16::ARR arr;
std::generate(
	arr.begin(), arr.end(), [count = 0]() mutable -> int {
	return count++;
});
A16::ARP arp = A16::to_arp(arr); (void)arp;
A16::ARF arf = A16::to_arf(arr);  (void)arf;

		auto rdr0 = intrinsic_array_to_vector(arf);

		/*
		decltype(auto) bellow reveals the underlying type
		namely it transform int* to int(&)[]
		that is reference to c array inside std::array
		*/
		decltype(auto) arf2 = dbj::arr::internal_array_reference(arr);

		decltype(auto) rdr1 = dbj::arr::intrinsic_array_to_vector(arf2);

		decltype(auto) arf3 = arf2;
		auto rdr2 = intrinsic_array_to_vector(arf3);
	}
}

#endif
