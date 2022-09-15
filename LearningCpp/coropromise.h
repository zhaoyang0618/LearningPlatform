#pragma once
#include <iostream>
#include <coroutine>
#include <exception> // for terminate()
// API to control the basic handling of a coroutine
// and to deal with a result or an exception
template <typename CoroType>
struct CoroPromise {
	auto get_return_object() { // create the coroutine handle
		std::cout << "- promise: get_return_object()\n";
		return std::coroutine_handle<CoroPromise<CoroType>>::from_promise(*this);
	}
	auto initial_suspend() { // start immediately?
		std::cout << "- promise: initial_suspend()\n";
		return std::suspend_always{}; // - suspend immediately
	}
	auto final_suspend() noexcept { // clean-ups / postprocessing
		std::cout << "- promise: final_suspend()\n";
		return std::suspend_always{}; // - suspend at the end
	}
	void unhandled_exception() { // deal with exceptions
		std::terminate(); // - terminate the program
	}
	void return_void() { // deal with the end or with return;
		std::cout << "- promise: return_void()\n";
	}
};
