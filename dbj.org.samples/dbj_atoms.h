#pragma once
#define _ENABLE_ATOMIC_ALIGNMENT_FIX
#include <atomic>

namespace dbj::nuclear {

#ifdef DBJ_TESTING_EXISTS

	namespace {
		struct Counters { int a; int b; };      // user-defined trivially-copyable type

		DBJ_TEST_CASE(__FILE__) {
			std::atomic<Counters> cnt{};         // specialization for the user-defined type

			Counters c1 = { 1, 2 };
			Counters c2 = { 3, 4 };

			cnt.store(c1);

			cnt.load();

		}
	}

#endif // DBJ_TESTING_EXISTS

}