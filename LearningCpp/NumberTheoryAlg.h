///一些数论相关的算法
#pragma once
#include <vector>

class NumberTheoryAlg
{
public:
	/// <summary>
	/// 判断一个整数是否为素数
	/// 最简单的方法，但是也是比较慢的方法。
	/// 而且输入整数不能太大
	/// 整数要求大于1
	/// </summary>
	/// <returns></returns>
	static bool IsPrime(unsigned int n);

	/// <summary>
	/// 判断一个整数是否为完全平方数
	/// </summary>
	/// <returns></returns>
	static bool IsSquare(unsigned int n);

	/// <summary>
	/// n逆序
	/// 例如163的逆序数是361
	/// </summary>
	/// <returns></returns>
	static unsigned int ReverseInteger(unsigned int n);

	/// <summary>
	/// 整数n中含有的因子p的个数
	/// 其实就是找到指数x，使得$p^x$整除$n$，但是$p^{x+1}$不能整除
	/// </summary>
	/// <param name="n"></param>
	/// <param name="p"></param>
	/// <returns></returns>
	static int NumOfFactor(unsigned int n, unsigned int p)
	{
		int idx = 0;
		while ((n % p) == 0)
		{
			idx++;
			n = n / p;
		}

		return idx;
	}

	/// <summary>
	/// 找到一个小于$\sqrt{n}$的素因子
	/// 肯定能找到
	/// 返回1说明找不到（不会返回这个，只是程序要求）
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static unsigned int FindPrimeFactor(unsigned int n);

	/// <summary>
	/// 统计所有的素因子的个数
	/// Omega函数
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static unsigned int Omega(unsigned int n);

	/// <summary>
	/// 找到连续的四个整数，它们的素因子数量相同
	/// 找到一个即停止，
	/// 不超过max
	/// 返回0表明没有找到
	/// </summary>
	/// <param name="max"></param>
	/// <returns></returns>
	static unsigned int FindFourConsecutiveIntegers(unsigned int max = 100000);

	/// <summary>
	/// 找到所有素数：其逆序数是素数
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static std::vector<unsigned int> FindAllPrimeAndReverseIntegerIsPrime(unsigned int max = 10000);

	/// <summary>
	/// 找到所有素数:它的逆序数是完全平方数
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static std::vector<unsigned int> FindAllPrimeAndReverseIntegerIsSquare(unsigned int max = 10000);

	/// <summary>
	/// 找到所有素数:它的逆序数是完全平方数
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	template<typename FUNC>
	static std::vector<unsigned int> FindAllPrimeSatisfyCondition(FUNC func, unsigned int min = 2, unsigned int max = 10000)
	{
		std::vector<unsigned int> vec;
		for (unsigned int i = min; i < max; i++)
		{
			auto p = IsPrime(i);
			if (p)
			{
				if (func(i))
				{
					vec.push_back(i);
				}
			}
		}

		return vec;
	}
};

