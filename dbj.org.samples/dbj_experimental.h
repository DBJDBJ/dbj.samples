#pragma once

namespace dbj::functional {
	// inspired with MSVC std lib
	// _Weak_types
	template<class Tpe_, class = void>
		struct Weak_Result_Type	{	// default definition
	};

	template<class Tpe_>
	struct Weak_Result_Type<Tpe_, void_t<
		typename Tpe_::result_type>
	> {	
		// defined if Tpe_::result_type exists
		using result_type = typename Tpe_::result_type;
	};

	template<class Tpe_, class = void>
		struct Weak_Argument_Type : Weak_Result_Type<Tpe_>
	{	// default definition
	};

	template<class Tpe_>
	struct Weak_Argument_Type<Tpe_, void_t <typename Tpe_::argument_type> >
		: Weak_Result_Type<Tpe_>
	{	// defined if Tpe_::argument_type exists
		using argument_type = typename Tpe_::argument_type;
	};

	template<class Tpe_,
		class = void>
		struct _Weak_binary_args
		: Weak_Argument_Type<Tpe_>
	{	// default definition
	};

	template<class Tpe_>
	struct _Weak_binary_args<Tpe_, void_t<
		typename Tpe_::first_argument_type,
		typename Tpe_::second_argument_type>>
		: Weak_Argument_Type<Tpe_>
	{	// defined if both types exist
		using first_argument_type = typename Tpe_::first_argument_type;
		using second_argument_type = typename Tpe_::second_argument_type;
	};
	template<class Tpe_>
	struct remove_pointer {	
		using type = Tpe_;
	};

	template<class Tpe_>
	using remove_pointer_t = typename remove_pointer<Tpe_>::type;

	template<class Tpe_>
	struct _Weak_types
	{	// provide nested types (sometimes)
		using _Is_f_or_pf = std::is_function<std::remove_pointer_t<Tpe_>>;
		using _Is_pmf = std::_Is_memfunptr <std::remove_cv_t<Tpe_>>;
		using type = std::conditional_t <_Is_f_or_pf::_Bool_type::value, _Is_f_or_pf,
			std::conditional_t<_Is_pmf::_Bool_type::value, _Is_pmf,
			_Weak_binary_args<Tpe_>>>;
	};

	/*
	  in here Member function is kind-of-a std::conditional_t
	*/
	template<class MemPTR>
	class Member_Function
		// : public _Weak_types<MemPTR>::type
	{	// wrap a pointer to member function/data
	private:
		MemPTR MemberPTR_;

	public:
		explicit Member_Function(MemPTR _Val) noexcept
			: MemberPTR_(_Val)
		{	// construct, storing a pointer to member function/data
		}

		template<class... _Types>
		auto operator()(_Types&&... _Args) const
			-> decltype(std::invoke(MemberPTR_, std::forward<_Types>(_Args)...))
		{	// invoke a pointer to member function/data
			return (std::invoke(MemberPTR_, std::forward<_Types>(_Args)...));
		}
	};

	template<class _Rx,	class Tpe_> 
	inline
		Member_Function<_Rx Tpe_::*> mem_fn(_Rx Tpe_::* _Pm) noexcept
	{	// wrap a pointer to member function/data
		return (Member_Function<_Rx Tpe_::*>(_Pm));
	}
}

namespace dbj { 
	namespace experimental {
		
		template <typename T> T identity(T);
		/* used without actual function existence
		struct X{};
		using vx = std::vector<identity(X)> ;
		*/

