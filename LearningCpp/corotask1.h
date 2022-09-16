#pragma once
///通常把task和promise合并，定义在一起:
#include <iostream>
#include "coropromise.h"
// handle to deal with a simple coroutine task
// - providing resume() to resume it
class CoroTask1 {
public:
	// helper type for state and customization:
	struct promise_type {
		auto get_return_object() { // create the coroutine handle
			std::cout << "- promise: get_return_object()\n";
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}
		auto initial_suspend() { // start immediately?
			std::cout << "- promise: initial_suspend()\n";
			return std::suspend_always{}; // - suspend immediately
			//return std::suspend_never{}; // - suspend immediately
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

private:
	// internal handle to allocated state:
	std::coroutine_handle<promise_type> coroHdl;
public:
	// constructor to initialize the coroutine:
	CoroTask1(auto h) : coroHdl{ h } {
		std::cout << "- CoroTask1: construct\n";
	}
	~CoroTask1() {
		std::cout << "- CoroTask1: destruct\n";
		if (coroHdl) {
			coroHdl.destroy();
		}
	}
	// don’t copy or move:
	CoroTask1(const CoroTask1&) = delete;
	CoroTask1& operator=(const CoroTask1&) = delete;
	// API to resume the coroutine
	// - returns whether there is still something to process
	bool resume() const {
		std::cout << "- CoroTask1: resume()\n";
		if (!coroHdl) {
			return false; // nothing (more) to process
		}
		coroHdl.resume(); // RESUME (just coroHdl() is also possible)
		return !coroHdl.done();
	}
};
