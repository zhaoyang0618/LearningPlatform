module; // start module unit with global module fragment
#include <string>
#include <vector>

export module Mod3:Customer; // interface partition declaration
import :Order; // import internal partition to use Order

export class Customer {
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
