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

namespace dbj_samples {

	class  Timer final
	{
	public:
		Timer()
			:  threadHandle(::GetCurrentThread())
			,  startTime(0)
		{
			_ASSERTE(threadHandle);

			DWORD_PTR systemMask;
			::GetProcessAffinityMask(GetCurrentProcess(), & processAffinityMask, &systemMask);
			::SetThreadAffinityMask( threadHandle, 1);
			::QueryPerformanceFrequency(reinterpret_cast< LARGE_INTEGER* >(& frequency));
			::SetThreadAffinityMask( threadHandle,  processAffinityMask);
		}

		void start(){
			 startTime = get_time();
		}

		double elapsed_in_ms() const	{
			__int64 const elapsedTime = get_time() -  startTime;
			double const seconds = double(elapsedTime) / double( frequency);
			return seconds * 1000.0;
		}

	private:
		__int64 get_time() const
		{
			_ASSERTE(threadHandle);
			LARGE_INTEGER curTime;
			::SetThreadAffinityMask( threadHandle, 1);
			::QueryPerformanceCounter(&curTime);
			::SetThreadAffinityMask( threadHandle,  processAffinityMask);
			return curTime.QuadPart;
		}

		void*  threadHandle{};

#if defined(_WIN64)
		unsigned __int64  processAffinityMask{};
#else
		unsigned long  processAffinityMask{};
#endif
		__int64  startTime{};
		__int64  frequency{};
	}; // Timer

	inline void sleep_ms(int ms)
	{
		::Sleep(ms);
	}

} // dbj

