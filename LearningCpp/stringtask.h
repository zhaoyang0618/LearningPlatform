#pragma once
#include <string>
#include <coroutine>
#include <exception> // for terminate()
class StringTask {
public:
	// customization points:
	struct promise_type {
		std::string result; // value from co_return
		void return_value(const auto& value) { // reaction on co_return
			result = value; // - store value locally
		}
		// the usual callbacks:
		auto get_return_object() {
			return std::coroutine_handle<promise_type>::from_promise(*this);
		}
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
	};
private:
	// internal coroutine handle:
	std::coroutine_handle<promise_type> coroHdl;
public:
	// constructors and destructor:
	StringTask(auto h) : coroHdl{ h } {
	}
	~StringTask() {
		if (coroHdl) {
			coroHdl.destroy();
		}
	}
	// no copying or moving is supported:
	StringTask(StringTask const&) = delete;
	StringTask& operator=(StringTask const&) = delete;
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
	std::string getResult() const {
		return coroHdl.promise().result;
	}
};
