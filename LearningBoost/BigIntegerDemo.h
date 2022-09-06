#pragma once
#include <iostream>
#include <chrono>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
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

mpz_int matrix_fib(int n)
{
    matrix<mpz_int> m(2, 2);
    m(0, 0) = 1;
    m(0, 1) = 1;
    m(1, 0) = 1;
    m(1, 1) = 0;

    matrix<mpz_int> res = mpow(m, n - 1);
    return res(0, 0);
}

class BigIntegerDemo
{
public:
    static int Run()
    {
        {
            //cpp_int类型暴力求解
            chrono::system_clock::time_point start = chrono::system_clock::now();
            cpp_int f = fib<cpp_int>(1000000);
            chrono::duration<double, std::milli> milli_sec = chrono::system_clock::now() - start;
            //cout << f << endl;
            cout << "cost " << milli_sec.count() << " millisecs\n";
            string sf = f.str();
            cout << sf.size() << "\n";
            cout << sf.substr(sf.size() - 8) << "\n";
        }

        {
            //mpz_int类型暴力求解
            chrono::system_clock::time_point start = chrono::system_clock::now();
            mpz_int f = fib<mpz_int>(1000000);
            chrono::duration<double, std::milli> milli_sec = chrono::system_clock::now() - start;
            //cout << f << endl;
            cout << "cost " << milli_sec.count() << " millisecs\n";
            string sf = f.str();
            cout << sf.size() << "\n";
            cout << sf.substr(sf.size() - 8) << "\n";
        }

        {
            //矩阵运算求解
            chrono::system_clock::time_point start = chrono::system_clock::now();
            mpz_int f = matrix_fib(1000000);
            chrono::duration<double> secs = chrono::system_clock::now() - start;
            //cout << f << endl;
            cout << "cost " << secs.count() << " secs\n";
            string sf = f.str();
            cout << sf.size() << "\n";
            cout << sf.substr(sf.size() - 8) << "\n";
        }
        return 0;
    }

};

