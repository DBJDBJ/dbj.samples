#pragma once

#ifdef DBJ_TESTING_EXISTS

#include "dbjtree\dbjtree.h"
#include "dbjtree\dbjtreeprint.h"

namespace {

	/*
	An example of what some are calling 'comfortable API'
	*/
	DBJ_TEST_CASE("simple binary tree test")
	{
		using std::wstring;
		typedef typename dbj::tree::BinaryNode<wstring> BinaryNodeT;

		auto rooty = BinaryNodeT::make_node(L"ROOT");
		(*rooty) << L"Minnie" << L"Moonnie" << L"Tiny" << L"Shiny" << L"Galaxy";
		rooty->insert(L"And", L"Variadic", L"Insert", L"Also", L"Available");
		std::wstring wos;
		dbj::treeprint::binary_tree_to_string(wos, rooty.get());
		dbj::print("\n", wos, "\n");
	}
	/*
	Test on an example tree of BinaryNode<wstring>
	*/
	DBJ_TEST_CASE("binary tree visitors test")
	{
		using std::wstring;
		constexpr wchar_t nl{ L'\n' };

		/*
		this lambda is our visitor processor
		it can be any lambda, function or functor that receives pointer
		to BinaryNode<> instance and returns bool.
		false	retval from the processor will stop the processing.
		*/
		auto printer = [](auto x) {
			DBJ_ASSERT(x);
			DBJ_ASSERT(x->data());
			std::wstring buf;
			 dbj::treeprint::bufprint(
				buf, L"[%*s]", 9,  dbj::conv::to_wstring(*(x->data()))
			);
			dbj::print("\n", buf);
			return true;

		};


		typedef typename dbj::tree::BinaryNode<int> BinaryNodeT;
		auto root = BinaryNodeT::make_node(0);

		(*root) << 1 << 2 << 3 << 4 << 5;

		root->insert(10, 9, 8, 7, 6);

		dbj::print("\nProcess then visit left and right: ");
		preorder(root.get(), printer);
		dbj::print("\nVisit left, Process then  visit right: ");
		inorder(root.get(), printer);
		dbj::print("\nVisit left, visit right then Process: ");
		postorder(root.get(), printer);

		dbj::print("\n");
		std::wstring wos;
		dbj::treeprint::binary_tree_to_string(wos, root.get());
		dbj::print(wos, "\n");
	}

} // testing anon space
#endif // DBJ_TESTING_EXISTS

#define DBJVERSION __DATE__ __TIME__
#pragma comment( user, "(c) 2017 by dbj@dbj.org code, Version: " DBJVERSION ) 
#undef DBJVERSION