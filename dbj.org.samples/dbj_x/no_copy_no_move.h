#pragma once

#include <dbj++.h>

namespace dbj::samples {

	/// <summary>
	/// inherit as private base this class
	/// to make the offspring(s) 
	/// no copy
	/// no move
	/// dbj.org created 
	/// </summary>
	class __declspec(novtable) no_copy_no_move
	{
	protected:
		no_copy_no_move() = default;
		virtual ~no_copy_no_move() {};
	private:
		// no copy
		no_copy_no_move(const no_copy_no_move&) = delete;
		no_copy_no_move& operator=(const no_copy_no_move&) = delete;
		// no move
		no_copy_no_move(no_copy_no_move&&) = delete;
		no_copy_no_move& operator=(no_copy_no_move&&) = delete;
	};

	DBJ_TEST_SPACE_OPEN(local_tests)

	struct D;
	extern const D not_d;
	struct D
	{
		enum class state_code : char { non_initialized ='?', constructed = '!'};
		state_code state{ state_code::non_initialized };
		// D() = delete;
		explicit D (state_code new_payload_ = state_code::constructed ) : state(new_payload_) {}
		D( const D & other_) : _ref_( other_ ), state( other_.state) {  }
		D & operator = (D  other_) { std::swap(state, other_.state);  return *this; }
		std::reference_wrapper<const D> _ref_{ not_d }; // OK
	};

	inline const D not_d ;

	DBJ_TEST_UNIT( member_ref_to_its_host ) {
		
		D d1, d2, d3;

		d1 = d2 = d3;

		auto s1 = d1.state;

	}

	struct A;
	extern const A not_a ;
	struct A
	{
		const A& a_ref_{ not_a }; // OK
		char payload{ '?' };
		A() {}
		A( const A & other_ ) : 
			a_ref_(other_ ), payload((char)other_.payload) {  }
		A & operator = (A  other_) { std::swap(payload, other_.payload);  return *this; }
	};
	
	inline const A not_a{ };                  // OK

	DBJ_TEST_UNIT(no_copy_no_move) {

		A a_1;  a_1.payload = '*';
		A a_2{ a_1 };
		A a_3{ a_2 };

		a_1 = a_2;

		auto r1 = a_1.payload;

		auto r3 = a_1.a_ref_.a_ref_.a_ref_.payload;
	}

	DBJ_TEST_SPACE_CLOSE
}