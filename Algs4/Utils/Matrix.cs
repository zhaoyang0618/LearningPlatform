using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Algs4.Utils
{
    internal class Matrix
    {
        public static double dot(double[] x, double[] y)
        {
            System.Diagnostics.Debug.Assert(x.Length == y.Length);
            double sum = 0;
            for (int i = 0; i < x.Length; i++)
            {
                for (int j = 0; j < y.Length; j++)
                    sum += x[i] * y[j];
            }

            return sum;
        }

        /// <summary>
        /// 不是所有的矩阵都可以相乘的, 
        /// a的列数需要等于b的行数
        /// </summary>
        /// <param name="a"></param>
        /// <param name="b"></param>
        /// <returns></returns>
        public static double[][] mult(double[][] a, double[][] b)
        {
            System.Diagnostics.Debug.Assert(a.Length > 0);
            System.Diagnostics.Debug.Assert(b.Length > 0);
            System.Diagnostics.Debug.Assert(b[0].Length > 0);
            System.Diagnostics.Debug.Assert(a[0].Length == b.Length);
            var ret = new double[a.Length][];
            return ret;
        }
    }
}
