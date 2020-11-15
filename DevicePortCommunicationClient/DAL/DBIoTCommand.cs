using System;
using System.Collections.Generic;
using System.Text;

namespace DevicePortCommunicationClient.DAL
{
    public class DBIoTCommand
    {
        public string Id
        {
            get;set;
        }

        /// <summary>
        /// 需要传输的指令
        /// </summary>
        public string Command
        {
            get; set;
        }

        /// <summary>
        /// 协议类型
        /// </summary>
        public int Protocol
        {
            get; set;
        }

        /// <summary>
        /// 用途
        /// </summary>
        public int Usage
        {
            get;set;
        }
    }
}
