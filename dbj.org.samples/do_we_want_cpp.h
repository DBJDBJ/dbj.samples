#pragma once

namespace dbj {
	namespace why {

		static inline int which(const int & arg) {
			return arg + 1;
		}

		// error C2365: 'dbj::why::which': redefinition; previous definition was 'function'
		// auto which = [](const int & arg) ->int { return arg + 1;  };

		template< typename T>
		static inline T which(const T & arg) {
			return arg + 1;
		}
	}
}
namespace {
	DBJ_TEST_CASE("") {

	}
}