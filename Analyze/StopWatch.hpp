#ifndef _STOPWATCH_H
#define _STOPWATCH_H

#include <chrono>
#include <ostream>


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

	void tic_total(std::ostream& os)
	{
		os << "us since start: " << get_total_time_us() << "\n";
	}

	void tic(std::ostream& os)
	{
		os << "us since last start: " << get_time_us() << "\n";
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
};

#endif
