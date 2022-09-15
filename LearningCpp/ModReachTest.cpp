///测试程序
#include "testlang.h"
#include <iostream>
import ModReach;

void TestModeReach()
{
	// 下面几行代码会产生编译错误
	// 原因就是Data没有export
	// 也就是Data是不可见的: not visible
	//Data d{ 11 }; // ERROR: type Data not exported
	//Customer c{ 42 };
	//const Data& dr = c.getData(); // ERROR: type Data not exported

	// 但是Data是可达的: reachable
	// 下面几行代码就没问题了
	Customer c{ 42 };
	const auto& dr = c.getData(); // OK: type Data is used
	auto d = c.getData(); // OK: d has type Data
	std::cout << d.value << '\n'; // OK: type Data is used

	// 这样也是可以的
	decltype(std::declval<Customer>().getData()) d1; // d has non-exported type Data
	
}
