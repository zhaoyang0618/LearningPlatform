using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Taiji.Utils
{
    /// <summary>
    /// 用于文件压缩
    /// 也可用于其他地方，文件名和文件内容
    /// https://stackoverflow.com/questions/53378497/net-core-create-on-memory-zipfile
    /// </summary>
    public class InMemoryFile
    {
        public string FileName { get; set; }
        public byte[] Content { get; set; }
    }
}
