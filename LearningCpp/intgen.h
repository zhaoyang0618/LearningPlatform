#pragma once
#include <coroutine>
#include <exception> // for terminate()

class IntGen 
{
public:
	// customization points:
	struct promise_type {
		int currentValue; // last value from co_yield
		auto yield_value(int value) { // reaction on co_yield
			currentValue = value; // - store value locally
			return std::suspend_always{}; // - suspend coroutine
		}
		// the usual callbacks:
		auto get_return_object() {
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}
		auto initial_suspend() { return std::suspend_never{}; }
		//auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
		void return_void() { }
	};

private:
	// internal coroutine handle:
	std::coroutine_handle<promise_type> coroHdl;
public:
	// constructors and destructor:
	IntGen(auto h) : coroHdl{ h } {
	}
	~IntGen() {
		if (coroHdl) {
			coroHdl.destroy();
		}
	}
	// no copying, but moving is supported:
	IntGen(IntGen const&) = delete;
	IntGen(IntGen&& rhs) noexcept
		: coroHdl(rhs.coroHdl) {
		rhs.coroHdl = nullptr;
	}
	IntGen& operator=(IntGen const&) = delete;
	// API:
	// - resume() to resume the coroutine
	// - getValue() to get the last value from co_yield
	bool resume() const {
		if (!coroHdl) {
			return false; // nothing (more) to process
		}
		coroHdl.resume(); // RESUME
		return !coroHdl.done();
	}
	int getValue() const {
		return coroHdl.promise().currentValue;
	}
};
