using Prism.Modularity;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ArtificialIntelligenceStudioClient.Core
{
    public class LocalAppContext
    {
        /// <summary>
        /// 用户名
        /// </summary>
        public string UserName { get; set; }
        /// <summary>
        /// 通行证
        /// </summary>
        public string Token { get; set; }
        /// <summary>
        /// 通行证过期时间
        /// </summary>
        public DateTime ExpiredTime { get; set; }

        public List<ModuleInfo> Modules { get; set; }

        public LocalAppContext()
        {
            this.UserName = String.Empty;
            this.Token = String.Empty;
            this.Modules = new List<ModuleInfo>();
        }
    }
}
