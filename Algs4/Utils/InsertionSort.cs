using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Algs4.Utils
{
    internal class InsertionSort
    {
        public static void sort<T>(T[] a) where T : IComparable<T>
        {
            int N = a.Length;
            for (int i = 0; i < N; i++)
            {
                //找到最小的
                int min = i;
                for (int j = i + 1; j < N; j++)
                {
                    //a[j]比a[min]还要小, 就更改min
                    if (less(a[j], a[min]))
                    {
                        min = j;
                    }
                }

                //把最小的放到前面
                exch(a, i, min);
            }
        }

        private static bool less<T>(T l, T r) where T : IComparable<T>
        {
            return l.CompareTo(r) < 0;
        }

        private static void exch<T>(T[] a, int i, int j)
        {
            T t = a[i];
            a[i] = a[j];
            a[j] = t;
        }
    }
}
