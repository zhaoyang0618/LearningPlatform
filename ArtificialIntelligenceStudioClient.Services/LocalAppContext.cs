using System;

namespace ArtificialIntelligenceStudioClient.Services
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
    }
}
