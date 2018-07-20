// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../stdafx.h"

DBJ_TEST_SPACE_OPEN(console)

template<typename T>
struct actual final {
	using type = std::decay_t< std::remove_pointer_t< T > >;
	constexpr static T value{};
};

DBJ_TEST_UNIT("console")
{
	using dbj::console::print;

	char buf[]{"ABCDEFGHIJKLMNOPQR"};
	DBJ_TEST_ATOM(actual< decltype(buf) >::value);

	print(buf);
	print(&buf);
	print(buf[0]);
	print(&buf[0]);
	print(*buf);

	print(dbj::range_to_string(buf));
	print(dbj::range_to_wstring(buf));
	print(dbj::range_to_u16string(buf));
	print(dbj::range_to_u32string(buf));
}

DBJ_TEST_SPACE_CLOSE
