#include "testlang.h"
#include <iostream>
import Mod1;
//import Mod2;

void testMod1()
{
	std::cout << "Test Module Mod1" << '\n';
	Customer c1{ "Kim" };
	c1.buy("table", 59.90);
	c1.buy(4, "chair", 9.20);

	c1.print();
	std::cout << " Average: " << c1.averagePrice() << '\n';
}
