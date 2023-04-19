using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockTraceApp.Models
{
    /// <summary>
    /// 股票信息
    /// </summary>
    internal class Stock
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string Code { get; set; }
        public string Description { get; set; }
        public string Type { get; set; }
    }
}
