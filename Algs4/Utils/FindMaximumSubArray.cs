using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Algs4.Utils
{
    /// <summary>
    /// 查找最大的子序列
    /// </summary>
    internal class FindMaximumSubArray
    {
        public static (int, int, int) find(int[] A)
        {
            return find(A, 0, A.Length - 1);
        }

        private static (int, int, int) find(int[] A,int low, int high)
        {
            if (high == low)
            {
                return (low, high, A[low]);
            }
            else
            {
                int mid = (low + high) / 2;
                var(leftlow, lefthigh, leftsum) = find(A, low, mid);
                var (rightlow, righthigh, rightsum) = find(A, mid + 1, high);
                var(crosslow, crosshigh, crosssum) = findMaxCrossingSubArray(A, low, mid, high);
                if(leftsum > rightsum && leftsum > crosssum)
                    return (leftlow, lefthigh, leftsum);
                else if(rightsum > crosssum && rightsum > leftsum)
                    return (rightlow, righthigh, rightsum);
                else
                    return (crosslow, crosshigh, crosssum);
            }
        }

        private static (int, int, int) findMaxCrossingSubArray(int[] A, int low, int mid, int high)
        {
            int leftsum = int.MinValue;
            int sum = 0;
            int maxLeft = mid;
            for(int i=mid; i>= low;i--)
            {
                sum = sum + A[i];
                if(sum > leftsum)
                {
                    leftsum = sum;
                    maxLeft = i;
                }
            }

            int rightsum = int.MinValue;
            int maxRight = mid;
            for (int i = mid+1; i <= high; i++)
            {
                sum = sum + A[i];
                if (sum > rightsum)
                {
                    rightsum = sum;
                    maxRight = i;
                }
            }

            return (maxLeft, maxRight, leftsum+rightsum);
        }
    }
}
