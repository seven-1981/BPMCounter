#ifndef _STOPWATCH_H
#define _STOPWATCH_H

#include <chrono>


class StopWatch
{
public:
	StopWatch() { init(); }
	~StopWatch() { }

	void init()
	{
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	long long get_total_time_us()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> yet;
		yet = std::chrono::high_resolution_clock::now();
		long long us = std::chrono::duration_cast<std::chrono::microseconds>(yet - this->m_startTime).count();
		return us;
	}

	long long get_time_us()
	{
		std::chrono::time_point<std::chrono::high_resolution_clock> yet;
		yet = std::chrono::high_resolution_clock::now();
		long long us = std::chrono::duration_cast<std::chrono::microseconds>(yet - this->m_startTime).count();
		this->m_startTime = yet;
		return us;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};

#endif
