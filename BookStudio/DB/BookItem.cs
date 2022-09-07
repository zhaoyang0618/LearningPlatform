using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BookStudio.DB
{
    internal class BookItem
    {
        [MaxLength(256)]
        public string Id { get; set; }
        [MaxLength(1024)]
        public string Name { get; set; }
        [MaxLength(256)]
        public string Authors { get; set; }
        [MaxLength(128)]
        public string Isbn { get; set; }
        [MaxLength(256)]
        public string Publisher { get; set; }
        [MaxLength(512)]
        public string Series { get; set; }
        public string Description { get; set; }
        [MaxLength(1024)]
        public string FilePath { get; set; }
    }
}
