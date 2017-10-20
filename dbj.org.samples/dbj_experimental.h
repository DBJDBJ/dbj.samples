#pragma once
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
#define DBJVERSION __DATE__ __TIME__
  // #pragma message("-------------------------------------------------------------")
#pragma message( "--------------------> Compiled: " __FILE__ ", Version: " DBJVERSION)
  // #pragma message("-------------------------------------------------------------")
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
