#pragma once

#ifndef implements
#define implements public
#endif

namespace dbj {
	namespace docops {
		using string = std::string;
		/*
		__interface msvc keyword
		explained here: https://docs.microsoft.com/en-us/cpp/cpp/interface

		Operation type interface
		Operation type is not a document type
		*/
		__interface IOperation {
			/*
			Future extension: introduce operations on database stored documents
			enum class type { online, local, database };
			*/
			enum class type { online, local };
			bool open(string name);
		};

		typedef typename std::unique_ptr<IOperation> pointer;

		/*
		return or change/return the operation type
		this alows external configuration/setup
		see the main() bellow
		*/
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
			class LocalDocOp final : implements IOperation {
				bool open(string name) { return true; }
			};
			class OnlineDocOp final : implements IOperation {
				bool open(string name) { return true; }
			};
			/*
			Future extension:
			class DatabaseDocOp final : public IOperation {
			bool open(string name) { return true; }
			};
			*/
		}
		/* IOperation implementations are decoupled from users
			with the use of factory method bellow
		*/
		inline pointer operations_policy(
			/* if we call with no arguments the configured op type is made
				otherwise the argument is used to make a desired op type
			*/
			const IOperation::type & type_to_make = operations_type()
		) {
			/*	Future extension plan:
					if (operations_type() == IOperation::type::database)
				return make_unique<DatabaseDocument>(DatabaseDocument{});
			*/
			if (type_to_make == IOperation::type::local)
				return std::make_unique<LocalDocOp>(LocalDocOp{});

			return std::make_unique<OnlineDocOp>(OnlineDocOp{});
		}
} // docops

namespace documents {
	using string = std::string;
		/*
		IDocument interface describes the behaviour of documents
		Document hierarchy uses Documents Operations
		Types of documents do not equal type of operations on them
		thus we do not inherit from doc operations
		*/
		__interface IDocument {
			/*
			Future extension: introduce database stored documents
			enum class type { online, local, database };
			*/
			enum class type { online, local };
			bool open(string name);
		};
		/*
		Future expansion: introduce DataBase document
		That will not change the existing code bellow whatsoever.
		*/
		class Document : implements IDocument {
			/*	Types of documents do not equal type of operations on them
				thus we do not inherit from doc operations
				Document hierarchy uses Documents Operations
				through this docops pointer
			*/
			docops::pointer docops = docops::operations_policy();
		public:
			bool open(string name) { return docops->open(name); }
		};

		class TextDoc final : public Document {};
		class BinaryDoc final : public Document {};

		/* a single method to open any kind of a document
			polymorphic behaviour is OK here
		*/
		DBJ_INLINE int opendoc(Document & doc, string name) { return doc.open(name); }

	}
} // dbj

namespace {

/* use for CRTP and Delegation */
template<typename T> class Log_Service 
{
public:
	void log ( const T * client_ ) const noexcept {
	}
};

/* CRTP */
class App1 : Log_Service<App1> {
public:
	App1() {}
	void log() {
		// use the log service 
		// from the base
		Log_Service::log( this );
	}
};

/*	Delegation*/
class App2 {
	const Log_Service<App2> & log_svc_;
public:
	App2() : log_svc_(Log_Service<App2>()) {
	}
	void log() {
		// delegate to log 
		// service Behind
		log_svc_.log(this);
	}
};

#ifdef DBJ_TESTING_EXISTS
	namespace {
		DBJ_TEST_UNIT(" dbj delegation research") {
			App2 app_2 ;
			App1 app_1 ;
			app_1.log();
			app_2.log();
		}
	}
#endif
}
  /* standard suffix for every other header here */
#pragma comment( user, __FILE__ "(c) 2017 by dbj@dbj.org | Version: " __DATE__ __TIME__ ) 

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
  */
