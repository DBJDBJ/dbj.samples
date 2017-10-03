#pragma once
/*
Copyright 2017 by dbj@dbj.org

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

__interface msvc keyword
explained here: https://docs.microsoft.com/en-us/cpp/cpp/interface
*/
#ifndef _MSC_VER
#error This code requires Visual C++ 
#endif // !_MSC_VER
#if _MSC_VER < 1911
#error This code requires Visual C++ 14.1 or better
#endif

#define implements public

#include <dbjio.h>
using dbj::io::print;

#include <memory>
#include <string>
using namespace std;


namespace dbj {
	namespace docops {

		// https://docs.microsoft.com/en-us/cpp/cpp/interface
		__interface IOperation {
			/*
			operation type is not a document type
			Future extension plan: introduce database stored documents
			enum class type { online, local, database };
			*/
			enum class type { online, local };
			bool open(string name);
		};

		typedef typename unique_ptr<IOperation> pointer;

		// return or change/return the type 
		// this alows to involve external config
		// see the main() bellow
		inline IOperation::type operations_type(
			const IOperation::type & the_type_ = IOperation::type::online
		)
		{
			static IOperation::type default_type = IOperation::type::online;
			static IOperation::type current_type = IOperation::type::online;
			return the_type_ != current_type ? current_type = the_type_ : current_type;
		}

		// IOperation implementations are completely hidden from users
		namespace {
			class LocalDocument final : public IOperation {
				bool open(string name) { return true; }
			};
			class OnlineDocument final : public IOperation {
				bool open(string name) { return true; }
			};
			/*
			Future extension plan:
			class DatabseDocument final : public IOperation {
			bool open(string name) { return true; }
			};
			*/
		}
		// the above is decoupled from users with the factory method bellow
		inline pointer operations_policy() {
			if (operations_type() == IOperation::type::local)
				return make_unique<LocalDocument>(LocalDocument{});
			/*
			Future extension plan:
			if (operations_type() == IOperation::type::database)
			return make_unique<DatabaseDocument>(DatabaseDocument{});
			*/
			return make_unique<OnlineDocument>(OnlineDocument{});
		}
	} // docops

	namespace documents {
		/*
		Document hierarchy uses Documents Operations
		types of documents do not equal type of operations on them
		Future expansion: introduce DataBase operations
		But do not change the code bellow whatsoever.
		*/
		class Document {
			docops::pointer docops = docops::operations_policy();
		public:
			bool open(string name) { return docops->open(name); }
		};
		class TextDoc final : public Document {};
		class BinaryDoc final : public Document {};

		inline int opendoc(Document & doc, string name) { return doc.open(name); }

		inline void test() {
			/* OPTIONAL
			   configure the docops to use online operations 
			*/
			auto ot = docops::operations_type(docops::IOperation::type::online);
			TextDoc text;
			opendoc(text, "world oyster");
		}
	}
} // dbj
