#pragma once

#include <iostream>

namespace {

	inline void string_literals_with_universal_character_names() {

		auto out = []( auto s_ ) {
			std::cout << s_ << std::endl;
		};
		/*
		https://docs.microsoft.com/en-us/cpp/cpp/string-and-character-literals-cpp
		*/
		// ASCII smiling face  
		const char*     s1 = ":-)";

		// UTF-16 (on Windows) encoded WINKING FACE (U+1F609)  
		const wchar_t*  s2 = L"😉 = \U0001F609 is ;-)";

		// UTF-8  encoded SMILING FACE WITH HALO (U+1F607)  
		const char*     s3 = u8"😇 = \U0001F607 is O:-)";

		// UTF-16 encoded SMILING FACE WITH OPEN MOUTH (U+1F603)  
		const char16_t* s4 = u"😃 = \U0001F603 is :-D";

		// UTF-32 encoded SMILING FACE WITH SUNGLASSES (U+1F60E)  
		const char32_t* s5 = U"😎 = \U0001F60E is B-)";

		out(s1);
		out(s2);
		out(s3);
		out(s4);
		out(s5);

	}
}