#pragma once
namespace dbj {
	/*
	_ACRTIMP int*       __cdecl __p___argc (void);
	_ACRTIMP char***    __cdecl __p___argv (void);
	_ACRTIMP wchar_t*** __cdecl __p___wargv(void);
	*/
#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	const auto  wargv_ = (__wargv);
	const auto	argv_ = (__argv);
	const auto	argc_ = (__argc);
	/* also
	char***    __cdecl __p__environ (void);
	wchar_t*** __cdecl __p__wenviron(void);
	*/
	// (*__p__environ())  // Pointer to narrow environment table
	constexpr char  **  dbj_env() { return (*__p__environ()); }
	// (*__p__wenviron()) // Pointer to wide environment table
	constexpr wchar_t	**  dbj_wenv() { return (*__p__wenviron()); }
#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	template<typename T>
	constexpr bool is_null(const T const * tp = 0)
	{
		return (tp == nullptr);
	}


	constexpr auto wargv_in_use = is_null(dbj_wenv());

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

	// auto vec = basic_problem::msvc_does_not_compile( std::string{});
	// auto vec = basic_problem::msvc_does_not_compile_too(std::string{});

	dbj::print("\ndbj")("\tprint")("\tis")("\tfluent\n");

	// auto cli = dbj::command_line_data();
	// dbj::print( "\ndbj::cli.wvec", (dbj::cli.wvec) );
	// dbj::print( "\ndbj::cli.nvec", (dbj::cli.nvec) );
}