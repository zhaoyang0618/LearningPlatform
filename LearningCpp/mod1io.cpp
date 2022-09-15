module; // start module unit with global module fragment
#include <iostream>
#include <format>

module Mod1; // implementation unit of module Mod1

void Customer::print() const
{
	// print name:
	std::cout << name << ":\n";
	// print order entries:
	for (const auto& od : orders) {
		std::cout << std::format("{:3} {:14} {:6.2f} {:6.2f}\n", od.count, od.name, od.price, od.count * od.price);
	}
	// print sum:
	std::cout << std::format("{:25} ------\n", ' ');
	std::cout << std::format("{:25} {:6.2f}\n", " Sum:", sumPrice());
}
