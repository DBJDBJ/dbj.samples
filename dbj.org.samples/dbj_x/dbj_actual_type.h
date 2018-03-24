#pragma once
namespace dbj {
	/// <summary>
	/// reveal the actual type of T
	/// </summary>
	template< typename T> struct actual_type {
		typedef typename std::decay_t< T > value_type;
		enum { is_pointer_ = false, single_pointer = false, double_pointer = false };
	};
	template< typename T> struct actual_type<T*> {
		typedef typename std::decay_t< T > value_type;
		enum { is_pointer_ = true, single_pointer = true, double_pointer = false };
	};
	template< typename T> struct actual_type<T**> {
		typedef typename std::decay_t< T > value_type;
		enum { is_pointer_ = true, single_pointer = false, double_pointer = true };
	};

	template < typename T>
	inline auto show_actual_type = []() {
		using at = actual_type< T > ;
		using actual_value_type = typename at::value_type;
		//constexpr bool is_it_char{ is_same_v<args_type, char> };
		//constexpr bool is_it_wchar{ is_same_v<args_type, wchar_t> };
		dbj::print("\nactual type of argument:", typeid(actual_value_type).name(),
			"\nis actual type a pointer:", (at::is_pointer_ ? "true" : "false"),
			"\nis actual type a single pointer:", (at::single_pointer ? "true" : "false"),
			"\nis actual type a bouble pointer:", (at::double_pointer ? "true" : "false")
		);
	};
} // dbj