#include "testlang.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <memory> // includes atomic now
using namespace std::literals; // for duration literals

template<typename T>
class AtomicList {
private:
	struct Node {
		T val;
		std::shared_ptr<Node> next;
	};
	std::atomic<std::shared_ptr<Node>> head;
public:
	AtomicList() = default;
	void insert(T v) {
		auto p = std::make_shared<Node>();
		p->val = v;
		p->next = head;
		while (!head.compare_exchange_weak(p->next, p)) { // atomic update
		}
	}
	void print() const {
		std::cout << "HEAD";
		for (auto p = head.load(); p; p = p->next) { // atomic read
			std::cout << "->" << p->val;
		}
		std::cout << std::endl;
	}
};

void testAtomicShared()
{
	AtomicList<std::string> alist;
	// populate list by elements from 10 threads:
	{
		std::vector<std::jthread> threads;
		for (int i = 0; i < 100; ++i) {
			threads.push_back(std::jthread{ [&, i] {
			for (auto s : {"hi", "hey", "ho", "last"}) {
			alist.insert(std::to_string(i) + s);
			std::this_thread::sleep_for(5ns);
			}
			} });
		}
	} // wait for all treads to finish
	// print resulting list:
	alist.print();
}
