﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockTraceApp.Models
{
    /// <summary>
    /// 股票买卖信息
    /// </summary>
    internal class Sale
    {
        public int Id { get; set; }
        public int StockId { get; set; }
        public DateTime Date { get; set; }
        public DateTime Time { get; set; }
        public double Price { get; set; }
        /// <summary>
        /// 买入(0)还是卖出(1)
        /// </summary>
        public int Direction{ get; set; }
        public double OpeningPrice { get; set; }
        public double LowestPrice { get; set; }
        public double HighestPrice { get; set; }
        /// <summary>
        /// 上证指数
        /// </summary>
        public double ShanghaiLowestPrice { get; set; }
        public double ShanghaiHighestPrice { get; set; }
        /// <summary>
        /// 深证指数
        /// </summary>
        public double ShenzhenLowestPrice { get; set; }
        public double ShenzhenHighestPrice { get; set; }
    }
}
