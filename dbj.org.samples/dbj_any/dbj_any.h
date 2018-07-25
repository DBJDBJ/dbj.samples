#pragma once

#include <any>
#include <variant>
#include <dbj++.h>


namespace dbj::samples {
	
	namespace any {

		using namespace std;
		template <typename T> class any_wrapper;

	template <typename T>
	class any_wrapper final 
	{
		static_assert(!std::is_reference<T>::value,
			"[dbj::any_wrapper] Can not use a reference type");

		std::any any_{};

	public:
		// types
		typedef any_wrapper type;
		typedef T data_type;

		any_wrapper() noexcept {	}

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
		any_wrapper(any_wrapper && rhs) noexcept : any_(move(rhs.any_)) {  }

		any_wrapper& operator=(any_wrapper&& x) noexcept {
			if ( this != &x ) {
			this->any_ = move(x.any_); 
			}
			return *this ;
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

		constexpr data_type get () const {
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

		const std::string to_string () const {
			const auto val_ = this->get();
			if constexpr( std::is_arithmetic<decltype(val_)>() ) {
				return { std::to_string(val_) };
			}

			if constexpr(std::is_integral<decltype(val_)>()) {
				return { std::to_string(val_) };
			}
#pragma warning( push )
#pragma warning( disable: 4312 )
			return { reinterpret_cast<const char *>(val_) };
// #pragma message("### DBJ --> Warning 4312 disabled in file: " __FILE__ ) 
#pragma warning( pop )
		}

		operator std::string () const  {
			return
				std::string{ "dbj::any::any_wrapper<" }
			.append(typeid(T).name())
			.append(" = ")
			.append(this->to_string())
			.append(">");
		}
	};

		// factory methods ----------------------------------------

		template <
			typename T,
			typename ANYW = typename any::any_wrapper< T >
		>
			inline auto make(T val_)
			-> ANYW
		{
			static_assert(!std::is_same<const char *, T>(),
				"std::any::make() can not use 'char *' pointer argument");

			return ANYW{val_};
		};

		// returns std::array of any_wrapper's
		// of the same type T
		template <
			typename T,
			std::size_t N
		>
		inline auto range(const T(&arrf)[N])
		{
			using ANYW = any::any_wrapper< T >;
			using RETT = std::array< ANYW, N >;
			using std_arr_t = RETT; 
			
			std_arr_t rezult{};
			std::size_t j{ 0 };

			for (auto element : arrf) {
// warning C28020: The expression '0<=_Param_(1)&&_Param_(1)<=1-1' is not true at this call.
// why ?
// calling the 'operator =' on dbj any_wrapper, so ... ?
				rezult[j++] = any::make(element);
			}
			return rezult;
		};

	} // any
} // dbj::samples

DBJ_TEST_SPACE_OPEN( dbj_any_wrapper_testing )

	DBJ_TEST_UNIT( dbj_any_wrapper) 
    {

		using namespace dbj::samples;
		try {
			int int_arr[]{42};
			auto any_0 = ( any::range(int_arr) );
			// NO CAN DO --> auto any_1 = range_test({ "Wot is this?" });

			// yes can do
			char word_[] = "Hallo bre!";
			auto    any_2 = ( any::make( word_) ) ;
			// NO CAN DO --> auto    any_3 = dbj::any::make( "Hallo bre!" );

			auto  v1 = any_2; // copy wrapper to wrapper
			auto  v2 = ( v1.get() ); // wrapper to value and so on
		}	catch (...) {
			dbj::log::print( dbj::Exception(
				__FUNCSIG__ "  Unknown exception caught! "
				));
		}
	}
DBJ_TEST_SPACE_CLOSE

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
