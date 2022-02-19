using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TorchSharp;

namespace AIStudioClient.Modules.TorchPlatform.AI
{
    class TorchExample
    {
        public static void Test()
        {
            var t1 = torch.ones(3, 4);
            System.Diagnostics.Debug.WriteLine(t1.ToString(true));

            var t2 = torch.ones(2, 4, 4);
            System.Diagnostics.Debug.WriteLine(t2);

            var t3 = torch.empty(4, 4);
            System.Diagnostics.Debug.WriteLine(t3);

            var t4 = torch.zeros(4, 4, dtype: torch.int32);
            System.Diagnostics.Debug.WriteLine(t4.ToString(true));

            var t5 = torch.zeros(4, 4, dtype: torch.complex64);
            System.Diagnostics.Debug.WriteLine(t5.ToString(true));

            t5[0, 0] = torch.tensor(35);
            System.Diagnostics.Debug.WriteLine(t5.ToString(true));

            // Normal distribution
            var t6 = torch.randn(3, 4);
            System.Diagnostics.Debug.WriteLine(t6.ToString(true));

            // Uniform distribution between [0,1[
            var t7 = torch.rand(3, 4);
            System.Diagnostics.Debug.WriteLine(t7.ToString(true));
        }
    }
}
