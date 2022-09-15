///���Գ���
#include "testlang.h"
#include <iostream>
import ModReach;

void TestModeReach()
{
	// ���漸�д��������������
	// ԭ�����Dataû��export
	// Ҳ����Data�ǲ��ɼ���: not visible
	//Data d{ 11 }; // ERROR: type Data not exported
	//Customer c{ 42 };
	//const Data& dr = c.getData(); // ERROR: type Data not exported

	// ����Data�ǿɴ��: reachable
	// ���漸�д����û������
	Customer c{ 42 };
	const auto& dr = c.getData(); // OK: type Data is used
	auto d = c.getData(); // OK: d has type Data
	std::cout << d.value << '\n'; // OK: type Data is used

	// ����Ҳ�ǿ��Ե�
	decltype(std::declval<Customer>().getData()) d1; // d has non-exported type Data
	
}
