#pragma once
#include <iostream>
#include <chrono>
#include <boost/multiprecision/cpp_int.hpp>
//#include <boost/multiprecision/gmp.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace std;
using namespace boost::multiprecision;
using namespace boost::numeric::ublas;

template <class T>
T fib(int n)
{
    T f = 1;
    T f1 = 1;
    T f2 = 1;
    for (int i = 1; i <= n - 2; ++i)
    {
        if (i == 1 && i == 2)
            f = 1;
        else
        {
            f1 = std::move(f2);
            f2 = std::move(f);
            f = f1 + f2;
        }
    }
    return f;
}

//计算阶乘
template <class T>
T factor(int n)
{
    if (n == 0) return 1;
    T f = 1;
    for (int i = 1; i <= n; ++i)
    {
        f = f * i;
    }
    return f;
}

//计算阶乘
template <class T>
T leftFactor(int n)
{
    if (n == 0) return 0;

    T sum = 1;
    T f = 1;
    for (int i = 1; i < n; ++i)
    {
        f = f * i;
        sum = sum + f;
    }
    return sum;
}

//欧几里得算法求两个整数的最大公约数
template <class T>
T euclid(T m, T n)
{
    if (m == 0) return n;
    if (n == 0) return m;

    T r = m % n;
    if (r == 0) return n;
    while (r != 0)
    {
        m = n;
        n = r;
        r = m % n;
    }

    return n;
}



template <class T>
T mpow(const T& b, int e)
{
    if (e == 1)
        return b;
    if (e % 2 == 1)
        return prod(b, mpow(b, e - 1));
    T tmp = mpow(b, e / 2);
    return prod(tmp, tmp);
}

//mpz_int matrix_fib(int n)
//{
//    matrix<mpz_int> m(2, 2);
//    m(0, 0) = 1;
//    m(0, 1) = 1;
//    m(1, 0) = 1;
//    m(1, 1) = 0;
//
//    matrix<mpz_int> res = mpow(m, n - 1);
//    return res(0, 0);
//}

class BigIntegerDemo
{
public:
    static int Run()
    {
        int num = 1000;
        {
            //cpp_int类型暴力求解
            chrono::system_clock::time_point start = chrono::system_clock::now();
            cpp_int f = factor<cpp_int>(num);// fib<cpp_int>(1000000000);
            chrono::duration<double, std::milli> milli_sec = chrono::system_clock::now() - start;
            //cout << f << endl;
            cout << "cost " << milli_sec.count() << " millisecs\n";
            string sf = f.str();
            cout << "factor of " << num << " has " << sf.size() << " digits\n";
            cout << "factor of " << num << " is: " << sf << "\n";
            //cout << sf.substr(sf.size() - 8) << "\n";
        }

        {
            //cpp_int类型暴力求解
            chrono::system_clock::time_point start = chrono::system_clock::now();
            cpp_int f = leftFactor<cpp_int>(num);// fib<cpp_int>(1000000000);
            chrono::duration<double, std::milli> milli_sec = chrono::system_clock::now() - start;
            //cout << f << endl;
            cout << "cost " << milli_sec.count() << " millisecs\n";
            string sf = f.str();
            cout << "left factor of " << num << " has " << sf.size() << " digits\n";
            cout << "left factor of " << num << " is: " << sf << "\n";
            //cout << sf.substr(sf.size() - 8) << "\n";

            int exceptionCount = 0;
            for (int i = 1000; i <= 10000; i++)
            {
                auto a = factor<cpp_int>(num);
                auto b = leftFactor<cpp_int>(num);
                cpp_int d = euclid<cpp_int>(a, b);
                if (d == 2)
                {
                }
                else
                {
                    exceptionCount++;
                    cout << "\nthe max common factor of !" << i << " and " << i << "! is not equal to 2\n";
                }
                cout << ".";
            }

            cout << "\n";

            if (exceptionCount > 0)
            {
                cout << "found exception count: " << exceptionCount << "\n";
            }
            else
            {
                cout << "no exception \n";
            }

        }

        //{
        //    //mpz_int类型暴力求解
        //    chrono::system_clock::time_point start = chrono::system_clock::now();
        //    mpz_int f = fib<mpz_int>(1000000);
        //    chrono::duration<double, std::milli> milli_sec = chrono::system_clock::now() - start;
        //    //cout << f << endl;
        //    cout << "cost " << milli_sec.count() << " millisecs\n";
        //    string sf = f.str();
        //    cout << sf.size() << "\n";
        //    cout << sf.substr(sf.size() - 8) << "\n";
        //}

        //{
        //    //矩阵运算求解
        //    chrono::system_clock::time_point start = chrono::system_clock::now();
        //    mpz_int f = matrix_fib(1000000);
        //    chrono::duration<double> secs = chrono::system_clock::now() - start;
        //    //cout << f << endl;
        //    cout << "cost " << secs.count() << " secs\n";
        //    string sf = f.str();
        //    cout << sf.size() << "\n";
        //    cout << sf.substr(sf.size() - 8) << "\n";
        //}
        return 0;
    }

};

