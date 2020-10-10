#include "ThreadWrapper.h"
#include <atomic>
#include <chrono>
#include <vector>
#include <iostream>

std::atomic_bool g_bExit = false;

void LogTimeWork()
{
	std::cout << "entering thread id: " << std::this_thread::get_id() << std::endl;
	while (!g_bExit)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));

		if (g_bExit)
		{
			break;
		}

		std::cout << "runing thread id: " << std::this_thread::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
}

void testThread()
{
	std::vector<ThreadWrapper> thread;
	for (int i = 0; i < 16; i++)
	{
		thread.emplace_back(LogTimeWork);
	}

	std::this_thread::sleep_for(std::chrono::seconds(17));

	g_bExit = true;
}

