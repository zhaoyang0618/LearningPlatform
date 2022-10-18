using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SpiderClient.IoC
{
    internal interface IMyIOCContainer
    {
        void ResolveType<TFrom, TTo>();
        T Resolve<T>();
    }
}
