using System;

namespace ArtificialIntelligenceStudioClient.Services
{
    public class LocalAppContext
    {
        /// <summary>
        /// �û���
        /// </summary>
        public string UserName { get; set; }
        /// <summary>
        /// ͨ��֤
        /// </summary>
        public string Token { get; set; }
        /// <summary>
        /// ͨ��֤����ʱ��
        /// </summary>
        public DateTime ExpiredTime { get; set; }
    }
}
