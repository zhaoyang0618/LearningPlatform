using System;
using System.Collections.Generic;
using System.Text;

namespace DevicePortCommunicationClient.DAL
{
    public class DBMachineDevice
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
        /// 一台设备可能存在多个需要通讯的模块
        /// </summary>
        public List<DBCommModule> DBCommModules
        {
            get; set;
        }
    }
}
