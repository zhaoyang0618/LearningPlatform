#pragma once
#include <thread>
#include <functional>
void testThread();

class ThreadWrapper
{
	std::thread _thread;

public:
	ThreadWrapper(std::function<void()> f)
		: _thread(f)
	{
	}

	ThreadWrapper(ThreadWrapper&& rhs) noexcept
		: _thread(std::move(rhs._thread))
	{
	}

	~ThreadWrapper()
	{
		if (_thread.joinable())
		{
			_thread.join();
		}
	}
};

