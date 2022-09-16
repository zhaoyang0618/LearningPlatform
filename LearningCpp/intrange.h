#pragma once
#include <coroutine>
#include <exception> // for terminate()
#include <cassert> // for assert()
class IntRange {
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
		auto initial_suspend() { return std::suspend_always{}; }
		auto final_suspend() noexcept { return std::suspend_always{}; }
		void unhandled_exception() { std::terminate(); }
		void return_void() { }
	};
private:
	// internal coroutine handle:
	std::coroutine_handle<promise_type> coroHdl;
public:
	// constructors and destructor:
	IntRange(auto h) : coroHdl{ h } {
	}
	~IntRange() {
		if (coroHdl) {
			coroHdl.destroy();
		}
	}
	// no copying, but moving is supported:
	IntRange(IntRange const&) = delete;
	IntRange(IntRange&& rhs) noexcept
		: coroHdl(rhs.coroHdl) {
		rhs.coroHdl = nullptr;
	}
	IntRange& operator=(IntRange const&) = delete;
	// iterator interface with begin() and end()
	struct iterator {
		std::coroutine_handle<promise_type> hdl; // nullptr on end
		iterator(auto p) : hdl{ p } {
		}
		void getNext() {
			if (hdl) {
				hdl.resume(); // RESUME
				if (hdl.done()) {
					hdl = nullptr;
				}
			}
		}
		int operator*() const {
			assert(hdl != nullptr);
			return hdl.promise().currentValue;
		}
		iterator operator++() {
			getNext(); // resume for next value
			return *this;
		}
		bool operator== (const iterator& i) const = default;
	};
	iterator begin() const {
		if (!coroHdl || coroHdl.done()) {
			return iterator{ nullptr };
		}
		iterator itor{ coroHdl }; // initialize iterator
		itor.getNext(); // resume for first value
		return itor;
	}
	iterator end() const {
		return iterator{ nullptr };
	}
};
