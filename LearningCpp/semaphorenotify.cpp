#include "testlang.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <semaphore>
using namespace std::literals; // for duration literals

void testSemaphoreNotify()
{
	int sharedData;
	std::binary_semaphore dataReady{ 0 }; // signal there is data to process
	std::binary_semaphore dataDone{ 0 }; // signal processing is done

	// start threads to read and process values by value:
	std::jthread process{ [&](std::stop_token st) {
		std::cout << "线程" << std::this_thread::get_id() << "开始" << std::endl;
		while (!st.stop_requested()) {
			// wait until next value ready:
			std::cout << "线程" << std::this_thread::get_id() << "等待" << std::endl;
			// - timeout after 1s to check stop_token
			if (dataReady.try_acquire_for(1s)) {
				int data = sharedData;
				// process it:
				std::cout << "[process] read " << data << std::endl;
				std::this_thread::sleep_for(data * 0.5s);
				std::cout << "[process] done" << std::endl;
				// signal processing done:
				dataDone.release();
			}
			else {
				std::cout << "[process] timeout" << std::endl;
			}
		}
		std::cout << "[process] end" << std::endl;
	}};

	// generate a couple of values:
	for (int i = 0; i < 10; ++i) {
		// store next value:
		std::cout << "[main] store " << i << std::endl;
		sharedData = i;
		// signal to start processing:
		dataReady.release();
		// wait until processing is done:
		dataDone.acquire();
		std::cout << "[main] processing done\n" << std::endl;
	}
	// end of loop signals stop
	std::cout << "[main] end\n" << std::endl;
}
