#pragma once
#include <chrono>
using namespace std::chrono;

class TimeLost
{
public:
	TimeLost()
		: m_begin(high_resolution_clock::now())
	{}

	~TimeLost()
	{}

public:
	void reset()
	{
		m_begin = high_resolution_clock::now();
	}

	int64_t elspsed()
	{
		auto delta = high_resolution_clock::now() - m_begin;
		return duration_cast<microseconds>(delta).count();
	}

private:
	time_point<high_resolution_clock> m_begin;
};

