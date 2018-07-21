// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

static char space[]{ " " };
static char line[]{ "------------------------------------------------------------" };


template< typename T> 
struct dbj_type final 
{
	template< typename TT>
	struct descriptor final 
	{
		using type = TT;
		constexpr   static inline const bool	is_pointer = std::is_pointer_v<TT>;
		constexpr 	static inline const bool	is_array = std::is_array_v<TT>;
		constexpr 	static inline const size_t  number_of_dimension = std::rank_v<TT>;
		constexpr 	static inline const size_t  first_extent = std::extent_v<TT>;
	};

	using def_type = descriptor<T>;
	using actual_type = descriptor<std::remove_cv_t< std::remove_pointer_t<T> >>;
	using under_type = descriptor<typename std::remove_all_extents<T>::type>;

	template<typename dbj_type>
	static std::string to_string( void ) noexcept
	{
		return 
		std::string{ "\ndefault type" } + 
		std::string { dbj_type::descriptor_to_string<dbj_type::def_type>( ) } +
		std::string{ "\nactual type" } +
		std::string { dbj_type::descriptor_to_string<dbj_type::actual_type>() } +
		std::string{ "\nunderlying type" } +
		std::string { dbj_type::descriptor_to_string<dbj_type::under_type>() };
	}

	template< typename descriptor >
	static std::string descriptor_to_string() noexcept
	{
		return DBJ::printf_to_buffer(
			"\n%-20s" 
			"\n%-20s : %s / %s"
			"\n%-20s : %s -- %zu"
			"\n%-20s : %s -- %zu",
			DBJ_TYPENAME(descriptor::type),
			space, (descriptor::is_pointer ? "Pointer" : "NOT Pointer"), (descriptor::is_array ? "Array" : "NOT Array"),
			space, "dimensions if array", descriptor::number_of_dimension,
			space, "dimension[0] size if array", descriptor::first_extent
		);
	}
};

#define DBJ_TYPE_REPORT_FUNCSIG(CON) \
		printf_to_console(CON, "\n%-20s : %s", "Function", __FUNCSIG__);\

template<typename T>
auto  dbj_type_report = []( dbj::console::IConsole * CON = nullptr )
{ 
		printf_to_console(CON, "\n%s", T::to_string<T>().c_str() );
};

/// <summary>
/// out overloads for outputing to IConsole instance
/// </summary>
namespace dbj::console {
	/*	the one and only, only the lonely ...etc ...	*/
template <typename T>
void out(T specimen)
{ 
	using argument_type = dbj_type<T> ;
	printf_to_console(&console_, "\n%s", line);
	DBJ_TYPE_REPORT_FUNCSIG(&console_);
	dbj_type_report<argument_type>( &console_ );
}

// template <typename T> void out( const T * specimen){	DBJ_TYPE_REPORT(&console_, decltype(specimen)); }

	template <typename T, size_t N>
	void out(T(&native_array)[N])
	{
		using argument_type = dbj_type<decltype(native_array)>;
		printf_to_console(&console_, "\n%s", line);
		DBJ_TYPE_REPORT_FUNCSIG(&console_);
		dbj_type_report<argument_type>( &console_);
		printf_to_console(&console_, "\n");

		if constexpr ( std::is_same_v<std::remove_cv_t<T>, char > ) {
			printf_to_console(&console_, "%-20s : %s", space, native_array);
		}
		else
		if constexpr (std::is_same_v<std::remove_cv_t<T>, wchar_t >) {
			printf_to_console(&console_, L"%-20S : %s", space, native_array);
		}
		else
		if constexpr (std::is_integral_v<T>) {
			printf_to_console(&console_, "%-20s : {", space );
			for (auto && elem : native_array) {
				printf_to_console(&console_, " %d ", elem);
			}
			printf_to_console(&console_, "} ");
		}
	}

	template <typename T, size_t N>
	void out( T(*native_array)[N])
	{
		printf_to_console(&console_, "\n%s", line);
		DBJ_TYPE_REPORT_FUNCSIG(&console_);
		out<T,N>(*native_array);
	}

}

// DBJ_TEST_SPACE_OPEN(console)

template<typename T>
struct actual final {
	using type = std::remove_cv_t< std::remove_pointer_t< T > >;
	constexpr inline static const type value{};
};

// DBJ_TEST_UNIT("console")
void dbj_test_console()
{
	using dbj::console::out;

	using A9 = dbj::arr::ARH<int, 9>;

	// native arrays out
	out(A9::ARR{ { 1,2,3,4,5,6,7,8,9 } });
	out(A9::to_arp(A9::ARR{ { 1,2,3,4,5,6,7,8,9 } }));
	out(A9::to_arf(A9::ARR{ { 1,2,3,4,5,6,7,8,9 } }));

	// pointers out
	out("R/O string literal");
	out(std::addressof(dbj::console::out<int>));

	const wchar_t(&& buf)[] { L"ABCDEFGHIJKLMNOPQR" };
	out(buf);
	out(buf[0]);
	out(&buf[0]);
	out(*buf);

	// out(dbj::range_to_string(buf));
}

// DBJ_TEST_SPACE_CLOSE
