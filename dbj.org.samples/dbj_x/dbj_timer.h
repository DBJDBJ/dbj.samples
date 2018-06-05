#pragma once
/*
Taken form UntiTest++ then modified by dbj.org
This is WIN32 implementation
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifndef _ASSERTE
#include <crtdbg.h>
#endif
#include <cstdint>

#ifndef implements
#define implements public
#endif

namespace dbj_samples {

#if defined(_WIN64)		
	typedef std::uint64_t			time_ticks_type;
	typedef std::uint64_t			frequency_type;
#else
	typedef std::uint32_t			time_ticks_type;
	typedef std::uint32_t			frequency_type;
#endif
	/// <summary>
	/// https://stackoverflow.com/questions/23374153/convert-double-to-int-in-c-without-round-down-errors
	/// </summary>
	/// <param name="ull_"></param>
	/// <returns>rounded to time_ticks_type</returns>
	inline time_ticks_type round_to_tt_type(unsigned long long  ull_) {
		return static_cast<time_ticks_type>(ull_ < 0 ? ull_ - 0.5 : ull_ + 0.5);
	}


	/// <summary>
	/// win32_timer_engine Interface 
	/// </summary>
	struct ITimer
	{
		virtual time_ticks_type	start() = 0;
		virtual time_ticks_type	elapsed() = 0;
	};

	namespace internal {

		class  win32_timer_engine final
		{
		public:
			win32_timer_engine()
				: threadHandle(::GetCurrentThread())
				, startTime(0)
			{
				_ASSERTE(threadHandle);

				DWORD_PTR systemMask;
				::GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemMask);
				::SetThreadAffinityMask(threadHandle, 1);
				::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
				::SetThreadAffinityMask(threadHandle, processAffinityMask);
			}

			time_ticks_type start() {
				return this->startTime = get_time();
			}

			time_ticks_type elapsed() const {
				time_ticks_type const elapsedTime = get_time() - startTime;
				time_ticks_type const seconds = 
					round_to_tt_type( elapsedTime / frequency );
				return seconds * 1000 ;
			}

		private:
			time_ticks_type	get_time() const
			{
				_ASSERTE(threadHandle);
				LARGE_INTEGER curTime{};
				::SetThreadAffinityMask(threadHandle, 1);
				::QueryPerformanceCounter(&curTime);
				::SetThreadAffinityMask(threadHandle, processAffinityMask);
				return (time_ticks_type)curTime.QuadPart ;
			}

			void*			threadHandle{};
			frequency_type  processAffinityMask{};
			time_ticks_type startTime{};
			frequency_type  frequency{};
		}; // win32_timer_engine

		   /*
		   and now ladies and gentlemen the modern solution
		   #include <ctime>
		   #include <chrono>

		   int main()
		   {
		   std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		   std::time_t now_c = std::chrono::system_clock::to_time_t(now - std::chrono::hours(24));
		   std::cout << "24 hours ago, the time was "
		   << std::put_time(std::localtime(&now_c), "%F %T") << '\n';

		   std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
		   std::cout << "Hello World\n";
		   std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		   std::cout << "Printing took "
		   << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
		   << "us.\n";
		   }
		   */

	} // internal

	inline void sleep_ms(int ms)
	{
		::Sleep(ms);
	}

	class win32_timer final : implements ITimer {
		internal::win32_timer_engine imp_{};
	public :
		// Inherited via ITimer
		virtual time_ticks_type start() override {
			return imp_.start();
		}
		virtual time_ticks_type elapsed() override {
			return imp_.elapsed();
		}
	};

} // dbj

