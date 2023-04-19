///
/// 标准C++的学习，目前这个编译器只支持C++17
/// 代码来源各种C++相关的数据，网络
/// 使用BOOST库
/// 

#include <iostream>
#include <cstdio>
#include <format>
#include <algorithm>
#include "LearningCpp.h"
#include "CppGrammer.h"
#include "inlinethreadlocal.h"
#include "trie.h"
#include "testlang.h"
#include "NumberTheoryAlg.h"

//使用模块
import Square;

bool IsValid(unsigned int p, unsigned int k)
{

	for (unsigned int i = 1; i < k; i++)
	{
		if (NumberTheoryAlg::IsPrime(p - 2 * i))
		{
			return false;
		}
		if (NumberTheoryAlg::IsPrime(p + 2 * i))
		{
			return false;
		}
	}

	if (!NumberTheoryAlg::IsPrime(p - 2 * k))
	{
		return false;
	}

	if (!NumberTheoryAlg::IsPrime(p + 2 * k))
	{
		return false;
	}

	return true;
}

/// <summary>
/// 近似值
/// </summary>
/// <param name="k"></param>
/// <returns></returns>
unsigned int ApproximationKthCompositeNumber(unsigned int k)
{
	double a = 1.0 / log(k);
	double b = 1.0 - a - a * a;
	double c = k * 1.0 / b;
	unsigned int n = ceil(c);
	std::cout << "第[" << k << "]个合数(近似值)：" << n << std::endl;
	return n; //floor(c);
}

/// <summary>
/// 输出第k个合数
/// n_k = 1 + \pi(n_k) + k
/// n_k - \pi(n_k) = 1+k
/// </summary>
/// <param name="k"></param>
/// <returns></returns>
unsigned int TheKthCompositeNumber(unsigned int k, unsigned int max, unsigned int min = 1)
{
	unsigned int ret = 0;
	for (unsigned int n = min; n < max; n++)
	{
		if (NumberTheoryAlg::PrimePi(n) == n - k - 1)
		{
			ret = n;
			std::cout << "第[" << k << "]个合数：" << n << std::endl;
			break;
		}
	}

	return ret;
}

/// <summary>
/// 输出第k个合数
/// n_k = 1 + \pi(n_k) + k
/// n_k - \pi(n_k) = 1+k
/// </summary>
/// <param name="k"></param>
/// <returns></returns>
unsigned int TheKthCompositeNumber(unsigned int k)
{
	unsigned int ret = 0;
	auto min = ApproximationKthCompositeNumber(k);
	auto max = min + 1000;
	unsigned int prev = min;
	unsigned int pi_prev = NumberTheoryAlg::PrimePi(min);
	for (unsigned int n = min; n < max; n++)
	{
		auto now = NumberTheoryAlg::PrimePi(n, prev, pi_prev);
		prev = n;
		pi_prev = now;
		if (now == n - k - 1)
		{
			ret = n;
			std::cout << "第[" << k << "]个合数：" << n << std::endl;
			break;
		}
	}

	return ret;
}

