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
#include <chrono>
// #include "no_copy_no_move.h"

namespace dbj::samples {

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
	/// <param name="ull_">floating type</param>
	/// <returns>rounded to time_ticks_type</returns>
	template<typename T>
	inline 
	typename std::enable_if_t< std::is_floating_point_v< T >, time_ticks_type >
	 round_to_tt_type(T  ull_) {
		return static_cast<time_ticks_type>(ull_ < 0 ? ull_ - 0.5 : ull_ + 0.5);
	}
	/// <summary>
	/// same but for integrals
	/// </summary>
	/// <param name="ull_">int type</param>
	/// <returns>narrowed to time_ticks_type</returns>
	template<typename T>
	inline
		typename std::enable_if_t< std::is_integral_v< T >, time_ticks_type >
		round_to_tt_type(T  int_) {
		return static_cast<time_ticks_type>(int_);
	}

	/// <summary>
	/// timer engine Interface 
	/// </summary>
	struct __declspec(novtable) ITimer
	{
		virtual time_ticks_type	start() = 0;
		virtual time_ticks_type	elapsed() = 0;
	};

	inline void sleep_seconds(int seconds_)	{
		std::this_thread::sleep_for(std::chrono::seconds(seconds_));
	}

	using itimer_pointer = std::shared_ptr<ITimer>;

	enum class timer_kind : int { win32 = 0, modern = 1 };

	namespace internal {

		class  win32_timer_engine final : public ITimer
		{
		public:
			win32_timer_engine  () noexcept
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

			// virtual time_ticks_type start() override
			virtual time_ticks_type start() override  {
				return this->startTime = get_time();
			}

			virtual time_ticks_type elapsed() override {
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

		using std::chrono::duration_cast;

		class modern_timer final  : public ITimer 
		{
		public:
			using Clock = typename  std::chrono::steady_clock;
			using Seconds = typename std::chrono::seconds;
			using MilliSeconds = typename std::chrono::milliseconds;
			using Microseconds = typename std::chrono::microseconds;
			using Nanoseconds = typename std::chrono::nanoseconds;
			using Unit = MilliSeconds;
			// Inherited via ITimer
			virtual time_ticks_type start() override {
				this->start_ =  Clock::now();
				return duration_cast<MilliSeconds>(start_.time_since_epoch()).count();
			}

			virtual time_ticks_type elapsed() override {
				typename Unit::rep elapsed = duration_cast<Unit>(Clock::now() - start_).count();
				return elapsed;
			}
		private: 
			Clock::time_point start_ = Clock::now() ;
		};

	} // internal

	/// <summary>
	/// the only visible class
	/// from this module
	/// has its engine changeble 
	/// at construction time by 
	/// the factory method 
	/// </summary>
	class  __declspec(novtable) the_timer final 
	{
		mutable itimer_pointer imp_{};
		
		explicit the_timer(itimer_pointer && engine_)
	    {
			imp_ = std::move(engine_);
		}

		the_timer() = default;

	public :
		~the_timer() = default;
		// no copy
		the_timer(const the_timer&) = delete;
		the_timer& operator=(const the_timer&) = delete;
		// move
		the_timer(the_timer&& other_) noexcept { std::swap(this->imp_, other_.imp_ ); }
		the_timer& operator=(the_timer&& other_) noexcept { std::swap(this->imp_, other_.imp_); return *this;  }


		time_ticks_type start()  {	return imp_->start();	}

		time_ticks_type elapsed()  { return imp_->elapsed(); }

	/// <summary>
	/// the factory method
	/// </summary>
	/// <returns>the_timer instance</returns>
		friend the_timer create_timer(timer_kind which_);

	}; // the timer

	   /// <summary>
	   /// the factory method
	   /// </summary>
	   /// <returns>the_timer instance</returns>
	inline the_timer create_timer(timer_kind which_) {

		if (which_ == timer_kind::win32)
			return  the_timer{
			itimer_pointer{ new internal::win32_timer_engine{} }
		};
		//
		return the_timer{
			itimer_pointer{ new internal::modern_timer{} }
		};
	}

} // dbj

