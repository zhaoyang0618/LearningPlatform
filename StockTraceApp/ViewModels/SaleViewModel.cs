using StockTraceApp.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockTraceApp.ViewModels
{
    public class SaleViewModel
    {
        public Sale Model { get; set; }
        public DateTime DateTime {
            get
            { 
                if(Model == null)
                    return DateTime.MinValue;

                return new DateTime(Model.Date.Year, Model.Date.Month, Model.Date.Day, Model.Time.Hour, Model.Time.Minute, Model.Time.Second);
            }
        }

        public double? Value
        {
            get
            {
                if (Model == null)
                    return null;

                return Model.Price;
            }
        }

        public string Name
        {
            get
            {
                if (Model == null)
                    return string.Empty;
                return Model.Price.ToString();
            }
        }

        public int Count
        {
            get
            {
                if (Model == null)
                    return 0;
                return Model.SaleCount;
            }
        }
    }
}
