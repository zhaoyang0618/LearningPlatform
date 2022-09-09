using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Algs4.Utils
{
    /// <summary>
    /// 归并排序
    /// 需要的内存和数组长度有关
    /// </summary>
    internal class MergeSort
    {
        public static void sort<T>(T[] a) where T : IComparable<T>
        {
            sort(a, 0, a.Length - 1);
        }
        public static void sort<T>(T[] a, int lo, int hi) where T : IComparable<T>
        {
            if(hi < lo)return;
            int mid = lo + (hi - lo) / 2;
            sort(a, lo, mid);
            sort(a, mid + 1, hi);
            merge(a, lo, mid, hi);
        }

        private static void merge<T>(T[] a, int lo, int mid, int hi) where T : IComparable<T>
        {
            int i = lo, j = mid + 1;
            T[]aux = new T[a.Length];
            for (int k = lo; k <= hi; k++)
                aux[k] = a[k];
            for(int k=lo; k <= hi; k++)
            {
                if (i > mid) a[k] = aux[j++];
                else if(j > hi) a[k] = aux[i++];
                else if (less(aux[j], aux[i])) a[k] = aux[j++];
                else a[k] = aux[i++];
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
