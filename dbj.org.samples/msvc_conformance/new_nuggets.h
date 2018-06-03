#pragma once
#include <vector>
#include <algorithm>
#include <set>
#include <iterator>
#include <functional>
#include <dbj_traits.h>

namespace dbj {

#if 0
	/*
	array to vector
	*/
	template<typename Type, size_t N, typename outype = std::vector<Type> >
	inline constexpr outype array_to_vector(const Type(&arr_)[N])
	{
		return outype{ arr_, arr_ + N };
	}

	/*
	find an item in anything that has begin and end iterators
	*/
	auto find = [](auto sequence, auto item) constexpr -> bool {
		return std::find(std::begin(sequence), std::end(sequence), item) != std::end(sequence);
	};

	/*
	bellow works for all the std:: string types and string view types
	but it also works for char pointers and wchar_t pointers
	and it is all inside the single lambda
	which is fast because redundant code is removed at compile time
	this is all invaluable for code maintenance
	all is in "one place", simple and easy to maintain
	*/
	auto starts_with = [](auto val_, auto mat_) -> bool {

		static_assert(dbj::eqt(val_, mat_),
			"dbj::does_start [error] arguments not of the same type"
			);

		if  constexpr(eqt(val_, char_star{})) {
			// #pragma message ("val type is char *")
			return starts_with(std::string(val_), std::string(mat_));
		}
		else if  constexpr(eqt(val_, wchar_star{})) {
			// #pragma message ("val type is wchar_t *")
			return starts_with(std::wstring(val_), std::wstring(mat_));
		}
		else {
			return 0 == val_.compare(0, mat_.size(), mat_);
		}
	};


	/*
	------------------------------------------------------------------------------------
	*/
	template <typename Type>
	inline std::vector<Type> remove_duplicates(const std::vector<Type> & vec, const bool sort = false) {

		if (sort) {
			/*
			this is apparently also faster for very large data sets
			*/
			const std::set<Type> s(vec.begin(), vec.end());
			return std::vector<Type>{ s.begin(), s.end() };
		}
		else {
			std::vector<Type> unique_chunks{};
			for (auto x : vec) {
				if (!dbj::find(unique_chunks, x)) {
					unique_chunks.push_back(x);
				}
			}
			return unique_chunks;
		}
	}

	template <typename Type, size_t N >
	inline std::vector<Type> remove_duplicates(const Type(&arr_)[N], const bool sort = false) {
		return remove_duplicates(std::vector<Type>{arr_, arr_ + N}, sort);
	}

	/*
	------------------------------------------------------------------------------------
	*/
	inline auto string_pad(std::string s_, char padchar = ' ', size_t maxlen = 12) {
		return s_.insert(0, maxlen - s_.length(), padchar);
	};

	inline auto string_pad(int number_) {
		return string_pad(std::to_string(number_));
	};
#endif // 0


	/*
	------------------------------------------------------------------------------------
	*/
#ifdef DBJ_TESTING_ONAIR
	DBJ_TEST_UNIT(": quick_test_of_some_new_ NOT clever_nuggets ")
	{
		using namespace dbj::util;

			int intarr[]{ 1,1,2,2,3,4,5,6,6,6,7,8,9,9,0,0 };
			auto ret1 = remove_duplicates(intarr);
			std::string as2 [16] {
				"abra", "ka", "dabra", "babra",
				"abra", "ka", "dabra", "babra",
				"abra", "ka", "dabra", "babra",
				"abra", "ka", "dabra", "babra",
			};
	
			auto ad = remove_duplicates(as2);
			char carr[] = { 'c','a','b','c','c','c','d', 0x0 };
			remove_duplicates(carr);
#ifdef _DEBUG
			auto see_mee_here = carr;
			(void)see_mee_here;
#endif
		{
				// moved to dbj_util.h of dbj++
			auto doesit1 = starts_with("abra ka dabra", "abra"); 
			(void)doesit1;
			auto doesit2 = starts_with(L"abra ka dabra", L"abra"); 
			(void)doesit2;
		}
	}
#endif
} // dbj
