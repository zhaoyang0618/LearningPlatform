using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TorchSharp;
using static TorchSharp.torch;

namespace AIStudioClient.Modules.TorchPlatform.AI
{
    /// <summary>
    /// BP神经网络
    /// https://www.cnblogs.com/lemon-567/p/14441137.html
    /// class Net(torch.nn.Module):
    ///     def __init__(self, n_feature, n_hidden, n_output):
    ///         super(Net, self).__init__()
    ///         self.hidden = torch.nn.Linear(n_feature, n_hidden)
    ///         self.predict = torch.nn.Linear(n_hidden, n_output)
    ///         
    ///     def forward(self, x):
    ///         x = F.relu(self.hidden(x))
    ///         x = self.predict(x)
    ///         return x
    /// </summary>
    internal class BPNetModel : nn.Module
    {
        public BPNetModel(int features, int hidden, int outputs)
        : base(nameof(BPNetModel))
        {
            RegisterComponents();
            this.hidden = nn.Linear(features, hidden);
            predict = nn.Linear(hidden, outputs);
        }

        public override Tensor forward(Tensor input)
        {
            using var x = hidden.forward(input);
            using var y = nn.functional.relu(x);
            return predict.forward(y);
        }

        private nn.Module hidden = nn.Linear(1000, 100);
        private nn.Module predict = nn.Linear(100, 10);

        /// <summary>
        /// 
        /// optimizer = torch.optim.SGD(net1.parameters(), lr=0.5)    #梯度下降方法
        /// loss_function = torch.nn.MSELoss()   #计算误差方法
        /// # 训练部分
        /// for i in range(500):
        ///   prediction = net1(xtrain)
        ///   prediction = prediction.squeeze(-1)
        ///   loss = loss_function(prediction, ytrain)
        ///   if loss.data <= 0.06:
        ///     break
        ///   optimizer.zero_grad()    #消除梯度
        ///   loss.backward()    #反向传播
        ///   optimizer.step()     #执行
        /// </summary>
        public static void Test()
        {
            var learning_rate = 0.01f;
            var model = new BPNetModel(1, 2, 1);
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

        public static void Model()
        {
            var net = nn.Sequential(
                nn.Linear(2, 12),
                nn.ReLU(),    //激活函数
                nn.Linear(12, 1),
                nn.LogSoftmax(1));
        }
    }
}