			class Experimental final
			{
			public:
				explicit constexpr Experimental() {}
				~Experimental() {}
			};

template<std::size_t N>
inline auto ordered_int_sequence(const std::size_t(&sequence)[N]) {

	for (size_t j = 0; j != N; ++j) sequence(j) = j;
	return sequence;
}

/*
consider this:

for ( n : sequence ) auto val = sequence[n] ;

What is val? Val is the value held on the n'th position inside the sequence.
But what if you do not want the value but the n'th index of this sequence?
Then you use me little Indicator class.

std::vector<std::string> svec = {"A","B","C"} ;

Indicator<std::vector<std::string>> indices{ svec } ;

for ( auto j : indices ) cout << j << " " ; // prints 0 1 2

*/

template<
	typename OBSERVED_SEQUENCE_TYPE,
	int N
>
class Indicator final {
public:
	using WEAK_PTR = std::weak_ptr<OBSERVED_SEQUENCE_TYPE>;
	using SHRD_PTR = std::shared_ptr<OBSERVED_SEQUENCE_TYPE>;

protected:

	size_t indices_[N] = { 0 };

	// std::vector<size_t> indices_ ;

	SHRD_PTR observed_;
	WEAK_PTR weak_ptr_;

	/* we do not know when and if observed sequence changes
	therefore we recompute the indices
	*/
	void assure() {

		weak_ptr_ = observed_;
		// observedSP is shared pointer to the observed sequence
		if (auto observedSP = weak_ptr_.lock()) {
			auto observed_sequence = *observedSP;
			// Indicator * THIS = const_cast<Indicator*>(this);
		}
		else {
			throw std::runtime_error("observed sequence has expired");
		}
	}

	// forbidden
	Indicator() : observed_(OBSERVED_SEQUENCE_TYPE()) { /* no op */ }

public:

	template<typename T, std::size_t N>
	static auto make(T & t_) {
		return Indicator<T, N>(t_);
	}

	explicit Indicator(OBSERVED_SEQUENCE_TYPE  & another)
		: observed_(std::make_shared<OBSERVED_SEQUENCE_TYPE>(another))
	{
		assure();
		ordered_int_sequence(indices_);
	}

	~Indicator()
	{
		observed_.reset();
	}


	auto operator = (OBSERVED_SEQUENCE_TYPE & another) {
		this->observed_ = std::make_shared<OBSERVED_SEQUENCE_TYPE>(another);
		assure();
		ordered_int_sequence(indices_);
		return (*this);
	}

	auto begin() {
		return std::begin(indices_);
	}

	auto end() {
		return std::end(indices_);
	}

	const auto at(size_t pos_)  const
	{
		assure();
		assert(this->begin() == 0);
		assert(this->end() > pos_);
		return indices_[pos_];
	}
private:
};

} // experimental
} // dbj

namespace dbj {
	namespace experimental_traits {

		template<class... T> struct type_list { std::tuple<T...> args = std::make_tuple<T...>(); };

		template<typename T> struct tname { const char * name = typeid(T).name; };

		/* usage:
		using mp_trait = MemberPointer_traits<decltype(&T::begin)>
		mp_trait::type if exists
		is the type of the pointer to the method T::begin
		*/
		template<typename>
		struct MemberPointer_traits {};

		template<class T, class U>
		struct MemberPointer_traits<U T::*> {
			using type = U;

		};
		/*
		Referee<T> requires T to be of an class type
		*/
		template<typename T, typename std::enable_if<std::is_class<T>::value>::type * = 0 >
		struct Referee
		{
			const T  & value_default{};
			using self_type = Referee;
			using value_reftype = std::reference_wrapper< std::remove_cv_t<T> >;
			using value_type = T;
			value_reftype reference = std::cref(value_default);
		};


		struct lama final { const char * name = "Alpaka"; };

		//#define DBJ_NV( symbol) "[", STR(symbol), "] :-> [" , symbol , "]"

		DBJ_TEST_CASE("Testing dbj experimental_traits") {
			using lamaref = Referee<lama>;
			dbj::io::printex("\n", __func__, "\t", DBJ_NV(typeid(lamaref).name()));
		}

	} // traits
} // dbj
#define DBJVERSION __DATE__ __TIME__
#pragma message( "--------------------> Compiled: " __FILE__ ", Version: " DBJVERSION)
#pragma comment( user, "(c) 2017 by dbj@dbj.org | Version: " DBJVERSION ) 
#undef DBJVERSION
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
