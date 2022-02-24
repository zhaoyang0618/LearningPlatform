using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TorchSharp;
using static TorchSharp.torch;

namespace AIStudioClient.Modules.TorchPlatform.AI
{
    internal class Trivial : nn.Module
    {
        public Trivial()
        : base(nameof(Trivial))
        {
            RegisterComponents();
        }

        public override Tensor forward(Tensor input)
        {
            using var x = lin1.forward(input);
            using var y = nn.functional.relu(x);
            return lin2.forward(y);
        }

        private nn.Module lin1 = nn.Linear(1000, 100);
        private nn.Module lin2 = nn.Linear(100, 10);

        public static void Test1()
        {
            var learning_rate = 0.01f;
            var model = new Trivial();
            var loss = nn.functional.mse_loss();

            var data = Enumerable.Range(0, 16).Select(_ => rand(32, 1000)).ToList<torch.Tensor>();  // Our pretend input data
            var results = Enumerable.Range(0, 16).Select(_ => rand(32, 10)).ToList<torch.Tensor>();  // Our pretend ground truth.

            var optimizer = torch.optim.SGD(model.parameters(), learning_rate);

            for (int i = 0; i < 300; i++)
            {
                for (int idx = 0; i < data.Count; i++)
                {
                    // Compute the loss
                    using var output = loss(model.forward(data[idx]), results[idx]);

                    // Clear the gradients before doing the back-propagation
                    model.zero_grad();

                    // Do back-progatation, which computes all the gradients.
                    output.backward();

                    optimizer.step();
                }

                //保存模型
                //model.save("");
            }

            var delta = loss(model.forward(data[0]), results[0]).item<float>();
        }

        public static void Test2()
        {
            var learning_rate = 0.01f;
            var model = new Trivial();
            var loss = nn.functional.mse_loss();

            var data = Enumerable.Range(0, 16).Select(_ => rand(32, 1000)).ToList<torch.Tensor>();  // Our pretend input data
            var results = Enumerable.Range(0, 16).Select(_ => rand(32, 10)).ToList<torch.Tensor>();  // Our pretend ground truth.

            var optimizer = torch.optim.SGD(model.parameters(), learning_rate);
            var scheduler = torch.optim.lr_scheduler.StepLR(optimizer, 25, 0.95);

            for (int i = 0; i < 300; i++)
            {

                for (int idx = 0; i < data.Count; i++)
                {
                    // Compute the loss
                    using var output = loss(model.forward(data[idx]), results[idx]);

                    // Clear the gradients before doing the back-propagation
                    model.zero_grad();

                    // Do back-progatation, which computes all the gradients.
                    output.backward();

                    optimizer.step();
                }

                scheduler.step();
            }

            var delta = loss(model.forward(data[0]), results[0]).item<float>();
        }
    }
}
