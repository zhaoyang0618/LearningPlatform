module; // start module unit with global module fragment

#include <string>

//��һ���Josuttis���顶C++20 The Complete Guid���в���
//�ο�420ҳ��ʾ��
//���ϵĴ���û��export
export module Mod3:Order; // internal partition declaration

struct Order {
	int count;
	std::string name;
	double price;
	Order(int c, std::string n, double p)
		: count{ c }, name{ n }, price{ p } {
	}
};
