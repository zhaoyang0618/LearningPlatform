using Microsoft.ML;
using MLApp.Common;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MLApp.ML.Base
{
    internal class BaseML
    {
        protected static string ModelPath => Path.Combine(AppContext.BaseDirectory, Constants.MODEL_FILENAME);
        protected readonly MLContext MlContext;
        protected BaseML()
        {
            MlContext=new MLContext(2020);
        }
    }
}
