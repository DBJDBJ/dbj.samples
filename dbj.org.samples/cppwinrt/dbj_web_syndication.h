#pragma once
#include <dbj_testing.h>
/*
https://moderncpp.com/2017/11/01/cppwinrt-in-the-windows-sdk/
https://kennykerr.ca/2018/03/09/cppwinrt-producing-async-objects/
*/
#pragma comment(lib, "windowsapp")
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Web.Syndication.h>

#ifdef CPPWINRT_VERSION
/*
DBJ: be very carefull not to have namespace named "winrt"
*/
namespace dbj::wrt {

	constexpr const wchar_t * feed_uri[]{
		L"https://kennykerr.ca/feed",
		L"https://dbj.org/feed"
	} ;

	using namespace winrt;
	using namespace Windows::Foundation;
	using namespace Windows::Web::Syndication;

	inline IAsyncAction MainAsync()
	{
		Uri uri( feed_uri[1] );
		SyndicationClient client;
		SyndicationFeed feed = co_await client.RetrieveFeedAsync(uri);

		for (auto&& item : feed.Items())
		{
			hstring title	= item.Title().Text();
			hstring summary = item.Summary().Text();

			dbj::print("\nTitle:\t", title.data(), "\nSummary:\t", summary.data());
		}
	}

#ifdef DBJ_TESTING_EXISTS
	DBJ_TEST_UNIT(": dbj winrt web syndication ")
	{
		// hm? 
		winrt::init_apartment();
		//
		MainAsync().get();
	}
#endif
}
#endif