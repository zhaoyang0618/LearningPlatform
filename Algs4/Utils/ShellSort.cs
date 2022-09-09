using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Algs4.Utils
{
    /// <summary>
    /// 需要的额外内存是常数
    /// </summary>
    internal class ShellSort
    {
        public static void sort<T>(T[] a) where T : IComparable<T>
        {
            int N = a.Length;
            int h = 1;
            while (h < N / 3) h = 3 * h + 1;
            while(h >= 1)
            {
                System.Diagnostics.Debug.WriteLine("H={0}", h);
                for (int i = h; i < N; i++)
                {
                    for(int j = i; j >= h && less(a[j], a[j - h]); j-=h)
                    {
                        exch(a, j, j - h);
                        show(a);
                    }

                }

                h /= 3;
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
        private static void show<T>(T[] a)
        {
            foreach (var v in a)
            {
                System.Diagnostics.Debug.Write(v);
                System.Diagnostics.Debug.Write(" ");
            }
            System.Diagnostics.Debug.Write("\n");
        }
    }
}
