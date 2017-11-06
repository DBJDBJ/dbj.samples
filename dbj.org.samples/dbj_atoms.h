#pragma once
#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#include <atomic>

/*
 It seems std::atomic + MSVC are not  ready yet? The simple code bellow throws a lot of exceptions 
 like this one:

 Exception thrown at 0x7461B872 in dbj.org.samples.exe: Microsoft C++ exception: std::out_of_range at memory location 0x00EFEA3C.
*/
namespace dbj::nuclear {

#ifdef DBJ_TESTING_EXISTS

	namespace {

		template<typename T>
		auto  nested_init_list_args ( const std::initializer_list< std::initializer_list< T >> & arg ) {
			dbj::print("\n",__func__, "\t", DBJ_NV(typeid(arg).name()));
		};

		struct Counters { int a; int b; };      // user-defined trivially-copyable type

		DBJ_TEST_CASE(__FILE__) {
			std::atomic<Counters> cnt{};         // specialization for the user-defined type

			Counters c1 = { 1, 2 };
			Counters c2 = { 3, 4 };

			cnt.store(c1);
				auto c3 = cnt.load();
				cnt.exchange(c2);
				auto c4 = cnt.load();

				nested_init_list_args({ {1,2} });
				nested_init_list_args({ { 1,2 },{3,4} });
		}
	}

#endif // DBJ_TESTING_EXISTS

}