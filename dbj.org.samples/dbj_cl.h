#pragma once
namespace dbj {

	template<typename T, typename = std::enable_if_t< std::is_pointer<T>::value> >
	constexpr inline bool is_null(const T tp = 0)
	{
		return (tp != nullptr);
	}


#define _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	const auto  wargv_ = (__wargv);
	const auto	argv_ = (__argv);
	const auto	argc_ = (__argc);

#undef _CRT_DECLARE_GLOBAL_VARIABLES_DIRECTLY

	namespace {
		
		using wvecT = std::vector<std::wstring>;
		using nvecT = std::vector<std::string >;

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
		
			static auto cli_vector = decide( 
				wargv_  
				? std::true_type{}
				: false
			);
			return cli_vector;
	};

}
namespace {
	
	auto msvc_does_compile = [](auto _string)
	{
		static_assert(
			! std::is_same<decltype(_string), std::string>::value  ||
			! std::is_same<decltype(_string), std::wstring>::value
			, "\n\nstatic assert failure in:\t" __FUNCSIG__ "\n\n An unsupported type string argument!\n\n");

		using string_type = decltype(_string);
		return std::vector<string_type>{};
	};

	DBJ_TEST_CASE(dbj::FILELINE(__FILE__, __LINE__, ": dbj lambda return type")) {

		dbj::print("\n _MSC_FULL_VER ", _MSC_FULL_VER);

		// auto the_cli_data = dbj::cli_data();

		dbj::print("\n\n",DBJ_NV(dbj::cli_data()));
		dbj::print("\n\n", DBJ_NV(typeid(dbj::cli_data()).name()));

		dbj::print("\n\nwargv_ is",dbj::is_null(dbj::wargv_) ? "" : " not " ," NULL");

		dbj::print("\n\nargv_ is",dbj::is_null(dbj::argv_) ? "" : " not "," NULL");

		bool wot = ( dbj::wargv_ != NULL ? true : false );

		// auto vec1 = ( dbj::wargv_ ? msvc_does_compile(std::wstring{}) : msvc_does_compile(std::string{}) );
	}
}
#if 0
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
auto command_line_data = [&]() {
	if constexpr (wargv_pointer) {
		return std::vector<std::wstring>(wargv_, wargv_ + argc_);
	}
	else {
		return std::vector<std::string>(argv_, argv_ + argc_);
	}
};

// http://en.cppreference.com/w/cpp/language/auto
auto does_not_compile_msvc = [](bool wide)
{
	if (wide)
		return std::vector<std::string>();

	return std::vector<std::wstring>();
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
/*
no common type in this case so '::type' will be undefined

using string_type = std::common_type<std::wstring , std::string>::type;
*/

#endif