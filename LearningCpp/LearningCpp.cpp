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

	num = 319;

	auto bPrime = NumberTheoryAlg::IsPrime(num);
	if (bPrime)
	{
		std::cout << num << "是素数" << std::endl;
	}
	else
	{
		std::cout << num << "不是素数" << std::endl;
		std::cout << num << "有因子：" << NumberTheoryAlg::FindPrimeFactor(num) << std::endl;
	}



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

	std::cout << "因数分解(282): " << std::endl;
	auto vec7 = NumberTheoryAlg::Factor(282);
	for (auto i : vec7)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	int data = 282;
	int z = sqrt(data);
	bool bflag = false;
	for (int i = 0; i <= z; i++)
	{
		auto w = data - i * i;
		if (NumberTheoryAlg::IsSquare(w))
		{
			std::cout << data << ":" << i << "*" << i << "+" << w << std::endl;
			bflag = true;
			break;
		}
	}
	if (!bflag)
	{
		std::cout << data << " no" << std::endl;
	}

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

	std::cout << "计算ln(2)" << std::endl;
	double sum1 = 0.0;
	int flag = 1;
	for (int i = 1; i <= 90; i++)
	{
		sum1 += flag * 1.0 / i;
		flag *= -1;
	}
	std::cout << sum1 << "; " << std::endl;

	std::cout << "比较调和级数和根号n的大小" << std::endl;
	double sum2 = 0.0;
	for (int i = 1; i <= 10; i++)
	{
		sum2 += 1.0 / i;
		auto delta = sum2 - sqrt(i);
		std::cout << "两者的差(n=" << i << "): " << delta << std::endl;
	}

	//
	std::cout << "二分法求方程的根" << std::endl;
	int cntIterate = 0;
	double leftI = 0.001;
	double rightI = 1;
	double mid = (leftI + rightI) / 2.0;
	double delta = rightI - leftI;
	//auto G = [](double x) {return x*x+cos(2*x)-1; };
	auto G = [](double x) {return x * x * exp(x) + log(x); };
	//注意这里默认函数在两个端点的函数值异号
	while (delta > 0.0001)
	{
		if (cntIterate > 1000) break;

		cntIterate++;
		double l = G(leftI);
		double m = G(mid);
		//double r = G(right);
		if (abs(m) < 0.0000001)
		{
			break;
		}

		if (l * m > 0)
		{
			//在右侧
			leftI = mid;
		}
		else if (l * m < 0)
		{
			//在左侧
			rightI = mid;
		}
		mid = (leftI + rightI) / 2.0;
		delta = rightI - leftI;
	}

	std::cout << "迭代了" << cntIterate << "次，得到方程的根: " << mid << "误差：0.0001" << std::endl;


	std::cout << "牛顿法求方程的根" << std::endl;
	cntIterate = 0;
	double left = 0.001;
	double right = 1;
	double x0 = (left + right) / 2.0;
	double x1 = right;
	//auto DG = [](double x) {return 2 * x - 2 * sin(2 * x); };
	auto DG = [](double x) {return 2 * x * exp(x) + x * x * exp(x) + 1.0 / x; };
	delta = 1;
	//注意这里默认函数在两个端点的函数值异号
	while (delta > 0.0001)
	{
		if (cntIterate > 1000) break;

		cntIterate++;
		x1 = x0 - G(x0) / DG(x0);
		delta = abs(x1 - x0);
		x0 = x1;
	}

	std::cout << "迭代了" << cntIterate << "次，得到方程的根: " << x0 << "误差：0.0001" << std::endl;

	//std::cout << sin(1.39159)/1.39159 << "约等于" << sqrt(2.0)/2.0 << std::endl;
	std::cout << G(mid) << "约等于" << G(x0) << std::endl;

	//(x*exp(x)-log(x)-1)/x
	auto FX = [](double x) {return (x * exp(x) - log(x) - 1) / x; };
	std::cout << FX(mid) << "或者" << FX(x0) << std::endl;

	auto FX2 = [](double x) {return x * exp(x); };
	std::cout << FX2(mid) << "或者" << FX2(x0) << std::endl;

	auto FX3 = [](double x) {return x * exp(x) - log(x) - 1; };
	std::cout << FX3(mid) << "或者" << FX3(x0) << std::endl;

	unsigned int divisor = 109;
	std::cout << "模p的二次剩余: " << divisor << std::endl;
	auto ret3 = NumberTheoryAlg::MinNonNegativeQuadraticRemainder(divisor);
	for (auto i : ret3)
	{
		std::cout << i << "; ";
	}
	std::cout << std::endl;

	//
	long long mm = 143;
	std::cout << "模p的8次剩余: " << mm << std::endl;
	long long max = (mm + 1) / 2;
	for (long long i = 1; i <= max; i++)
	{
		//auto r = (long)pow(i, 8);
		long long r = i * i;
		r = r * r;
		r = r * r;
		if (r % mm == 3)
		{
			std::cout << i << "; ";
		}
	}
	//auto ret4 = NumberTheoryAlg::KTimesRemainder(3, 8, mm);
	//for (auto i : ret4)
	//{
	//	std::cout << i << "; ";
	//}
	std::cout << std::endl;

	testPtrDelete();

	//
	for (int i = 0; i < 20; i++)
	{
		unsigned int n = 2 * i + 1;
		for (int k = 3; k < 20000; k++)
		{
			//要求k和k+n+1都是素数,然后k和k+n+1之间没有素数
			int start = k;
			int end = k + n + 1;
			if (NumberTheoryAlg::IsPrime(start) && NumberTheoryAlg::IsPrime(end))
			{
				bool hasPrime = false;
				for (unsigned v = start + 1; v < end; v++)
				{
					if (NumberTheoryAlg::IsPrime(v))
					{
						hasPrime = true;
						break;
					}
				}
				if (!hasPrime)
				{
					std::cout << "恰好有" << n << "个连续的合数，位于（" << start << ", " << end << "）之间" << std::endl;
					break;
				}
			}
		}
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
