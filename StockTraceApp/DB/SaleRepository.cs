using Microsoft.EntityFrameworkCore;
using StockTraceApp.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockTraceApp.DB
{
    class SaleRepository : GenericRepository<Sale>
    {
        public SaleRepository(DbContext context) : base(context)
        {
        }
    }
}
