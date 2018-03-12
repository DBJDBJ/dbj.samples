#pragma once
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

	using namespace winrt;
	using namespace Windows::Foundation;
	using namespace Windows::Web::Syndication;

	inline IAsyncAction MainAsync()
	{
		Uri uri(L"https://kennykerr.ca/feed");
		SyndicationClient client;
		SyndicationFeed feed = co_await client.RetrieveFeedAsync(uri);

		for (auto&& item : feed.Items())
		{
			hstring title = item.Title().Text();

			printf("%ls\n", title.c_str());
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