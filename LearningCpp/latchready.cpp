#include "testlang.h"

#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include <latch>
using namespace std::literals; // for duration literals

void testLatchReady()
{
	std::size_t numThreads = 10;
	// initialize latch to start the threads when all of them are initialized:
	std::latch allReady{ 10 }; // initialize countdown with number of threads
	// start numThreads threads:
	std::vector<std::jthread> threads;
	for (int i = 0; i < numThreads; ++i) {
		threads.push_back(std::jthread{ [i, &allReady] {
			// initialize each thread (simulate to take some time):
			std::this_thread::sleep_for(100ms * i);
			// synchronize threads so that all start together here:
			allReady.arrive_and_wait();
			// perform whatever the thread does
			// (loop printing its index):
			for (int j = 0; j < i + 5; ++j) {
				std::cout.put(static_cast<char>('0' + i)).flush();
				std::this_thread::sleep_for(50ms);
			}
			} 
			});
	}
}
