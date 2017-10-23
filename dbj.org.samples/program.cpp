#include "stdafx.h"


#if 0 == __has_include(<functional>)
#include <functional>
#endif


#if 0
/*
  VS2017 Community edition (latest as of 2017-10-20), C++17 build
  Global space is poluted with 6 variables form corecrt_math.h

C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\ucrt\corecrt_math.h 
From line 980
_CRT_NONSTDC_DEPRECATE(_j0) _Check_return_ _ACRTIMP double __cdecl j0(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_j1) _Check_return_ _ACRTIMP double __cdecl j1(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_jn) _Check_return_ _ACRTIMP double __cdecl jn(_In_ int _X, _In_ double _Y);
_CRT_NONSTDC_DEPRECATE(_y0) _Check_return_ _ACRTIMP double __cdecl y0(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_y1) _Check_return_ _ACRTIMP double __cdecl y1(_In_ double _X);
_CRT_NONSTDC_DEPRECATE(_yn) _Check_return_ _ACRTIMP double __cdecl yn(_In_ int _X, _In_ double _Y);
*/
/* 
   if there is no namespace arround users declarations/definitions:

   error C2365: 'j0': redefinition; previous definition was 'function'
   ...
   error C2365: 'yn': redefinition; previous definition was 'function'
 */
namespace {
	constexpr auto j0 = true;
	constexpr auto j1 = true;
	constexpr auto jn = true;
	constexpr auto y0 = true;
	constexpr auto y1 = true;
	constexpr auto yn = true;
}

int main(int argc, char* argv[])
{
	/*
	if there is namespace arround users declarations/definitions above:

	error C2872: 'j0': ambiguous symbol
	...
	error C2872: 'yn': ambiguous symbol
	*/
		std::cout << j0 << j1 << jn << y0 << y1 << yn ;
	return true;
}
#endif

int main(int argc, char* argv[])
{
	dbj::testing::execute();
	return true;
}

#ifdef DBJ_TESTING_EXISTS
/* dbj type traits and enable if helpers */ namespace {
	using dbj::io::printex;

	template<typename T > 
	struct TypeValue {
		using type = T;
		T   value ;
		const char * name() const { return typeid(T).name(); }
		const char * my_name() const { return typeid(TypeValue).name(); }

		TypeValue (T v ) : value(v) { }

		friend DBJ_INLINE std::ostream&
			operator<<(std::ostream& out, const TypeValue & tv_) {
			return out << tv_.my_name() << " value type:" << tv_.name() << ", value: " << tv_.value;
		}
	};

	template<typename T, typename dbj::require_integral<T> = 0>
	DBJ_INLINE auto Object(T&& t) { return TypeValue<T>{t}; }

	template<typename T, typename dbj::require_floating<T> = 0>
	DBJ_INLINE auto Object(T&& t) { return TypeValue<T>{ t }; }   

	/*usage*/
	DBJ_TEST_CASE(dbj::nicer_filename(__FILE__)) {
		printex("\n", DBJ_NV(dbj::is_floating<float>()));
		printex("\n", DBJ_NV(dbj::is_integral<int>()));
		printex("\n", DBJ_NV(dbj::is_object<int>()));

		printex("\n", DBJ_NV(Object(42)));
		printex("\n", DBJ_NV(Object(42.0f)));
	}
} // namespace

