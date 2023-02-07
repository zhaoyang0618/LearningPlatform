///
/// ��׼C++��ѧϰ��Ŀǰ���������ֻ֧��C++17
/// ������Դ����C++��ص����ݣ�����
/// ʹ��BOOST��
/// 

#include <iostream>
#include <cstdio>
#include <format>
#include "LearningCpp.h"
#include "CppGrammer.h"
#include "inlinethreadlocal.h"
#include "trie.h"
#include "testlang.h"
#include "NumberTheoryAlg.h"

//ʹ��ģ��
import Square;


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

	//
	auto p = std::make_unique<int>(10);
	auto x(std::move(p));

	//
	//testSample();
	//testMap();
	//testPrinting();
	//testStricterExpressionEvaluationOrder();
	//testGuaranteedCopyElision();
	//testTemplate();
	//testOptional();
	//testVariant();
	//testAny();
	//testStringView();
	//testStringConversion();
	////testThread();
	//testParallel();
	//testSTLCookbook();
	//testSTLContainers();

	//ThreadDemo::TestPackagedTask();
	//ThreadDemo::TestPackageTaskFuture();

	//
	//CppGrammerDemo();
	//ThreadDemo::TestAsync2();
	//ThreadDemo::TestLock();
	//ThreadDemo::TestPackagedTask2();
	//testThreadLocal();
	//testCpp20();
	//ExploringCpp20();
	//testFoldExpressions();
	//testTrackNew();
	//trie<string>::test();

	//
	int num = 42;
	//auto err = square(num);//����ֱ������
	Square sq = toSquare(num);
	std::cout << std::format("the sqaure of {} is: {}\n", num, sq.getValue());

	//TestModeReach();
	//testMod1();
	//testMod3();
	//testCoro();
	//testjthread();
	//testLatch();
	//testLatchReady();
	//testBarrier();
	//testSemaphore();
	//testSemaphoreNotify();
	//testAtomicRef();
	//testAtomicShared();
	//testAtomicWeak();
	//testAtomicWait();
	//testAtomicTicket();
	testAsyncStream();

	std::cout << "��������" << std::endl;
	for (int a = 1; a < 10; a++)
	{
		for (int b = 0; b < 10; b++)
		{
			for (int c = 0; c < 10; c++)
			{
				int A = a * a * a;
				int B = b * b * b;
				int C = c * c * c;
				int val = 100 * a + 10 * b + c;
				if ((A + B + C) == val)
				{
					std::cout << val << std::endl;
				}
			}
		}
	}

	std::cout << "�ҵ��������ĸ����������ǵ�������������ͬ" << std::endl;
	auto ret = NumberTheoryAlg::FindFourConsecutiveIntegers();
	std::cout << ret << "; " << ret + 1 << "; " << ret + 2 << "; " << ret + 3 << std::endl;

	std::cout << "�ҵ�10000���ڵ���������������Ҳ������" << std::endl;
	auto vec1 = NumberTheoryAlg::FindAllPrimeAndReverseIntegerIsPrime();
	for (auto i : vec1)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;


	std::cout << "�ҵ�10000���ڵ�������������������ȫƽ����" << std::endl;
	auto vec2 = NumberTheoryAlg::FindAllPrimeAndReverseIntegerIsSquare();
	for (auto i : vec2)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	std::cout << "�ҵ�10000���ڵ�����p��ʹ��P+2,p+6Ҳ������" << std::endl;
	auto cond = [](unsigned int p) { return NumberTheoryAlg::IsPrime(p + 2) && NumberTheoryAlg::IsPrime(p + 6); };
	auto vec3 = NumberTheoryAlg::FindAllPrimeSatisfyCondition(cond);
	for (auto i : vec3)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	system("pause");
	return 0;
}
