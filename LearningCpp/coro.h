#pragma once
#include <iostream>
#include <coroutine> // for std::suspend_always()
#include "corotask.h" // for CoroTask

CoroTask sayHello()
{
	std::cout << "Hello" << '\n';
	co_await std::suspend_always(); // SUSPEND
	std::cout << "World" << '\n';
	co_await std::suspend_always(); // SUSPEND
	std::cout << "!" << '\n';
}
