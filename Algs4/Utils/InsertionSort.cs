﻿using System;
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
                for (int j = i; j > 0 && less(a[j], a[j-1]); j--)
                {
                    exch(a, j, j-1);
                    show(a);
                }
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
            foreach(var v in a)
            {
                System.Diagnostics.Debug.Write(v);
                System.Diagnostics.Debug.Write(" ");
            }
            System.Diagnostics.Debug.Write("\n");
        }
    }
}
