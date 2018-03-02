#include "stdafx.h"
#include <dbj_util.h>



#if 1
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
}
#endif
#if 0
	template <typename T, size_t... Indices>
	auto vectorToTupleHelper(const vector<T>& v, index_sequence<Indices...>) {
		return std::make_tuple(v[Indices]...);
	}
	template <std::size_t N, typename T>
	auto vectorToTuple(const std::vector<T>& v) {
		assert(v.size() >= N);
		return vectorToTupleHelper(v, std::make_index_sequence<N>());
	}
#endif

	static void test_vector_to_touple() {
		vector<int> vint{ 1, 2, 3 };
			auto tpl0 = dbj::util::seq_tup<vector<int>,3>(vint);

			int vintage []{ 1, 2, 3 };
			auto tpl1 = dbj::util::seq_tup(vintage);
	}

	extern "C" void quick_local_tests(decltype(dbj::print) & print)
	{
		test_vector_to_touple();

		auto r1 = dbj::sign(+2.0f);
		auto r2 = dbj::sign(-2.0f);
		auto r3 = dbj::sign(0.0f);

		auto r4 = std::signbit(+42.0f);

		// int I = magic_int_float_transformation(42.42f);
		// Program_Illustrating_the_use_of_Arrays_and_Functions();
	}
