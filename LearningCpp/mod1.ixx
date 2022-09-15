/// 分成多个文件实现module

module;			//start module unit with global module fragment
				//Everything from this area is not exported (no macros, no declarations, no definitions)

#include <string>
#include <vector>

export module Mod1;	//module declaration

struct Order {
	int count;
	std::string name;
	double price;
	Order(int c, std::string n, double p)
		: count{ c }, name{ n }, price{ p } {
	}
};

export class Customer
{
private:
	std::string name;
	std::vector<Order> orders;

public:
	Customer(std::string n)
		: name{ n } {
	}
	void buy(std::string ordername, double price) {
		orders.push_back(Order{ 1, ordername, price });
	}
	void buy(int num, std::string ordername, double price) {
		orders.push_back(Order{ num, ordername, price });
	}
	double sumPrice() const;
	double averagePrice() const;
	void print() const;
};
