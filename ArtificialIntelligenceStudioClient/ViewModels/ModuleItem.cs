using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.ViewModels
{
    /// <summary>
    /// 模块配置
    /// 假设模块dll文件在modules文件夹中
    /// 配置文件中只需要放入对应的dll文件
    /// 不过目前不知道该dll依赖的dll应该放在哪里
    /// </summary>
    internal class ModuleItem
    {
        public string name { get; set; }

        public string description { get; set; }
    }
}
