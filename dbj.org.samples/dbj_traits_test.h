#pragma once
#ifdef DBJ_TESTING_EXISTS
/* dbj type traits and enable if helpers */ 
namespace {
	using namespace dbj::win::con ;

	template<typename T, typename dbj::require_integral<T> = 0>
	DBJ_INLINE auto Object(T&& t) { return std::variant<T>(t); }

	template<typename T, typename dbj::require_floating<T> = 0>
	DBJ_INLINE auto Object(T&& t) { return std::variant<T>(t); }

	/*usage*/
	DBJ_TEST_CASE(dbj::nicer_filename(__FILE__)) {
		printex(
			"\n", DBJ_NV(dbj::is_floating<float>())
		);
		printex("\n", DBJ_NV(dbj::is_integral<int>()));
		printex("\n", DBJ_NV(dbj::is_object<int>()));

		printex("\n", DBJ_NV(Object(42)));
		printex("\n", DBJ_NV(Object(42.0f)));
	}
} // namespace
#endif // DBJ_TESTING_EXISTS