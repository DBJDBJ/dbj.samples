// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

DBJ_TEST_SPACE_OPEN(dbj_traits_testing)

template<typename T, typename dbj::require_integral<T> = 0 >
inline auto Object(T&& t) { return std::variant<T>(t); }

template<typename T, typename dbj::require_floating<T> = 0>
inline auto Object(T&& t) { return std::variant<T>(t); }

/*usage*/
DBJ_TEST_UNIT(" dbj basic traits tests") {

	DBJ_TEST_ATOM(dbj::is_floating<decltype(42.0f)>());
	DBJ_TEST_ATOM(dbj::is_integral<decltype(42u)>());
	DBJ_TEST_ATOM(dbj::is_object<decltype(42u)>());

	DBJ_TEST_ATOM(Object(42));
	DBJ_TEST_ATOM(Object(42.0f));
}

//-----------------------------------------------------------------------------
// RC == micro Range and Container
// I like it. A lot.
template< typename T, std::size_t N>
struct range_container final {
using data_ref = T(&)[N];
T data_[N]{};
T * begin() { return data_; }
T * end() { return data_ + N; }
size_t size() const { return N; }
data_ref data() const { return data_; }
};

DBJ_TEST_UNIT(" dbj container traits tests")
{
	using namespace std;

	using ia3 = array<int, 3>;
	using vi = vector<int>;

	DBJ_TEST_ATOM(dbj::is_std_array_v<ia3>);
	DBJ_TEST_ATOM(dbj::is_std_array_v<vi>);

	DBJ_TEST_ATOM(dbj::is_std_vector_v<ia3>);
	DBJ_TEST_ATOM(dbj::is_std_vector_v<vi>);

	DBJ_TEST_ATOM(dbj::is_range_v<ia3>);
	DBJ_TEST_ATOM(dbj::is_range_v<vi>);

	auto is_it_range = dbj::inner::is_range< range_container<int, 3> >::value ;
	// bellow wont work because comma operator screws macros in a royal way
	// DBJ_TEST_ATOM(dbj::inner::is_range<range_container<int,3>>::value);
}


DBJ_TEST_SPACE_CLOSE(dbj_traits_testing)

