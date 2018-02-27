#pragma once

#include <variant>

#pragma region dbj any with identity
namespace dbj {
	
	namespace any {
		auto data_store = [] (const auto * const new_val = 0 ) {
			static auto last_{ *new_val };
			if (new_val) last_ = *new_val;
			return last_;
		};
	}

	template <typename T>
	class any_wrapper final {
#if 0		
		template <typename T>
		struct data {
			typedef T data_type;
			static_assert(!std::is_reference<T>::value, "[dbj::any_wrapper] Do not use a reference type");
			mutable std::any any_{};
			T & get() const noexcept { return std::any_cast<data_type>(this->any_); }
		};

		template<typename T>
		auto data_store (const std::optional<T> new_val = std::nullopt ) {
			static data<T> last_{ new_val };
			if (new_val) last_.any_ = *new_val;
			return last_;
		};

		// with data
		template <typename T>
		any_wrapper(const T & ref) noexcept {
			data_store<T>(ref);
		}

		template <typename T>
		any_wrapper(T && ref) noexcept {
			data_store<T>(move(ref));
		}
#endif
		std::any any_{};

	public:
		// types
		typedef any_wrapper type;
		typedef T data_type;

		any_wrapper() {
		}

		// give data
		any_wrapper( const data_type & ref) noexcept
			: any_(ref)
		{
		}

		any_wrapper(data_type && ref) noexcept 
			: any_(std::move(ref))
		{
		}
		// copy
		any_wrapper(const any_wrapper& rhs) noexcept { this->any_ = rhs.any_ ; }
		any_wrapper& operator=(const any_wrapper& x) noexcept { this->any_.swap(x.any_); return *this; }
		// move
		any_wrapper(any_wrapper && rhs) { this->any_.swap(rhs.any_); }
		any_wrapper&& operator=( any_wrapper&& x) noexcept { this->any_.swap(x.any_); return *this; }
		// destruct
		~any_wrapper() { this->any_.reset(); }

		// using the stored value in std::any
		// access
		operator data_type		 & () const noexcept { return std::any_cast<data_type>(this->any_); }
		operator data_type const & () const noexcept { return std::forward<data_type>(std::any_cast<data_type>(this->any_)); }
		operator data_type		&& () && noexcept    { return std::move(std::any_cast<data_type>(this->any_)); }

		data_type & get()		const noexcept { return std::any_cast<data_type>(this->any_); }

		// only if function is stored
		template< class... ArgTypes >
		std::invoke_result_t<T&, ArgTypes...>
			operator() (ArgTypes&&... args) const {
			return std::invoke(get(), std::forward<ArgTypes>(args)...);
		}

		data_type val() const {
			try {
				return std::any_cast<data_type>(this->any_);
			}
			catch (std::bad_any_cast & x) {
				throw dbj::Exception(x.what());
			}
		}

		bool empty() const {
			return !(this->any_).has_value();
		}

		// factory method
	};

	namespace any {
		// factory method
		auto make = [](const auto & value , const auto & ... args)
		{
			using   namespace std;
			constexpr
				size_t argsnum = sizeof...(args);
			using   data_type = decay_t<decltype(value)>;
			using	ANYW		= dbj::any_wrapper< data_type > ;
			using   any_range   = vector< ANYW >;

			any_range rezult{}; rezult.resize(1 + argsnum);
			rezult.push_back(ANYW{ value });

			if ( argsnum > 0) {
				char dumzy[argsnum] = { ((rezult.push_back(ANYW{args}))  ,0)... };
			}

			return rezult;
		};


	} // any
} // dbj
#if 0
namespace dbj {
	// hiden implementation
	namespace {
		/* this in essence makes std::any a real usefull container of anything */
		template <typename T>
		class Any final {
			static_assert(!std::is_reference<T>::value, "Do not use a reference type");
			mutable std::any value{};
		public:
			typedef T data_type;
			typedef Any type ;

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
} // dbj
#endif
#ifdef DBJ_TESTING_EXISTS
namespace dbj_any_wrapper_testing {

	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": dbj any wrapper ")) {
		auto me = dbj::any::make(42,32) ;
		auto  v1 = me[0]; // copy wrapper to wrapper
		const int &  v2 = v1; // wrapper to value and so on
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
