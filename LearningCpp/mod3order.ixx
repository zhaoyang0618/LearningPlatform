module; // start module unit with global module fragment

#include <string>

//这一点和Josuttis的书《C++20 The Complete Guid》有差异
//参考420页的示例
//书上的代码没有export
export module Mod3:Order; // internal partition declaration

struct Order {
	int count;
	std::string name;
	double price;
	Order(int c, std::string n, double p)
		: count{ c }, name{ n }, price{ p } {
	}
};
