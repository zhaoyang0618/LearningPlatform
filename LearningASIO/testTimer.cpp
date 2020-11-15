#include "testTimer.h"
#include <asio.hpp>
#include <iostream>

/// <summary>
/// 同步
/// </summary>
void syncTimer()
{
	std::cout << "Using a timer synchronously" << std::endl;
	asio::io_context io;

	asio::steady_timer t(io, asio::chrono::seconds(5));
	t.wait();

	std::cout << "Hello, world!" << std::endl;
}

void print(const asio::error_code& /*e*/)
{
	std::cout << "Hello, world!" << std::endl;
}

/// <summary>
/// 异步
/// </summary>
void asyncTimer()
{
	std::cout << "Using a timer asynchronously" << std::endl;
	asio::io_context io;

	asio::steady_timer t(io, asio::chrono::seconds(5));
	t.async_wait(&print);
	io.run();
}

void print2(const asio::error_code& e,
	asio::steady_timer* t, int* count)
{
	if (*count < 5)
	{
		std::cout << *count << std::endl;
		++(*count);
		t->expires_at(t->expiry() + asio::chrono::seconds(1));
		std::cout << "Hello, world!" << std::endl;
		t->async_wait(std::bind(print2, e, t, count));
	}
}

void asyncTimerWithPara()
{
	std::cout << "Binding arguments to a handler" << std::endl;

	asio::io_context io;

	int count = 0;
	asio::steady_timer t(io, asio::chrono::seconds(1));
	asio::error_code e;
	t.async_wait(std::bind(print2, e, &t, &count));
	io.run();
	std::cout << "Final count is " << count << std::endl;
}

class printer
{
public:
	printer(asio::io_context& io)
		: timer_(io, asio::chrono::seconds(1))
		, count_(0)
	{
		timer_.async_wait(std::bind(&printer::print, this));
	}

	~printer()
	{
		std::cout << "Final count is " << count_ << std::endl;
	}

	void print()
	{
		if (count_ < 5)
		{
			std::cout << count_ << std::endl;
			++(count_);
			timer_.expires_at(timer_.expiry() + asio::chrono::seconds(1));
			std::cout << "Hello, world!" << std::endl;
			timer_.async_wait(std::bind(&printer::print, this));
		}
	}

private:
	asio::steady_timer timer_;
	int count_;
};

void asyncTimerWithMemberFunction()
{
	std::cout << "Using a member function as a handler" << std::endl;
	asio::io_context io;
	printer p(io);
	io.run();
}


class printer2
{
public:
	printer2(asio::io_context& io)
		: strand_(asio::make_strand(io))
		, timer1_(io, asio::chrono::seconds(1))
		, timer2_(io, asio::chrono::seconds(1))
		, count_(0)
	{
		timer1_.async_wait(asio::bind_executor(strand_,
			std::bind(&printer2::print1, this)));

		timer2_.async_wait(asio::bind_executor(strand_,
			std::bind(&printer2::print2, this)));
	}

	~printer2()
	{
		std::cout << "Final count is " << count_ << std::endl;
	}

	void print1()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 1: " << count_ << std::endl;
			++count_;

			timer1_.expires_at(timer1_.expiry() + asio::chrono::seconds(1));

			timer1_.async_wait(asio::bind_executor(strand_,
				std::bind(&printer2::print1, this)));
		}
	}

	void print2()
	{
		if (count_ < 10)
		{
			std::cout << "Timer 2: " << count_ << std::endl;
			++count_;

			timer2_.expires_at(timer2_.expiry() + asio::chrono::seconds(1));

			timer2_.async_wait(asio::bind_executor(strand_,
				std::bind(&printer2::print2, this)));
		}
	}

private:
	asio::strand<asio::io_context::executor_type> strand_;
	asio::steady_timer timer1_;
	asio::steady_timer timer2_;
	int count_;
};

void timer5()
{
	std::cout << "Synchronising handlers in multithreaded programs" << std::endl;
	asio::io_context io;
	printer2 p(io);
	//对于这种重载函数，需要说明具体使用哪一个
	asio::thread t(std::bind((asio::io_context::count_type(asio::io_context::*)())&asio::io_context::run, &io));
	io.run();
	t.join();
}

void testTimer()
{
	syncTimer();
	asyncTimer();
	asyncTimerWithPara();
	asyncTimerWithMemberFunction();
	timer5();
}
