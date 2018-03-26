#pragma once
/*
Taken form UntiTest++ then modified
*/
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace dbj {

	class  Timer final
	{
	public:
		Timer()
			: m_threadHandle(::GetCurrentThread())
			, m_startTime(0)
		{
			// typedef unsigned long DWORD_PTR;

			DWORD_PTR systemMask;
			::GetProcessAffinityMask(GetCurrentProcess(), &m_processAffinityMask, &systemMask);
			::SetThreadAffinityMask(m_threadHandle, 1);
			::QueryPerformanceFrequency(reinterpret_cast< LARGE_INTEGER* >(&m_frequency));
			::SetThreadAffinityMask(m_threadHandle, m_processAffinityMask);
		}

		void start(){
			m_startTime = get_time();
		}

		double elapsed_in_ms() const	{
			__int64 const elapsedTime = get_time() - m_startTime;
			double const seconds = double(elapsedTime) / double(m_frequency);
			return seconds * 1000.0;
		}

	private:
		__int64 get_time() const
		{
			LARGE_INTEGER curTime;
			::SetThreadAffinityMask(m_threadHandle, 1);
			::QueryPerformanceCounter(&curTime);
			::SetThreadAffinityMask(m_threadHandle, m_processAffinityMask);
			return curTime.QuadPart;
		}

		void* m_threadHandle{};

#if defined(_WIN64)
		unsigned __int64 m_processAffinityMask{};
#else
		unsigned long m_processAffinityMask{};
#endif

		__int64 m_startTime{};
		__int64 m_frequency{};
	};

	void sleep_ms(int ms)
	{
		::Sleep(ms);
	}

}

