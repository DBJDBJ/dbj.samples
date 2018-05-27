#include "stdafx.h"

// https://medium.com/@LoopPerfect/c-17-vs-c-14-if-constexpr-b518982bb1e2

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

	// does T supports some method 

	template<class T>
	constexpr auto supportsAPI() 
		-> decltype(
			& T::Method1, 
			& T::Method2,
			true_type
			)
	{
		
		return {};
	}

	template<class T>
	constexpr auto supportsAPI () -> false_type
	{
		return {};
	}
	
	//usage
	template<class T>
	int compute(T x) {
		if constexpr(supportsAPI<T>()) {
			// only gets compiled if the condition is true
			return x.Method1();
		}
		else {
			return 0;
		}
	}

	static int few_more_nuggets() {
		struct X final {
			void Method1() {}
			void Method2() {}
		};

		using wot = decltype( & X::Method1 );

		auto isthis = typeid(wot).name();

		return compute(X{});
	}

	static auto rezult = few_more_nuggets();
}