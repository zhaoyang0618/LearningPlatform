#include "NumberTheoryAlg.h"
#include <assert.h>
#include <iostream>
#include <cmath>
#include <algorithm>

std::vector<unsigned int> NumberTheoryAlg::PrimeNumbers;

bool NumberTheoryAlg::IsPrime(unsigned int n)
{
	//assert(n > 1);
	if (n <= 1) return false;

	//ǰ�漸������ֱ���ж�
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

bool NumberTheoryAlg::IsPrime(unsigned int n, const std::vector<unsigned int>& primes)
{
	if (primes.size() == 0) return IsPrime(n);
	//std::sort(primes.begin(), primes.end());
	//Ҫ��primes���������������С��������
	if (n <= 1) return false;

	//ǰ�漸������ֱ���ж�
	if (n == 2 || n == 3 || n == 5 || n == 7) return true;
	if (n == 4 || n == 6 || n == 8 || n == 9) return false;

	auto up = (unsigned int)std::sqrt(n);
	unsigned int max = 2;
	for (auto p : primes)
	{
		if (p == n)
			return true;

		if ((p < n) && ((n % p) == 0))
			return false;

		if (p > up)
			return true;

		if (max < p)
			max = p;
	}

	for (unsigned int i = max; i <= up; i++)
	{
		if ((n % i) == 0)
			return false;
	}

	return true;
}

unsigned int NumberTheoryAlg::PrimePi(unsigned int n)
{
	if (n <= 1) return 0;

	//ǰ�漸������ֱ���ж�
	if (n == 2) return 1;

	///����Ϊ2������ֱ�Ӱ�ż��ȫ���ų�����Ϊ���ǲ�����������
	std::vector<unsigned int> primes;
	unsigned int num = 1;
	for (unsigned int i = 3; i <= n; i+=2)
	{
		if (IsPrime(i,  primes))
		{
			num++;
			if (primes.size() < 1000)
			{
				primes.push_back(i);
			}
		}
	}

	return num;
}

unsigned int NumberTheoryAlg::PrimePi(unsigned int n, std::vector<unsigned int>& primes, int max)
{
	if (n <= 1) return 0;

	//ǰ�漸������ֱ���ж�
	if (n == 2) return 1;

	///����Ϊ2������ֱ�Ӱ�ż��ȫ���ų�����Ϊ���ǲ�����������
	unsigned int num = 1;
	for (unsigned int i = 3; i <= n; i += 2)
	{
		if (IsPrime(i, primes))
		{
			num++;
			if (primes.size() < max)
			{
				primes.push_back(i);
			}
		}
	}

	return num;
}

unsigned int NumberTheoryAlg::PrimePi(unsigned int n, unsigned int min, unsigned int pi_min)
{
	if (n <= 1) return 0;

	//ǰ�漸������ֱ���ж�
	if (n == 2) return 1;

	if (n == min) return pi_min;

	unsigned int num = pi_min;
	if (min % 2 != 0)
	{
		//��֤Ϊż��
		min = min + 1;
	}

	//�ų���ż��
	for (unsigned int i = min+1; i <= n; i+=2)
	{
		if (IsPrime(i))
		{
			num++;
		}
	}

	return num;
}

unsigned int NumberTheoryAlg::PrimePi(unsigned int n, unsigned int min, unsigned int pi_min, const std::vector<unsigned int>& primes)
{
	if (n <= 1) return 0;

	//ǰ�漸������ֱ���ж�
	if (n == 2) return 1;

	if (n == min) return pi_min;

	unsigned int num = pi_min;
	if (min % 2 != 0)
	{
		//��֤Ϊż��
		min = min + 1;
	}

	//�ų���ż��
	for (unsigned int i = min + 1; i <= n; i += 2)
	{
		if (IsPrime(i, primes))
		{
			num++;
		}
	}

	return num;
}

bool NumberTheoryAlg::IsSquare(unsigned int n)
{
	//��ȫ������
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

	//n������������ֱ�ӷ���n
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

		//��������ȣ�˵���Ѿ���������������
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
			std::cout << "��ͬ��������������" << a << std::endl;
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

std::vector<unsigned int> NumberTheoryAlg::MinNonNegativeQuadraticRemainder(unsigned int divisor)
{
	assert(divisor > 2);

	std::vector<unsigned int> vec;
	if (!IsPrime(divisor)) return vec;
	auto max = (divisor - 1) / 2;
	for (unsigned int i = 1; i <= max; i++)
	{
		auto r = (i * i) % divisor;
		vec.push_back(r);
	}

	//����
	std::sort(vec.begin(), vec.end());
	return vec;
}

std::vector<unsigned int> NumberTheoryAlg::KTimesRemainder(unsigned int a, unsigned int k, unsigned int m)
{
	assert(m > 2);

	std::vector<unsigned int> vec;
	unsigned int max = (m + 1) / 2;
	for (unsigned int i = 1; i <= max; i++)
	{
		auto r = (long)pow(i, k);
		if (r % m == 0)
		{
			vec.push_back(r);
		}
	}

	//����
	std::sort(vec.begin(), vec.end());
	return vec;
}

