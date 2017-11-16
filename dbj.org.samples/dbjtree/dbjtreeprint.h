#pragma once
#if 0
#include <string>
#endif

#include <stdarg.h>
#include <sstream>

namespace dbj {
	namespace conv {
		template <typename T>
		inline std::wstring	& to_wstring(T & t) {
			static std::wstring retval_; // prolong the life of returned value
			std::wostringstream out ;
			out << t;
			retval_.clear(); 
			retval_.append( out.str() ); //copy
			return retval_;
		}

		inline std::wstring	& to_wstring(std::wstring & t) {
			static std::wstring retval_; // prolong the life of returned value
			retval_.clear();
			retval_.append(t.data()); //copy
			return retval_;
		}
	}
}

namespace dbj {
	namespace treeprint {
		namespace {
			constexpr wchar_t doubles[] = { L"║═╚" };
			constexpr wchar_t singles[] = { L"│─└" };
			constexpr wchar_t SS = L' '; // single space
			constexpr int V{ 0 }; 
			constexpr int H{ 1 }; 
			constexpr int K{ 2 };
			/*
			we have nicked the following form corecrt_wstdio.h

			_Success_(return >= 0)
			_Check_return_opt_
			_CRT_STDIO_INLINE int __CRTDECL _snwprintf_s(
			_Out_writes_opt_(_BufferCount) _Always_(_Post_z_) wchar_t*       const _Buffer,
			_In_                                              size_t         const _BufferCount,
			_In_                                              size_t         const _MaxCount,
			_In_z_ _Printf_format_string_                     wchar_t const* const _Format,
			...)
			#if defined _NO_CRT_STDIO_INLINE
			;
			#else
			{
			int _Result;
			va_list _ArgList;
			__crt_va_start(_ArgList, _Format);
			_Result = _vsnwprintf_s_l(_Buffer, _BufferCount, _MaxCount, _Format, NULL, _ArgList);
			__crt_va_end(_ArgList);
			return _Result;
			}

			*/

			template< size_t N>
				inline int dbj_snwprintf_s
				(
				wchar_t        (&_Buffer)[N],
				size_t         const _BufferCount,
				size_t         const _MaxCount,
				wchar_t const* const _Format, 
					...)
			{
				int _Result;
				::va_list _ArgList;
				__crt_va_start(_ArgList, _Format);
				_Result = _vsnwprintf_s_l(_Buffer, _BufferCount, _MaxCount, _Format, NULL, _ArgList);
				__crt_va_end(_ArgList);
				return _Result;
			}

			template <typename ... Args>
			inline void bufprint( std::wstring & buff_ , wchar_t const * const format, Args ... args) noexcept
			{
				/* NOTE: 512 happpens to be the BUFSIZ */
				wchar_t buffer[BUFSIZ*2] = {};
				size_t buffer_size = dbj::countof(buffer);
				assert(-1 != dbj_snwprintf_s(buffer, buffer_size -1, buffer_size -1, format, (args) ...));
				buff_.append(buffer);
			}

			/*
			Print the binary tree made of NODE's
			NODE must have three methods:
			NODE * left  ();
			NODE * right ();
			NODE::DataType * data  ();
			Where NODE::DataType requirement is to be unicode output streamable

			The actual alorithm inspired by: http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/
			*/
			constexpr size_t BUFSIZ_ = { 2056 };

			class BinaryTreePrinter final {

				wchar_t depth[BUFSIZ_] = {};
				int di = int(0);

				void Push(wchar_t c)
				{
					// depth[di++] = L' ';
					depth[di++] = c;
					depth[di++] = L' ';
					depth[di++] = L' ';
					depth[di] = 0;
				}

				void Pop()
				{
					// 3? why 3?
					depth[di -= 3] = 0;
				}
				/*
				this is the only data memeber
				*/
				mutable std::wstring out_buf_{ BUFSIZ };
			public:
				BinaryTreePrinter() {}

				~BinaryTreePrinter(){out_buf_.clear();}

				BinaryTreePrinter(BinaryTreePrinter&&) = default;
				BinaryTreePrinter & operator = (BinaryTreePrinter&&) = default;

				/*
				starting from an given node print into and return the result in an wstring
				*/
				template<typename NODE>
				std::wstring operator ()(NODE * tree, const int outwid_ = 0)
				{
					print_(tree, outwid_);
					out_buf_.append(L"\n");
					return std::wstring{ out_buf_.data() }; // return the copy
				}
				/* save it for return and then flush the local output buffer */
				auto flush() {
					auto retval{ this->out_buf_.data() }; // return the copy
					out_buf_.clear();
					return retval;
				}
			private:
				// print the tree into the local buffer
				template<typename NODE>
				std::wstring  & print_(NODE* tree, const int outwid_)
				{
					if (tree) {
						assert(tree->data());
						// .. works only for strings
						bufprint(
							out_buf_, L"{%*s}\n", outwid_,  dbj::conv::to_wstring(*(tree->data()))
						);
					}
					if (tree->left()) {
						bufprint(out_buf_, L"%s%c\n", depth, singles[V]);
						bufprint(out_buf_, L"%s%c%c%c", depth, singles[K], singles[H], singles[H]);
						Push(singles[V]);
						print_(tree->left(), outwid_); // recurse left
						Pop();
					}
					if (tree->right()) {
						// this->out_buf_ << depth << singles[V] << L"\n";
						// this->out_buf_ << depth << singles[K] << singles[H] << singles[H];
						bufprint(out_buf_, L"%s%c\n", depth, singles[V]);
						bufprint(out_buf_, L"%s%c%c%c", depth, singles[K], singles[H], singles[H]);
						Push(SS);
						print_(tree->right(), outwid_);
						Pop();
					}

					return this->out_buf_;
				}

			}; // eof BinaryTreePrinter
		} // namespace

		template <typename TREE>
		inline void binary_tree_to_string ( std::wstring & out_, TREE * root_ , unsigned outwid = 0 )
		{
			BinaryTreePrinter tpf{};
			 out_.append( tpf(root_, outwid) );
		}
#if 0
		void test2()
		{
		/*

		 http://stackoverflow.com/questions/2492077/output-unicode-strings-in-windows-console-app

		 */
			static std::wstring doubles = L"║═╚";
			static std::wstring singles = L"│─└";
			_setmode(_fileno(stdout), _O_U16TEXT);
			std::wcout << L"Testing unicode -- English -- Ελληνικά -- Español." << std::endl
				<< doubles << std::endl
				<< singles << std::endl;

			/* 
			  apparently must do this
			  also make sure NOT to mix cout and wcout in the same executable
			*/
			std::wcout << std::flush; 
			fflush(stdout); 
			_setmode(_fileno(stdout), _O_TEXT); 
		}
#endif
	} //namespace treeprint 
} //namespace dbj 
#pragma comment( user, "(c) 2017 by dbj@dbj.org :: " __FILE__ " :: " __DATE__ " :: " __TIME__ ) 
  /*
  Copyright 2017 dbj@dbj.org

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
