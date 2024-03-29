#include <iostream>
#include <thread>
#include <chrono>
#include "coro.h"
#include "intgen.h"
#include "intrange.h"
#include "stringtask.h"

template <typename T>
IntGen loopOver(const T& coll)
{
	// coroutine that iterates over the elements of a collection:
	for (int elem : coll) {
		std::cout << "- yield " << elem << '\n';
		co_yield elem; // calls yield_value(elem) on promise
		std::cout << "- resume\n";
	}
}

template <typename T>
IntRange loopOver2(const T& coll)
{
	// coroutine that iterates over the elements of a collection:
	for (auto elem : coll) {
		std::cout << "- suspend" << '\n';
		co_yield elem; // calls yield_value(elem) on promise
		std::cout << "- resume" << '\n';
	}
}

StringTask computeInSteps()
{
	std::string ret;
	ret += "Hello";
	co_await std::suspend_always(); // SUSPEND
	ret += " World";
	co_await std::suspend_always(); // SUSPEND
	ret += "!";
	co_return ret;
}

void testCoro()
{
	using namespace std::literals;

	//第一个例子 
	{
		// start coroutine:
		CoroTask sayHelloTask = sayHello();
		std::cout << "coroutine sayHello() started\n";
		// loop to resume the coroutine until it is done:
		while (sayHelloTask.resume()) { // resume
			std::this_thread::sleep_for(1000ms);
		}
	}

	//第二个例子
	{
		// define generator that yields the elements of a collection:
		std::vector<int> coll{ 0, 8, 15, 33, 42, 77 };
		IntGen gen = loopOver(coll);
		// loop to resume the coroutine until there is no more value:
		std::cout << "start loop:\n";
		while (gen.resume()) { // resume until we have the next value
			std::cout << "main(): value: " << gen.getValue() << '\n';
			std::this_thread::sleep_for(1s);
		}
	}

	//
	{
		// define generator that yields the elements of a collection:
		std::vector<int> coll{ 0, 8, 15, 33, 42, 77 };
		IntRange gen = loopOver2(coll);
		// loop to resume the coroutine until there is no more value:
		std::cout << "start loop:\n";
		for (const auto& val : gen) {
			std::cout << "value: " << val << '\n';
			std::this_thread::sleep_for(1s);
		}
	}

	//第三个例子
	{
		StringTask task = computeInSteps();
		// loop to resume the coroutine until it is done:
		while (task.resume()) { // RESUME
			std::this_thread::sleep_for(500ms);
		}
		// print return value of coroutine:
		std::cout << "result: " << task.getResult() << '\n';

	}
}
