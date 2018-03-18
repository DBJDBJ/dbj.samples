#include "stdafx.h"
#include <dbj_util.h>



#if 1
namespace {
	using namespace std;
	/*
	http://www.open-std.org/pipmail/ub/2016-February/000565.html
	*/
	/// <summary>
	/// C++ "stunt" which perhaps is not. It shows the good usage of std artefacts
	/// based on SFINAE. And it is suffciently limiting function not to be labelled a "stunt"
	/// Both types have to be :
	///   -- of the same size
	///   -- trivially copyable
	///   -- and destructible
	/// This also shows extremely efficient compile-time coding of course.
	/// And. It this is an example where raw pointer are most efficient mechanism
	/// No copying and no moving is invoved here ...
	/// </summary>
	template<typename T, typename U>
	static T * change_object_type(U *p) {
		static_assert(sizeof(T) == sizeof(U));
		static_assert(is_trivially_copyable_v<T> && is_trivially_copyable_v<U>);
		char buff[sizeof(T)];
		memcpy(buff, p, sizeof(T));
		p->~U();
		T *result = new (p) T;
		memcpy(result, buff, sizeof(T));
		return result;
	}

	/// <summary>
	/// Encapsulation of the usage of  change_object_type
	/// </summary>
	/// <param name="f"> float to be magically transformed to int</param>
	/// <returns>the result of the "magic"</returns>
	static int magic_int_float_transformation(float f) {
		/** float is dead, long live the int! */
		int *i = change_object_type<int>(&f); 
		return *i;
	}

	/// <summary>
	/// Illustrating the use of Arrays and Functions in C
    /// https://www.cs.uic.edu/~jbell/CourseNotes/C_Programming/Arrays.html
    /// </summary>
	namespace {

		typedef double numarr[];

		// Finds max in the array
		extern "C" static double maxArray(const numarr numbs, int arraySize);

		extern "C"  static int Illustrating_the_use_of_Arrays_and_Functions(void) {

			// here we use the typedef in declaration
			numarr array1{ 10.0, 20.0, 100.0, 0.001 };

			// we can not use the same single dim array in declaring two dim array
			double array2[2][3]{ 
				{ 5.0, 10.0, 20.0 },
				{ 8.0, 15.0, 42.0 } 
			};

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

		/// <summary>
		/// Function to find the maximum in an array of doubles
		/// Note the use of the keyword "const" to prevent changing array data
		/// </summary>
		/// <param name="numbs">array argument </param>
		/// <param name="arraySize">user defined size of the array</param>
		/// <returns></returns>
		extern "C" static double  maxArray(const numarr numbs, int arraySize) {

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
	} //ns
} // ns
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
	/// <summary>
	/// tuple is collection of different types
	/// vector elementes are all of the same type
	/// there is no much point transforming vector to tuple
	/// </summary>
	static void test_vector_to_touple() {
		vector<int> vint{ 1, 2, 3 };
			auto tpl0 = dbj::util::seq_tup<vector<int>,3>(vint);

			int vintage []{ 1, 2, 3 };
			auto tpl1 = dbj::util::seq_tup(vintage);
	}

#ifdef DBJ_TESTING_EXISTS
	DBJ_TEST_UNIT(": quick_local_tests ")
	{
		test_vector_to_touple();

		auto r1 = dbj::sign(+2.0f);
		auto r2 = dbj::sign(-2.0f);
		auto r3 = dbj::sign(0.0f);

		auto r4 = std::signbit(+42.0f);

		// int I = magic_int_float_transformation(42.42f);
		// Illustrating_the_use_of_Arrays_and_Functions();
	}
#endif