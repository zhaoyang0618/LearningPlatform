#include "testlang.h"
#include <iostream>
#include <stop_token>
#include <future> // for async()
#include <thread> // for sleep_for()
using namespace std::literals; // for duration literals

void func(std::stop_token st, int num)
{
	auto id = std::this_thread::get_id();
	std::cout << "call func(" << num << ")\n";
	// register a first callback:
	std::stop_callback cb1{ st, [num, id] {
	std::cout << "- STOP1 requested in func(" << num
	<< (id == std::this_thread::get_id() ? ")\n"
	: ") in main thread\n");
	} };
	std::this_thread::sleep_for(9ms);
	// register a second callback:
	std::stop_callback cb2{ st, [num, id] {
	std::cout << "- STOP2 requested in func(" << num
	<< (id == std::this_thread::get_id() ? ")\n"
	: ") in main thread\n");
	} };
	std::this_thread::sleep_for(2ms);
}

void testjthread()
{
	// create stop_source and stop_token:
	std::stop_source ssrc;
	std::stop_token stok{ ssrc.get_token() };
	// register callback:
	std::stop_callback cb{ stok, [] {
	std::cout << "- STOP requested in main()\n" << std::flush;
	} };
	// in the background call func() a couple of times:
	auto fut = std::async([stok] {
		for (int num = 1; num < 10; ++num) {
			func(stok, num);
		}
		});
	// after a while, request stop:
	std::this_thread::sleep_for(120ms);
	ssrc.request_stop();
}
