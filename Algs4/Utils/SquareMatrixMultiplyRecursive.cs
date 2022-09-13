using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Algs4.Utils
{
    /// <summary>
    /// 方阵乘法: 分治算法
    /// </summary>
    internal class SquareMatrixMultiplyRecursive
    {
        public static double[,] mul(double[,] a, double[,] b)
        {
            //要求是方阵
            var n = a.Length;
            var c = new double[n, n];
            if(n == 1)
            {
                c[0, 0] = a[0, 0] * b[0, 0];
            }
            else
            {
                //分解
            }

            return c;
            throw new NotImplementedException();
        }
    }
}
