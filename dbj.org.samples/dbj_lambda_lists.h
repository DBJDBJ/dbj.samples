#pragma once

#pragma region LAMBDA LISP

namespace dbj {
	/*
	The key abstraction : list lambda
	*/
	auto llist = [&](auto ...xs) {
		/*return value is lambda */
		return [=](
			/* whose single argument is lambda */
			auto proc_lambda
			) {
			/* which is called with variadic arguments parameter pack
			from the list() call stack
			*/
			return proc_lambda(xs...);
		};
	};
		// ll == "list lambdas" or "lambda lists"
		namespace ll {

		/*
		to understand it all:

		auto l123 = list(1,2,3) // returns lambda --> [=](auto proc_lambda) { return proc_lambda(xs...); };

		head(l123) //  head() is given that lambda as argument
		inside head() calls into that lambda -- see it now above once more
		thus head() does -- l123()
		agument to l123() is lambda defined in the call to it as argument
		(!critical detail here!) that lambda receives xs... that is saved on the
		argument stack of the list() call previously -- see the list() above

		now try to follow all of this in the debuger
		*/
		auto head = [&](auto list_lambda) {
			return list_lambda(
				/*
				this is given as proc_lambda() to the result lambda od the list()
				arguments are the original variadic arguments  of the list()
				*/
				[=](auto first, auto ...rest) {
				/*
				we return list of one element so that users of head() and tail()
				can use  then both in a symetrical manner
				see the usage of print() for example
				*/
				return llist(first);
			}
			);
		};

		// returns list() lambda
		auto tail = [&](auto list_lambda) {
			return list_lambda([=](auto first, auto ...rest) { return llist(rest...); });
		};

		auto length = [&](auto list_lambda) {
			return list_lambda([=](auto ...z) { return sizeof...(z); });
		};

		/* dbj added 
		   use with namespace adornment
		*/
		auto tuple = [&](auto list_lambda) {
			// 
			return list_lambda(
				[=](auto ...args) {
				return std::make_tuple(args...);
			}
			);
		};
} // list 
} // dbj

DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": lambda lists")) {
	// usage
	auto my_list = dbj::llist(1, '2', "3", false, 13.0f); // return lambda() internal to list()
	auto my_head = dbj::ll::head(my_list); // returns list of one element -- list(1)
	auto my_tail = dbj::ll::tail(my_list); // returns list('2', "3", false, 13.0f)
	/*
	after playing with lambda lists we print what we need to print
	we use tuple() because dbj::print() knows how to print touples
	but not lambda lists
	*/
	dbj::print("\nlist: ", dbj::ll::tuple(my_list));
	dbj::print("\nhead: ", dbj::ll::tuple(my_head));
	dbj::print("\ntail: ", dbj::ll::tuple(my_tail));

	// dbj::print("\n", DBJ_NV( length(list()) ));
}
#pragma endregion 