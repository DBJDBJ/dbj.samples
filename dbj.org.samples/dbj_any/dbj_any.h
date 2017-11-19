#pragma once

#pragma region dbj any with identity

namespace dbj {
	// hiden implementation
	namespace {
		/* this in essence makes std::any a real usefull container of anything */
		template <typename T>
		class Any final {
			mutable std::any value{};
		public:
			typedef T data_type;
			using type_ = Any;

			Any() {};

			Any(data_type data) : value(data) {	}

			data_type val() const {
				try {
					return std::any_cast<data_type>(this->value);
				}
				catch (std::bad_any_cast & x) {
					dbj::trace("function: %s. Exception at %s(%d) [%s]", __FUNCSIG__, __FILE__, __LINE__, x.what());
					throw dbj::Exception(x.what());
				}
			}

			bool empty() const {
				return !(this->value).has_value();
			}

		}; // Any
	} // nspace
	// the interface is two lambdas
	namespace any {
		auto make = [](auto value)
		{
			return dbj::Any<decltype(value)>(value);
		};

		auto val = [](const auto & dbj_any)
		{
			return dbj_any.val();
		};
	} // any

} // dbj

#ifdef DBJ_TESTING_EXISTS
namespace dbj_any_testing {

	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": value ptr ")) {
		auto me = dbj::any::make(42);
		auto v1 = dbj::any::val (me);
		// or
		auto v2 = me.val();
	}
}
#endif
#pragma endregion 

/* standard suffix */
#pragma comment( user, __FILE__ "(c) 2017 by dbj@dbj.org | Version: " __DATE__ __TIME__ ) 
/*
Copyright 2017 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
