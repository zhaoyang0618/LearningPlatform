using Microsoft.EntityFrameworkCore;
using System;
using System.Collections.Generic;
using System.Text;

namespace DevicePortCommunicationClient.DAL
{
    class IoTDbContext : DbContext
    {
        private static IoTDbContext _context = new IoTDbContext();
        public static IoTDbContext Context
        {
            get
            {
                return _context;
            }
        }

        public static void Release()
        {
            if (_context != null)
            {
                _context.Dispose();
                _context = null;
            }
        }

        public IoTDbContext()
        {
        }

        protected override void OnConfiguring(
            DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer(@"Data Source=.\SQLEXPRESS;Integrated Security=SSPI;Database=iot_db");
            //optionsBuilder.UseLazyLoadingProxies();
            base.OnConfiguring(optionsBuilder);
        }

        public DbSet<DBMachineDevice> DBMachineDevices { get; set; }
        public DbSet<DBCommModule> DBCommModules { get; set; }
        public DbSet<DBIoTCommand> DBIoTCommands { get; set; }
    }
}
