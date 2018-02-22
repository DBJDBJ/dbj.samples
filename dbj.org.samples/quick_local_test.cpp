#include "stdafx.h"

namespace {
	static int magic_int_float_transformation(float f);
	static int Program_Illustrating_the_use_of_Arrays_and_Functions(void);
	static void test_vector_to_touple();
	}

extern "C" void quick_local_tests(decltype(dbj::print) & print)
{
	test_vector_to_touple();

	int I = magic_int_float_transformation(42.42f);
	Program_Illustrating_the_use_of_Arrays_and_Functions();
}


namespace {
	using namespace std;
	/*
	http://www.open-std.org/pipmail/ub/2016-February/000565.html
	*/

	template<typename T, typename U>
	inline T *
		change_object_type(U *p) {
		static_assert(sizeof(T) == sizeof(U));
		static_assert(is_trivially_copyable_v<T> && is_trivially_copyable_v<U>);
		char buff[sizeof(T)];
		memcpy(buff, p, sizeof(T));
		p->~U();
		T *result = new (p) T;
		memcpy(result, buff, sizeof(T));
		return result;
	}

	static int magic_int_float_transformation(float f) {
		int *i = change_object_type<int>(&f); // float is dead, long live the int!
		return *i;
	}
/*
Program Illustrating the use of Arrays and Functions
https://www.cs.uic.edu/~jbell/CourseNotes/C_Programming/Arrays.html
*/
	using namespace std;

	typedef double numarr[];

	// Finds max in the array
	static double maxArray(const numarr numbs, int arraySize);

	static int Program_Illustrating_the_use_of_Arrays_and_Functions(void) {

		numarr array1{ 10.0, 20.0, 100.0, 0.001 };
		double array2[2][3]{ { 5.0, 10.0, 20.0 },{ 8.0, 15.0, 42.0 } };

		int sizes[2] = { 4, 3 };
		double max1, max2, max3, max4, max5;

		max1 = maxArray(array1, 4);
		max2 = maxArray(array1, sizes[0]);
		// max3 works becuse of how arrays are
		// laid out in memory
		// max3 is thus sum of the second row
		// of the array2
		max3 = maxArray(array2[1], 3);
		// max4 also works becuse of how arrays are
		// laid out in memory
		// max4 is thus sum of the whole array2
		max4 = maxArray(array2[0], 6);
		max5 = maxArray(array1, -4);

		dbj::print("\nMaximums are ", max1, ", ", max2, ", ", max3
			, ", ", max4, ", ", max5);

		return 0;
	}

	/*
	Function to find the maximum in an array of floats
	Note the use of the keyword "const" to prevent changing array data
	*/
	static double  maxArray(const numarr numbs, int arraySize) {

		int i;
		double max;

		if (arraySize <= 0) {
			return 0.0;
		}

		max = numbs[0];

		for (i = 1; i < arraySize; i++)
			max = (numbs[i] > max) ? numbs[i] : max;

		return max;

	}

	template <typename T, size_t... Indices>
	auto vectorToTupleHelper(const vector<T>& v, index_sequence<Indices...>) {
		return std::make_tuple(v[Indices]...);
	}
	template <std::size_t N, typename T>
	auto vectorToTuple(const std::vector<T>& v) {
		assert(v.size() >= N);
		return vectorToTupleHelper(v, std::make_index_sequence<N>());
	}
	/*
	dbj vector to touple optimization of  https://stackoverflow.com/posts/28411055/
	*/
	namespace dbjx {

		using namespace std;

		template<class T>
		struct is_vector : std::false_type {
		};

		template<class T>
		struct is_vector<std::vector<T> > {
			static bool const value = true;
		};

		auto vectorToTuple = [] (const auto & v)
		{
			constexpr auto ok = is_vector<decltype(v)>::value;

			// template <typename T, size_t... Indices>
			auto helper = [] (const auto & v, auto ... Indices ) {
				return std::make_tuple(v[Indices]...);
			};

			return helper(v, 0, 1, 2);
		};
	}

	// as per http://graphics.stanford.edu/~seander/bithacks.html#CopyIntegerSign
	auto dbj_sign = [](const auto & v) constexpr -> int {
		return (v > 0) - (v < 0); // -1, 0, or +1
	};

	static void test_vector_to_touple() {

		auto r1 = dbj_sign(+2.0f);
		auto r2 = dbj_sign(-2.0f);
		auto r3 = dbj_sign(0.0f);

		auto r4 = std::signbit(+42.0f);

		auto tp0 = dbjx::vectorToTuple(std::vector<int>{1,2,3});
	}
}
