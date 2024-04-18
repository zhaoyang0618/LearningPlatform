using Microsoft.ML;
using MLApp.ML.Base;
using MLApp.ML.Objects;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MLApp.ML
{
    internal class Trainer: BaseML
    {
        public void Train(string trainingFileName)
        {
            if (!File.Exists(trainingFileName))
            {
                Console.WriteLine($"Failed to find training data file ({ trainingFileName} ");
                return;
            }
            IDataView trainingDataView = MlContext.Data.LoadFromTextFile<RestaurantFeedback>(trainingFileName);
        }
    }
}