/* dbj stlen and strnlen */ namespace  {
	using dbj::io::print;
	using dbj::io::printex;

	/*
	dbj crt caters for char, wchar_t, char16_t, char32_t
	for details please see https://docs.microsoft.com/en-us/cpp/cpp/char-wchar-t-char16-t-char32-t
	*/
	template< typename T, size_t N>
	DBJ_INLINE void strlen_strnlen_test( 
		const T (&prompt)[N]) 
	{
		printex("\n\nTesting array of type ", typeid(T).name() , " and of length ", N, "\t" );
		printex("\n\t", DBJ_NV(dbj::countof(prompt)));
		// char type arrays are using dbj.org "zero time" versions     
		printex("\n\t" , DBJ_NV(dbj::strlen(prompt)));
		printex("\n\t", DBJ_NV(dbj::strnlen(prompt, BUFSIZ)));

		// testing for the T * support 
		auto pointer_tester = [](auto cptr ) {
			// cptr becomes pointer due to the standard decay of C++ array to pointer
			using pointer_to_array = decltype(cptr);

			printex("\n\nTesting the support for the ", typeid(pointer_to_array).name(), " pointer to the same array\n");
			// using UCRT strlen
			printex("\n\t", DBJ_NV(dbj::strlen(cptr)));
			// using UCRT strnlen note: std has no strnlen ...
			printex("\n\t", DBJ_NV(dbj::strnlen(cptr, BUFSIZ)));
		};
		
			pointer_tester(prompt);
	}

	DBJ_TEST_CASE("dbj crt") {
		print("\n(c) % by dbj.org, MSVC version: %", YEAR , __VERSION__);
		
char	 promptA[]  =  "0123456789";
wchar_t  promptW[]  = L"0123456789";
char16_t prompt16[] = u"0123456789";
char32_t prompt32[] = U"0123456789";

strlen_strnlen_test(promptA);
strlen_strnlen_test(promptW);
strlen_strnlen_test(prompt16);
strlen_strnlen_test(prompt32);

assert( dbj::strnlen(promptA,  BUFSIZ) == 10 );
assert( dbj::strnlen(promptW,  BUFSIZ) == 10 );
assert( dbj::strnlen(prompt16, BUFSIZ) == 10 );
assert( dbj::strnlen(prompt32, BUFSIZ) == 10 );

	}

#if 0
	struct X final { };

	DBJ_TEST_CASE("dbj traits") {
		using xref = dbj::traits::Referee<X>;
		printex("\nFrom inside [", __func__, "]\t", DBJ_NV(typeid(xref).name()));
	}

	DBJ_TEST_CASE("dbj inheritance") {

		auto print_line = [](bool new_line = true, const size_t len_ = 80, const char chr_ = '-') {
			const std::string line_(len_, chr_);
			printex("\n", line_.data());
		};

		auto measure = [&print_line](auto object, const char * msg = "") -> void {
			print_line();
			printex("\n", msg, "\nType name:\t", typeid(object).name(),
				"\nSpace requirements in bytes",
				"\nType:\t\t", sizeof(decltype(object)),
				"\nInstance:\t", sizeof(object),
				"\nAllocation:\t", alignof(decltype(object))
			);
		};

		dbj::philology::HelloWorld<> hello{};
		dbj::philology::HelloWorld2<> hello2{};

		print("\nBEFORE RUN\n");
		measure(hello);
		measure(hello2);
		print_line();
		hello.run("\nHelloWorld -- Default policies");
		hello2.run("\nHelloWorld2 -- No inheritance");
		print_line();
		print("\nAFTER RUN\n");
		measure(hello);
		measure(hello2);
	};


	DBJ_TEST_CASE("dbj util")
	{
		auto f = []() -> std::tuple<int, int> // this function returns multiple values
		{
			int x = 5;
			return std::make_tuple(x, 7); // return {x,7}; in C++17
		};

		// heterogeneous tuple construction
		int n = 1;
		auto t = std::make_tuple(10, "Test", 3.14, std::ref(n), n);
		n = 7;
		auto[v1, v2, v3, v4, v5] = t;
		std::cout << "The value of t is " << "("
			<< v1 << ", " << v2 << ", "
			<< v3 << ", " << v4 << ", "
			<< v5 << ")\n";

		// function returning multiple values
		auto[a, b] = f();
		std::cout << a << " " << b << "\n";
	}

	DBJ_TEST_CASE("dbj util testing") 
	{
		using namespace dbj::util;
		auto fun = [](auto arg) -> void { cout << (arg);  };
		repeater(fun, "XYZ", true, 1, 0.2f);
		auto tupler = []() { return make_tuple(1, 2, 3); };
		// auto[ a, b, c] = tupler();
		auto whatever = make_from_tuple< decltype(tupler()) >(tupler());
		printex(whatever);
	}

	DBJ_TEST_CASE("dbj documents") {

		using IOperation = dbj::docops::IOperation;
		/* OPTIONAL: configure the docops to use online operations
		*/
		auto ot = dbj::docops::operations_type(IOperation::type::online);
		dbj::documents::TextDoc text;
		dbj::documents::opendoc(text, "world oyster");
	}

	DBJ_TEST_CASE("dbj tokenizer_test") {

		using dbj::io::print;
		const char * sentence = "abra % ka % dabra";
		dbj::fm::tokenizer tok(sentence, "%");
		print("Input sentence: [%]", sentence);
		for (auto w : tok) {
			print("\ntok[%] = [%]", w, tok[w]);
		}
		print("\n\n");
	}
#endif // 0
}

