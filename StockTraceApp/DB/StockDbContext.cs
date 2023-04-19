using Microsoft.EntityFrameworkCore;
using StockTraceApp.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace StockTraceApp.DB
{
    public class StockDbContext : DbContext
    {
        public DbSet<Stock> Stocks { get; set; }
        public DbSet<Sale> Sales { get; set; }

        protected override void OnConfiguring(
            DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlite("Data Source=stocks.db");
            //optionsBuilder.UseLazyLoadingProxies();
        }
    }
}
