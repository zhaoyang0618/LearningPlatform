using System;
using System.Collections.Generic;
using System.Text;

namespace DevicePortCommunicationClient.DAL
{
    public class DBCommModule
    {
        /// <summary>
        /// 唯一编码
        /// </summary>
        public string Id
        {
            get; set;
        }

        /// <summary>
        /// 名称
        /// </summary>
        public string Name
        {
            get; set;
        }

        /// <summary>
        /// IP地址
        /// </summary>
        public string IPAddress
        {
            get; set;
        }

        /// <summary>
        /// 通讯端口
        /// </summary>
        public int Port
        {
            get; set;
        }

        /// <summary>
        /// 指令集
        /// </summary>
        public List<DBIoTCommand> DBIoTCommands
        {
            get; set;
        }
    }
}
