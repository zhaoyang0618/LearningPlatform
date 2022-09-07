using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BookStudio.DB
{
    internal class BookDBContext : DbContext
    {
        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            //optionsBuilder.UseSqlite("Data Source=lessons.db");
            optionsBuilder.UseSqlite("Filename=Books.db");
        }

        public DbSet<BookItem> Books { get; set; } //在数据库中生成数据表A
    }
}
