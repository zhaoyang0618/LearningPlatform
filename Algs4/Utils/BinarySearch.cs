using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Algs4.Utils
{
    /// <summary>
    /// 二分查找
    /// </summary>
    internal class BinarySearch
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="key">要查找的内容</param>
        /// <param name="a"></param>
        /// <returns></returns>
        public static int rank(int key, int[]a)
        {
            int lo = 0;
            int hi = a.Length - 1;
            while(lo <= hi)
            {
                int mid = lo + (hi - lo) / 2;
                if (key < a[mid]) hi = mid - 1;
                else if (key > a[mid]) lo = mid + 1;
                else return mid;

            }
            return -1;
        }

        public static void test()
        {

        }
    }
}
