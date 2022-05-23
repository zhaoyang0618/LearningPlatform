using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TorchSharp.Modules;
using static TorchSharp.torch;

namespace AIStudioClient.Modules.TorchPlatform.AI
{

    //class Encoder(nn.Module):
    //def __init__(self,
    //             input_size = 2,
    //             embedding_size = 128,
    //             hidden_size = 256,
    //             n_layers = 4,
    //             dropout = 0.5):
    //    super().__init__()
    //    self.hidden_size = hidden_size
    //    self.n_layers = n_layers
    //    self.linear = nn.Linear(input_size, embedding_size)
    //    self.rnn = nn.LSTM(embedding_size, hidden_size, n_layers,
    //                       dropout = dropout)
    //    self.dropout = nn.Dropout(dropout)

    //def forward(self, x):
    //    """
    //    x: input batch data, size: [sequence len, batch size, feature size]
    //    for the argoverse trajectory data, size(x) is [20, batch size, 2]
    //    """
    //    # embedded: [sequence len, batch size, embedding size]
    //    embedded = self.dropout(F.relu(self.linear(x)))
    //    # you can checkout https://pytorch.org/docs/stable/nn.html?highlight=lstm#torch.nn.LSTM
    //    # for details of the return tensor
    //    # briefly speaking, output coontains the output of last layer for each time step
    //    # hidden and cell contains the last time step hidden and cell state of each layer
    //    # we only use hidden and cell as context to feed into decoder
    //    output, (hidden, cell) = self.rnn(embedded)
    //    # hidden = [n layers * n directions, batch size, hidden size]
    //    # cell = [n layers * n directions, batch size, hidden size]
    //    # the n direction is 1 since we are not using bidirectional RNNs
    //    return hidden, cell
    internal class Encoder: nn.Module
    {
        int hidden_size = 0;
        int n_layers = 0;
        Linear linear = null;
        LSTM rnn = null;
        Dropout dropout = null;
        public Encoder(int input_size = 2,
            int embedding_size = 128,
                int hidden_size = 256,
                 int n_layers = 4,
                 double dropout = 0.5)
            : base(nameof(Encoder))
        {
            RegisterComponents();
            this.hidden_size = hidden_size;
            this.n_layers = n_layers;
            this.linear = nn.Linear(input_size, embedding_size);
            this.rnn = nn.LSTM(embedding_size, hidden_size, n_layers, dropout: dropout);
            this.dropout = nn.Dropout(dropout);
        }

        public (Tensor, Tensor) forward(Tensor x)
        {
            //embedded: [sequence len, batch size, embedding size]
            var output = this.linear.forward(x);
            output = nn.ReLU().forward(output);
            output = this.dropout.forward(output);
            //# you can checkout https://pytorch.org/docs/stable/nn.html?highlight=lstm#torch.nn.LSTM
            //# for details of the return tensor
            //# briefly speaking, output coontains the output of last layer for each time step
            //# hidden and cell contains the last time step hidden and cell state of each layer
            //# we only use hidden and cell as context to feed into decoder
            var (a, hidden, cell) = this.rnn.forward(output);
            //# hidden = [n layers * n directions, batch size, hidden size]
            //# cell = [n layers * n directions, batch size, hidden size]
            //# the n direction is 1 since we are not using bidirectional RNNs
            return (hidden, cell);
        }
    }
}
