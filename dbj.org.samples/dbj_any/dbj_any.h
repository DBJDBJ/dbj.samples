#pragma once

#include <variant>
#include <dbj_testing.h>

#pragma region dbj any with identity
namespace dbj {
	
	namespace any {

		using namespace std;

		inline auto data_store = [] (const auto * const new_val = 0 ) {
			static auto last_{ *new_val };
			if (new_val) last_ = *new_val;
			return last_;
		};

		template <typename T> class any_wrapper;
/*
		class base {
		public:
			// base of all visitors 
			class visitor {};
			// entrance for the visitor
			virtual void enter( visitor && ) const = 0 ;
		};

		class null_wrapper final
			: public base
		{
			void enter(visitor &&) const {}
		};
*/
	template <typename T>
	class any_wrapper final 
		// : public base
	{

		static_assert(!std::is_reference<T>::value, "[dbj::any_wrapper] Do not use a reference type");

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

		any_wrapper() {	}

		// give data
		any_wrapper( const data_type & ref) noexcept
			: any_(ref) 		{
		}

		any_wrapper(data_type && ref) noexcept 
			: any_( move(ref))	{
		}
		// copy
		any_wrapper(const any_wrapper& rhs) noexcept : any_ ( rhs.any_ ) { }
		any_wrapper& operator=(const any_wrapper& x) noexcept {
			if (this != &x) {
				this->any_ = x.any_; 
			}
			return *this;
		}
		// move
		any_wrapper(any_wrapper && rhs) : any_(move(rhs.any_)) {  }
		any_wrapper&& operator=(any_wrapper&& x) noexcept {
			if ( this != &x ) {
			this->any_ = move(x.any_); 
			}
			return std::forward<any_wrapper>( *this );
		}
		// destruct
		~any_wrapper() { this->any_.reset(); }

		// using the stored value in std::any
		// access
		operator data_type		 & () const noexcept = delete;
		operator data_type const & () const noexcept { return move(this->get());  }
		operator data_type		&& () && noexcept    { return move(this->get()) ; }

		// data_type && get()		const noexcept { return move(any_cast<data_type>(this->any_)); }

		// only if function is stored
		template< class... ArgTypes >
		invoke_result_t<T&, ArgTypes...>
			operator() (ArgTypes&&... args) const {
			if (!empty()) {
				return invoke(get(), forward<ArgTypes>(args)...);
			} 
			throw dbj::Exception(" can not call on empty data wrapped ");
		}

		data_type get () const {
			try {
				return any_cast<data_type>(this->any_);
			}
			catch (std::bad_any_cast & x) {
				throw dbj::Exception(x.what());
			}
		}

		bool empty() const {
			return !(this->any_).has_value();
		}
	};

		// factory method


		template <
			typename T,
			typename ANYW = typename dbj::any::any_wrapper< T >
		>
			inline auto make(T val_)
		{
			static_assert(!std::is_same<const char *, T>(),
				"std::any::make() can not use 'char *' pointer argument");

			return ANYW{val_};
		};
		/*
		inline auto make( const char * val_)
		{
			static_assert(! std::is_pointer<char *>(),
				"std::any::make() can not use 'char *' pointer argument");
			return dbj::any::any_wrapper< const char * >{val_};
		};
		*/
		// returns std::array of any wrappers
		// of the same type
		template <
			typename T,
			std::size_t N,
			typename ANYW = typename dbj::any::any_wrapper< T >
		>
			inline auto range(const T(&arrf)[N])
		{
			using   any_range = std::array< ANYW, N >;
			any_range rezult{};
			size_t j{ 0 };

			for (T element : arrf) {
				rezult[j++] = dbj::any::make(element);
			}
			return rezult;
		};

	} // any
} // dbj

namespace dbj::win::con {
	template< typename T>
	void out(const dbj::any::any_wrapper<T> & daaw_) {
		dbj::win::con::out(daaw_.get());
	}
}

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

	template< typename T, size_t N >
	inline auto range_test (const T(&arg_)[N] )
	{
		auto any_0 = dbj::any::range(arg_);
		dbj::print("\n", DBJ_NV(any_0[0].get()));
		return any_0;
	};

	DBJ_TEST_UNIT(": dbj any wrapper ") {

		try {
			dbj::print( "\n is a result of:\t", DBJ_NV(range_test({ 42 }))  );
			// NO CAN DO --> auto any_1 = range_test({ "Wot is this?" });

			char word_[] = "Hallo bre!";
			// yes can do
			auto    any_2 = dbj::any::make( word_);
			// NO CAN DO --> auto    any_3 = dbj::any::make( "Hallo bre!" );

			auto  v1 = any_2; // copy wrapper to wrapper
			auto  v2 = v1.get(); // wrapper to value and so on
		}	catch (...) {
			using namespace dbj::win::con;
			dbj::print(
				painter_command::text_color_reset,
				painter_command::bright_red,
				__FUNCSIG__ "  Unknown exception caught! ",
				painter_command::text_color_reset
				);
		}
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
