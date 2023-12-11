///һЩ������ص��㷨
#pragma once
#include <vector>

class NumberTheoryAlg
{
public:
	static std::vector<unsigned int> PrimeNumbers;

	/// <summary>
	/// �ж�һ�������Ƿ�Ϊ����
	/// ��򵥵ķ���������Ҳ�ǱȽ����ķ�����
	/// ����������������̫��
	/// ����Ҫ�����1
	/// </summary>
	/// <returns></returns>
	static bool IsPrime(unsigned int n);
	/// <summary>
	/// ʹ��һ�������ĸ�������
	/// </summary>
	/// <param name="n"></param>
	/// <param name="primes"></param>
	/// <returns></returns>
	static bool IsPrime(unsigned int n, const std::vector<unsigned int>& primes);

	/// <summary>
	/// ͳ��С�ڵ���n�������ĸ���
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static unsigned int PrimePi(unsigned int n);
	static unsigned int PrimePi(unsigned int n, std::vector<unsigned int>& primes, int max = 1000);

	/// <summary>
	/// ͳ��С�ڵ���n�������ĸ���
	/// �Ѿ�֪��min���ڵ���������,����ֻ��Ҫ��min+1��ʼ����
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static unsigned int PrimePi(unsigned int n, unsigned int min, unsigned int pi_min);
	static unsigned int PrimePi(unsigned int n, unsigned int min, unsigned int pi_min, const std::vector<unsigned int>& primes);

	/// <summary>
	/// �ж�һ�������Ƿ�Ϊ��ȫƽ����
	/// </summary>
	/// <returns></returns>
	static bool IsSquare(unsigned int n);

	/// <summary>
	/// n����
	/// ����163����������361
	/// </summary>
	/// <returns></returns>
	static unsigned int ReverseInteger(unsigned int n);

	/// <summary>
	/// ����n�к��е�����p�ĸ���
	/// ��ʵ�����ҵ�ָ��x��ʹ��$p^x$����$n$������$p^{x+1}$��������
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
	/// �ҵ�һ��С��$\sqrt{n}$��������
	/// �϶����ҵ�
	/// ����1˵���Ҳ��������᷵�������ֻ�ǳ���Ҫ��
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static unsigned int FindPrimeFactor(unsigned int n);

	/// <summary>
	/// ͳ�����е������ӵĸ���
	/// Omega����
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static unsigned int Omega(unsigned int n);

	/// <summary>
	/// �ҵ��������ĸ����������ǵ�������������ͬ
	/// �ҵ�һ����ֹͣ��
	/// ������max
	/// ����0����û���ҵ�
	/// </summary>
	/// <param name="max"></param>
	/// <returns></returns>
	static unsigned int FindFourConsecutiveIntegers(unsigned int max = 100000);

	/// <summary>
	/// �ҵ�������������������������
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static std::vector<unsigned int> FindAllPrimeAndReverseIntegerIsPrime(unsigned int max = 10000);

	/// <summary>
	/// �ҵ���������:��������������ȫƽ����
	/// </summary>
	/// <param name="n"></param>
	/// <returns></returns>
	static std::vector<unsigned int> FindAllPrimeAndReverseIntegerIsSquare(unsigned int max = 10000);

	/// <summary>
	/// �ҵ���������:��������������ȫƽ����
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

	/// <summary>
	/// ���ӷֽ�
	/// </summary>
	/// <param name="max"></param>
	/// <returns></returns>
	static std::vector<unsigned int> Factor(unsigned int num)
	{
		std::vector<unsigned int> vec;
		if (num > 0)
		{
			if (num == 1)
			{
				vec.push_back(1);
			}
			else if (IsPrime(num))
			{
				vec.push_back(num);
			}
			else
			{
				//�ҵ�һ��������
				auto h = num / 2 + 1;
				unsigned int next = 0;
				for (int i = 2;i < h; i++)
				{
					if (IsPrime(i) && num % i == 0)
					{
						vec.push_back(i);
						next = num / i;
						break;
					}
				}

				auto ret = Factor(next);
				if (ret.size() > 0)
				{
					vec.insert(vec.end(), ret.begin(), ret.end());
				}
			}
		}
		return vec;
	}

	/// <summary>
	/// ������С�Ǹ�ʣ�� ����ʣ��
	/// ����Ҫ��divisorΪ����
	/// ������ʱ������֤��
	/// </summary>
	static std::vector<unsigned int> MinNonNegativeQuadraticRemainder(unsigned int divisor);
};

