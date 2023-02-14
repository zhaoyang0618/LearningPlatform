///һЩ������ص��㷨
#pragma once
#include <vector>

class NumberTheoryAlg
{
public:
	/// <summary>
	/// �ж�һ�������Ƿ�Ϊ����
	/// ��򵥵ķ���������Ҳ�ǱȽ����ķ�����
	/// ����������������̫��
	/// ����Ҫ�����1
	/// </summary>
	/// <returns></returns>
	static bool IsPrime(unsigned int n);

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
};

