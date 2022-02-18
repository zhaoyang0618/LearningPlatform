using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PythonExam
{
    public class LocalAppContext
    {
        public string name { get; set; }

        public string call()
        {
            return string.Format("名称为: {0}", name);
        }
    }
}
