///
/// 标准C++的学习，目前这个编译器只支持C++17
/// 代码来源各种C++相关的数据，网络
/// 使用BOOST库
/// 

#include <iostream>
#include <cstdio>
#include "LearningCpp.h"


int main(int argc, char* argv[])
{
	//
	try
	{
		CmdLine cmdLine(argc, (const char**)argv);
		if (auto arg = cmdLine.Find("paramInt"); arg)
			std::cout << "paramInt is " << std::get<int>(*arg) << '\n';
		if (auto arg = cmdLine.Find("paramFloat"); arg)
		{
			if (const auto intPtr = std::get_if<int>(&*arg); intPtr)
				std::cout << "paramFloat is " << *intPtr << " (integer)\n";
			else
				std::cout << "paramFloat is " << std::get<float>(*arg) << '\n';
		}
		if (auto arg = cmdLine.Find("paramText"); arg)
			std::cout << "paramText is " << std::get<std::string>(*arg) << '\n';
	}
	catch (std::bad_variant_access& err)
	{
		std::cerr << " ...err: accessing a wrong variant type, "
			<< err.what() << '\n';
	}
	catch (std::runtime_error& err)
	{
		std::cerr << " ...err: " << err.what() << '\n';
	}	//
	testSample();
	testMap();
	testPrinting();
	testStricterExpressionEvaluationOrder();
	testGuaranteedCopyElision();
	testTemplate();
	testOptional();
	testVariant();
	testAny();
	testStringView();
	testStringConversion();
	//testThread();
	testParallel();
	testSTLCookbook();
	testSTLContainers();
	system("pause");
	return 0;
}
