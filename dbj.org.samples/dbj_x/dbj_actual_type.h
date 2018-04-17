#pragma once
namespace dbj {

	template<typename T>
	struct is_pointer_pointer final {
		using arg_type = std::decay_t< T >;
		enum : bool {
			value =
			std::is_pointer_v<arg_type> &&
			std::is_pointer_v< typename std::remove_pointer_t<arg_type> >
		} ;
	};

	template<typename T>
	constexpr auto is_pointer_pointer_v = is_pointer_pointer<T>::value;

	auto is_p2p = [](auto arg) constexpr -> bool {
		using arg_type = std::decay_t< decltype(arg) >;
#if 0
		return std::is_pointer_v<arg_type> &&
			std::is_pointer_v< typename std::remove_pointer_t<arg_type> >;
#endif
		return is_pointer_pointer_v< arg_type >;
	};

	/// <summary>
	/// reveal the actual type of T
	/// </summary>
	template< typename T> struct actual_type {
		typedef typename std::decay_t< T > value_type;
		enum {
			is_lv_ref = std::is_lvalue_reference_v<T>,
			is_rv_ref = std::is_rvalue_reference_v<T>,
			is_pointer_ = std::is_pointer_v<T>,
			double_pointer = is_pointer_pointer_v<T> 
		};
	};
	
	/*
	template< typename T> struct actual_type<T*> {
		typedef typename std::decay_t< T > value_type;
		enum { is_pointer_ = true, double_pointer = false };
	};
	template< typename T> struct actual_type<T**> {
		typedef typename std::decay_t< T > value_type;
		enum { is_pointer_ = true, double_pointer = true };
	};
	*/

	// template alias helper for the above
	template<typename T>
	using actual_type_t = typename actual_type<T>::value_type;

	/// <summary>
	/// return the actual type description in json format
	/// </summary>
	template < typename T>
	inline auto show_actual_type = []() 
		-> std::string 
	{
		using at = actual_type< T > ;
		using actual_value_type = typename at::value_type;

		std::string tempo_{ BUFSIZ || 1024 }; // optimize so that heap mem alloc does not happen

		tempo_ = " { 'actual_type' : " ;
		tempo_.append(typeid(actual_value_type).name())
			.append(", 'is_lv_ref':").append(at::is_lv_ref ? "true" : "false")
			.append(", 'is_rv_ref':").append(at::is_rv_ref ? "true" : "false")
			.append(", 'pointer':").append(at::is_pointer_ ? "true" : "false")
			// .append(", 'single_pointer':").append(at::single_pointer ? "true" : "false")
			.append(", 'double_pointer':")
			.append(at::double_pointer ? "true" : "false")
			.append("}");

		return tempo_.data() ;
	};
} // dbj

#ifdef DBJ_TESTING_EXISTS
/*
 Reasoning, motivation and testing
*/
namespace {

	auto lambada = []( auto arg ) {
		using naive_type = decltype(arg);
		using the_type = dbj::actual_type_t< naive_type >;
		dbj::print("\nCalled with arg type:\t", typeid(naive_type).name());
		dbj::print("\n", dbj::show_actual_type<naive_type>());
	};

	auto lambada_complicada = []( const auto & arg) {
		using naive_type = decltype(arg);
		using the_type = dbj::actual_type_t< naive_type >;
		dbj::print("\nCalled with arg type:\t", typeid(naive_type).name());
		dbj::print("\n",dbj::show_actual_type<naive_type>());
	};


	DBJ_TEST_UNIT(" dbj actual type; motivation and testing ") {

		const char * argv{ "ONE" };

		 bool bubble1 = dbj::is_p2p( & argv );
		 bool bubble2 = dbj::is_p2p(argv[0] ) ;

		 bool bubble3 = dbj::is_pointer_pointer_v< decltype(&argv) >;
		 bool bubble4 = dbj::is_pointer_pointer_v< decltype( argv ) >;

		dbj::print("\n\n---------------------------------\nNow Calling lambada\n");
		lambada(argv);
		dbj::print("\n");
		lambada(&argv);

		dbj::print("\n\n---------------------------------\nNow Calling lambada complicada\n");
		lambada_complicada(argv);
		dbj::print("\n");
		lambada_complicada(&argv);

	}
}

#endif