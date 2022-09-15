#include <iostream>
#include <thread>
#include <chrono>
#include "coro.h"

void testCoro()
{
	using namespace std::literals;
	// start coroutine:
	CoroTask sayHelloTask = sayHello();
	std::cout << "coroutine sayHello() started\n";
	// loop to resume the coroutine until it is done:
	while (sayHelloTask.resume()) { // resume
		std::this_thread::sleep_for(1000ms);
	}
}