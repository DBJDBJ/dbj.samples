#pragma once
/*
inspired by improving the example code here
http://en.cppreference.com/w/cpp/utility/tuple/make_tuple
*/

#include <iostream>
#include <tuple>
#include <functional>

namespace dbj {
	namespace util {

		inline std::tuple<int, int> f() // this function returns multiple values
		{
			int x = 5;
			return std::make_tuple(x, 7); // return {x,7}; in C++17
		}

		namespace {
			inline void test()
			{
				// heterogeneous tuple construction
				int n = 1;
				auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
				n = 7;
				/*
				std::cout << "The value of t is " << "("
					<< std::get<0>(t) << ", " << std::get<1>(t) << ", "
					<< std::get<2>(t) << ", " << std::get<3>(t) << ", "
					<< std::get<4>(t) << ")\n";
				*/
				auto[v1, v2, v3, v4, v5] = t;
				std::cout << "The value of t is " << "("
					<< v1 << ", " << v2 << ", "
					<< v3 << ", " << v4 << ", "
					<< v5 << ")\n";

				// function returning multiple values
				/*
				int a, b;
				std::tie(a, b) = f();
				*/
				auto[a, b] = f();
				std::cout << a << " " << b << "\n";
			}

			auto dumsy = dbj::testing::add(test);
		}
	} // util
} // dbj