#pragma once
namespace dbj {

#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	const auto  wargv_ = (__wargv);
	const auto	argv_ = (__argv);
	const auto	argc_ = (__argc);

#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	template<typename T, typename = std::enable_if_t< std::is_pointer<T>::value> >
	constexpr inline bool is_null(const T tp = 0)
	{
		return (tp != nullptr);
	}

	namespace {
		
		using wvecT = std::vector<std::wstring>;
		using nvecT = std::vector<std::string >;

		template <typename T, typename R> R cli_vec_(R * = 0 );

		template <>
		wvecT cli_vec_<std::true_type, wvecT>( wvecT * )
		{
			return wvecT{ wargv_, wargv_ + argc_ };
		}

		template <>
		nvecT cli_vec_< std::false_type, nvecT>(nvecT * )
		{
			return nvecT{ argv_, argv_ + argc_ };
		}
		// wargv_ !=  nullptr
		auto decide(std::true_type tt) {
			return wvecT{ wargv_, wargv_ + argc_ };
		}
		// wargv_ ==  nullptr
		auto decide( bool ft) {
			return nvecT{ argv_, argv_ + argc_ };
		}
	}

	auto cli_data = []() {
		
		    //typedef std::integral_constant<wchar_t **, wargv_> two_t;

			return decide( 
				wargv_ != NULL 
				? std::true_type{}
				: false
			);

			// return cli_vec_<  >();
	};
	
	class cli_type final {
		cli_type() = delete;
		cli_type(const cli_type &) = delete;
		cli_type & operator = (const cli_type &) = delete;
	public:
		using wvec_type = std::vector<std::wstring>;
		using nvec_type = std::vector<std::string >;
		wvec_type wvec{ wargv_, wargv_ + argc_ };
		nvec_type nvec{ argv_, argv_ + argc_ };

		static cli_type & object() {
			static cli_type data_{};
			return data_;
		};
	};
	/*
	https://stackoverflow.com/questions/47452748/how-to-decide-on-auto-return-type-at-run-time/47457592#47457592
	*/
	/*
	auto command_line_data = [&]() {
	if constexpr (wargv_pointer) {
	return std::vector<std::wstring>(wargv_, wargv_ + argc_);
	}
	else {
	return std::vector<std::string>(argv_, argv_ + argc_);
	}
	};
	*/
}
namespace {

	auto msvc_does_not_compile = [](auto _string)
		-> std::vector< decltype(_string) >
	{
		using string_type = decltype(_string);
		return std::vector<string_type>{};
	};

	auto msvc_does_not_compile_too = [](auto _string)
	{
		using string_type = decltype(_string);

		if constexpr (std::is_same<string_type, std::string>::value)
			return std::vector<std::string>;
		else {
			return std::vector<std::wstring>;
		}
	};

	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": dbj command line")) {

		auto the_cli_data = dbj::cli_data();

		// auto wargv_in_use = dbj::is_null(dbj::wargv_);
		// auto argv_in_use = dbj::is_null(dbj::argv_);

		// constexpr auto sol = std::addressof( std::add_const<decltype(dbj::wargv_)>() );

		// auto vec = basic_problem::msvc_does_not_compile( std::string{});
		// auto vec = basic_problem::msvc_does_not_compile_too(std::string{});
		// auto cli = dbj::command_line_data();
		// dbj::print( "\ndbj::cli.wvec", (dbj::cli.wvec) );
		// dbj::print( "\ndbj::cli.nvec", (dbj::cli.nvec) );
	}
}