#pragma once
#include <iostream>
#include "coropromise.h"
// handle to deal with a simple coroutine task
// - providing resume() to resume it
class CoroTask {
public:
	// helper type for state and customization:
	using promise_type = CoroPromise<CoroTask>;
private:
	// internal handle to allocated state:
	std::coroutine_handle<promise_type> coroHdl;
public:
	// constructor to initialize the coroutine:
	CoroTask(auto h) : coroHdl{ h } {
		std::cout << "- CoroTask: construct\n";
	}
	~CoroTask() {
		std::cout << "- CoroTask: destruct\n";
		if (coroHdl) {
			coroHdl.destroy();
		}
	}
	// don¡¯t copy or move:
	CoroTask(const CoroTask&) = delete;
	CoroTask& operator=(const CoroTask&) = delete;
	// API to resume the coroutine
	// - returns whether there is still something to process
	bool resume() const {
		std::cout << "- CoroTask: resume()\n";
		if (!coroHdl) {
			return false; // nothing (more) to process
		}
		coroHdl.resume(); // RESUME (just coroHdl() is also possible)
		return !coroHdl.done();
	}
};