/* dbj experimental traits */ namespace {

	struct Struct final {
		void method() {};
	};

	/*
	type has signature and name

	this: void (Struct::*)() is a type signatue of Struct::method()
	or any method on Struct that returns void and has no arguments

	type is given name by using typedef or (in modern C++) using declaration

	typedef void (Struct::*methodP)(); 

	methodP is a type name of the type signature  void (Struct::*)()
	which is better looking from C++ 11 with "using" declaration

	using  methodP = void (Struct::*)(); // methodP is a type
	*/
	using  methodP = void (Struct::*)(); // methodP is a type
	using  fakeMP  = void (std::string::*)();

	/*
	dbj MemberPointer_traits<> requires a single template parameter
	which is a type of the method of a class or struct
	it can be given just by its signature of by its name
	*/

	/*
	  this function will exist only if give, at compile time, an type signatue or name 
	  of any class or struct
	  thus it will work for any class/struct and any method 
	  the only requirement is: T is a method on some class or struct
	*/
	template < typename T >
	constexpr bool is_method_invocable() {
		return std::is_invocable<T>::value;
	}

	auto tname = [](const auto & T) -> const char * { return typeid(decltype(T)).name(); };

	auto is_member = [](auto member) {
		return std::is_member_function_pointer<decltype(member)>::value;
	};
#if 0
	template <typename T>
	class has_begin
	{
		//typedef char one;
		//typedef long two;
		/*sizeof(test<T>(0)) == sizeof(char) if C::begin exist */
		//template <typename C> static one test(decltype(&C::begin));
		//template <typename C> static two test(...);

		//template <typename C, typename T                = void > struct tester { bool value = false; };
		//template <typename C, typename decltype(&C::begin) = 0 > struct tester { bool value = true; };
	public:
		
		using MPT = typename dbj::experimental_traits::MemberPointer_traits<T>::type;

		bool operator () () const {
			if constexpr( sizeof( T::begin ) ) {
				bool value = true;
			}
			else {
				bool value = false;
			}
		}
	};

	// https://stackoverflow.com/questions/42812829/why-doesnt-stdis-invocable-accept-a-non-type-template-parameter
	template< typename F, typename ...Args>
	auto callF(F function, Args...args) 
		-> typename std::enable_if<std::is_invocable_v<F, Args... > >  
	{
		std::invoke(function, std::forward<Args>(args)...);
	}
#endif
	DBJ_TEST_CASE("dbj experimental traits") {

		// printex("\n", DBJ_NV( tname<has_begin<methodP>::MPT>()), "\n");
		// printex("\n", DBJ_NV( tname<has_begin<fakeMP>::MPT>()), "\n");

		auto mp = dbj::functional::mem_fn(&Struct::method);

		auto mp2 = is_member(&Struct::method);
#if 0
		dbj::io::printex("\nMemeberPoint is not checking IF method does exist on a given class.\n",
			DBJ_NV(
				typeid(
					dbj::experimental_traits::MemberPointer_traits<void (std::string::*)()>::type
				).name()
			));
#endif
		auto mp3 = std::is_invocable< decltype(is_member), decltype(&tname) >::value ;

		
	}
}
#endif
// EOF