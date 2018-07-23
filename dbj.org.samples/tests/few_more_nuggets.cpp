// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

// https://medium.com/@LoopPerfect/c-17-vs-c-14-if-constexpr-b518982bb1e2

// does T supports some method popular solution does not work?
struct X final {
	void A(int) {
		DBJ::TRACE("\n\nIn %s\n\n", __func__);
	}
	void B(float) {
		DBJ::TRACE("\n\nIn %s\n\n", __func__);
	}
};

namespace {

	using namespace std;

	// get the n-th

	template<unsigned n>
	struct nth_arg final {

		template<typename X, typename ... Args>
		constexpr auto operator()(X x, Args ... xs) {
			if constexpr(n > sizeof ... (xs)) {
				return;
			}
			else if constexpr(n > 0) {
				return nth_arg<n - 1>{}(xs...);
			}
			else {
				return x;
			}
		}
	};

	template<class T>
	constexpr auto supportsAPI( ) -> decltype(&T::begin, &T::end, true_type{}) {
		return {};
	}

	template<class T>
	constexpr auto supportsAPI( ) -> false_type {
		return {};
	}
	
	//usage
	template<typename T>
	auto compute( const T & x) {
		
		std::string name{ DBJ_TYPENAME(T) };
		
		if constexpr(
			supportsAPI<T>()
		) 
		{
			DBJ::TRACE("\n\n%s has method 'begin()'\n\n", name.c_str()  ) ;
			return x.begin();
		}
			DBJ::TRACE("\n\n%s has no method 'begin()'\n\n", name.c_str() );
			return 0 ;
	}

	DBJ_TEST_UNIT(" few_more_nuggets")
	{
		decltype(auto) wot_1 = DBJ_TEST_ATOM(
			typeid(  void (std::string::*) (int) ).name() 
			);

		auto specimen = string{ "0123456789" };
		decltype(auto) wot_2 = 
			DBJ_TEST_ATOM(
				compute(specimen)
				);
	}
}