/// <summary>
/// 输出第k个合数: k包含在veck数组里面，要求veck中数据是从小到大排列的
/// n_k = 1 + \pi(n_k) + k
/// n_k - \pi(n_k) = 1+k
/// </summary>
/// <param name="k"></param>
/// <returns></returns>
std::vector<unsigned int> TheKthCompositeNumber(std::vector<unsigned int> veck)
{
	std::vector<unsigned int> retV;
	auto size = veck.size();
	if (size == 0) return retV;
	//排序
	std::sort(veck.begin(), veck.end());

	unsigned int k = veck[0];
	auto first = ApproximationKthCompositeNumber(k);
	std::vector<unsigned int> primes;
	unsigned int maxPrimeIndex = 1024 * 128;
	unsigned int pi_first = NumberTheoryAlg::PrimePi(first, primes, maxPrimeIndex);
	auto sizeP = primes.size();
	std::cout << "第[" << sizeP << "]个素数：" << primes[sizeP -1] << std::endl;

	////找到前面1000个素数
	//std::vector<unsigned int> primes(1000);
	//primes.push_back(2);
	unsigned nn = primes[sizeP - 1] + 2;
	while (true)
	{
		sizeP = primes.size();
		if (sizeP == maxPrimeIndex)
		{
			std::cout << "第[" << sizeP << "]个素数：" << primes[sizeP - 1] << std::endl;
			break;
		}

		if (NumberTheoryAlg::IsPrime(nn, primes))
		{
			primes.push_back(nn);
		}

		nn += 2;
	}

	for (auto kk : veck)
	{
		auto min = ApproximationKthCompositeNumber(kk);
		auto max = min + 300000;// *2;
		unsigned int prev = min;
		unsigned int pi_prev = NumberTheoryAlg::PrimePi(min, first, pi_first, primes);
		first = prev;
		pi_first = pi_prev;
		for (unsigned int n = min; n < max; n++)
		{
			auto now = NumberTheoryAlg::PrimePi(n, prev, pi_prev, primes);
			prev = n;
			pi_prev = now;
			if (now == n - kk - 1)
			{
				retV.push_back(n);
				std::cout << "第[" << kk << "]个合数：" << n << std::endl;
				break;
			}
		}

	}


	return retV;
}

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
	//auto err = square(num);//会出现编译错误
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

	std::cout << "查找数字" << std::endl;
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

	std::cout << "找到连续的四个整数，它们的素因子数量相同" << std::endl;
	auto ret = NumberTheoryAlg::FindFourConsecutiveIntegers();
	std::cout << ret << "; " << ret + 1 << "; " << ret + 2 << "; " << ret + 3 << std::endl;

	std::cout << "找到10000以内的素数，其逆序数也是素数" << std::endl;
	auto vec1 = NumberTheoryAlg::FindAllPrimeAndReverseIntegerIsPrime();
	for (auto i : vec1)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;


	std::cout << "找到10000以内的素数，其逆序数是完全平方数" << std::endl;
	auto vec2 = NumberTheoryAlg::FindAllPrimeAndReverseIntegerIsSquare();
	for (auto i : vec2)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	std::cout << "找到10000以内的素数p，使得P+2,p+6也是素数" << std::endl;
	auto cond = [](unsigned int p) { return NumberTheoryAlg::IsPrime(p + 2) && NumberTheoryAlg::IsPrime(p + 6); };
	auto vec3 = NumberTheoryAlg::FindAllPrimeSatisfyCondition(cond);
	for (auto i : vec3)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	std::cout << "找到25以内的正整数p，使得2500-99p是完全平方数" << std::endl;
	for (int i = 1; i < 26; i++)
	{
		auto n = 2500 - 99 * i;
		if (NumberTheoryAlg::IsSquare(n))
		{
			std::cout << i << "; ";
		}
	}
	std::cout << std::endl;

	std::cout << "找到10000以内的素数p，使得P-6,p+6也是素数" << std::endl;
	//auto cond2 = [](unsigned int p) { return NumberTheoryAlg::IsPrime(p - 6) && 
	//	(!NumberTheoryAlg::IsPrime(p - 4)) && (!NumberTheoryAlg::IsPrime(p - 2)) &&
	//	(!NumberTheoryAlg::IsPrime(p + 2)) && (!NumberTheoryAlg::IsPrime(p + 4)) &&
	//	NumberTheoryAlg::IsPrime(p + 6); };
	auto cond2 = [](unsigned int p) { return IsValid(p, 3); };
	auto vec4 = NumberTheoryAlg::FindAllPrimeSatisfyCondition(cond2, 6);
	for (auto i : vec4)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	std::cout << "找到10000以内的素数p，使得P-12,p+12也是素数" << std::endl;
	//auto cond3 = [](unsigned int p) { return NumberTheoryAlg::IsPrime(p - 12) && NumberTheoryAlg::IsPrime(p + 12); };
	auto cond3 = [](unsigned int p) { return IsValid(p, 6); };
	auto vec5 = NumberTheoryAlg::FindAllPrimeSatisfyCondition(cond3, 6);
	for (auto i : vec5)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	std::cout << "找到50000以内的素数p，使得P-18,p+18也是素数" << std::endl;
	//auto cond4 = [](unsigned int p) { return NumberTheoryAlg::IsPrime(p - 18) && NumberTheoryAlg::IsPrime(p + 18); };
	auto cond4 = [](unsigned int p) { return IsValid(p, 9); };
	auto vec6 = NumberTheoryAlg::FindAllPrimeSatisfyCondition(cond4, 6, 50000);
	for (auto i : vec6)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	std::cout << "因数分解(2107): " << std::endl;
	auto vec7 = NumberTheoryAlg::Factor(2107);
	for (auto i : vec7)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	unsigned int sum = 1001;
	if (NumberTheoryAlg::IsPrime(sum))
	{
		std::cout << sum << "是素数" << std::endl;
	}
	else
	{
		std::cout << sum << "是合数" << std::endl;
		auto ret = NumberTheoryAlg::Factor(sum);
		for (auto f : ret)
		{
			std::cout << f << "; ";
		}
		std::cout << std::endl;
	}

	//std::cout << "查找第K个合数" << std::endl;
	//auto idxC = TheKthCompositeNumber(1, 1000);
	//idxC = TheKthCompositeNumber(10, 4294967295, idxC);
	//idxC = TheKthCompositeNumber(100, 4294967295, idxC);
	//idxC = TheKthCompositeNumber(1000, 4294967295, idxC);
	////auto n_10000 = ApproximationKthCompositeNumber(10000);
	////auto n_100000 = ApproximationKthCompositeNumber(100000);
	////auto n_1000000 = ApproximationKthCompositeNumber(1000000);
	////auto n_10000000 = ApproximationKthCompositeNumber(10000000);
	////auto n_100000000 = ApproximationKthCompositeNumber(100000000);
	////auto n_1000000000 = ApproximationKthCompositeNumber(1000000000);
	////idxC = TheKthCompositeNumber(10000);
	////idxC = TheKthCompositeNumber(100000);
	////idxC = TheKthCompositeNumber(1000000);
	////idxC = TheKthCompositeNumber(10000000);
	////idxC = TheKthCompositeNumber(100000000);
	////idxC = TheKthCompositeNumber(1000000000);
	//auto r = TheKthCompositeNumber({ 10000, 100000, 1000000, 10000000, 100000000, 1000000000});
	system("pause");
	return 0;
}
