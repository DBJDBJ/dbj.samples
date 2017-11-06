#pragma once
#ifdef DBJ_TESTING_EXISTS
/* dbj type traits and enable if helpers */ 
namespace {
	// using namespace dbj::win::con ;

	template<typename T, typename dbj::require_integral<T> = 0>
	DBJ_INLINE auto Object(T&& t) { return std::variant<T>(t); }

	template<typename T, typename dbj::require_floating<T> = 0>
	DBJ_INLINE auto Object(T&& t) { return std::variant<T>(t); }

	/*usage*/
	DBJ_TEST_CASE(dbj::nicer_filename(__FILE__)) {
		dbj::print(
			"\n", DBJ_NV(dbj::is_floating<float>())
		);
		dbj::print("\n", DBJ_NV(dbj::is_integral<int>()));
		dbj::print("\n", DBJ_NV(dbj::is_object<int>()));

		dbj::print("\n", DBJ_NV(Object(42)));
		dbj::print("\n", DBJ_NV(Object(42.0f)));
	}
} // namespace
#endif // DBJ_TESTING_EXISTS