#include "NumberTheoryAlg.h"
#include <assert.h>
#include <iostream>
#include <cmath>

bool NumberTheoryAlg::IsPrime(unsigned int n)
{
	assert(n > 1);

	//前面几个素数直接判断
	if (n == 2 || n == 3 || n == 5 || n == 7) return true;
	if (n == 4 || n == 6 || n == 8 || n == 9) return false;

	auto up = (unsigned int)std::sqrt(n);
	for (unsigned int i = 2; i <= up; i++)
	{
		if ((n % i) == 0)
			return false;
	}

	return true;
}

bool NumberTheoryAlg::IsSquare(unsigned int n)
{
	//完全立方数
	auto a = n % 4;
	if (a == 2 || a == 3) return false;
	auto sqrt = (unsigned int)std::sqrt(n);
	return (n == (sqrt * sqrt));
}

unsigned int NumberTheoryAlg::ReverseInteger(unsigned int n)
{
	unsigned int result = 0;
	while (true)
	{
		result *= 10;
		auto digits = n % 10;
		result += digits;
		n = (n - digits) / 10;
		if (n == 0)
			break;
	}

	return result;
}

unsigned int NumberTheoryAlg::FindPrimeFactor(unsigned int n)
{
	assert(n > 1);

	//n自身是素数，直接返回n
	if (IsPrime(n)) return n;

	auto up = (unsigned int)std::sqrt(n);
	for (unsigned int i = 2; i <= up; i++)
	{
		auto isPrime = IsPrime(i);
		if (isPrime)
		{
			if ((n % i) == 0)
			{
				return i;
			}
		}
	}

	return 1;
}

unsigned int NumberTheoryAlg::Omega(unsigned int n)
{
	unsigned int num = 0;
	assert(n > 1);
	while (true)
	{
		auto primeFactor = FindPrimeFactor(n);
		if (primeFactor > 1)
		{
			num++;
		}

		//这两个相等，说明已经是最后的素因子了
		if (primeFactor == n)
			break;

		n = n / primeFactor;
	}

	return num;
}

unsigned int NumberTheoryAlg::FindFourConsecutiveIntegers(unsigned int max)
{
	for (unsigned int i = 2; i < max; i++)
	{
		auto a = Omega(i);
		auto b = Omega(i+1);
		auto c = Omega(i+2);
		auto d = Omega(i+3);
		if (a == b && b == c && c == d)
		{
			std::cout << "相同的素因子数量：" << a << std::endl;
			return i;
		}
	}

	return 0;
}

std::vector<unsigned int> NumberTheoryAlg::FindAllPrimeAndReverseIntegerIsPrime(unsigned int max)
{
	std::vector<unsigned int> vec;
	for (unsigned int i = 2; i < max; i++)
	{
		auto p = IsPrime(i);
		if (p)
		{
			auto reverse = ReverseInteger(i);
			if (IsPrime(reverse))
			{
				vec.push_back(i);
			}
		}
	}

	return vec;
}


std::vector<unsigned int> NumberTheoryAlg::FindAllPrimeAndReverseIntegerIsSquare(unsigned int max)
{
	std::vector<unsigned int> vec;
	for (unsigned int i = 2; i < max; i++)
	{
		auto p = IsPrime(i);
		if (p)
		{
			auto reverse = ReverseInteger(i);
			if (IsSquare(reverse))
			{
				vec.push_back(i);
			}
		}
	}

	return vec;
}
