using System;
using System.Collections.Generic;
using System.Text;

namespace DevicePortCommunicationClient.Device
{
    /// <summary>
    /// 物联网指令
    /// </summary>
    public class IoTCommand
    {
        /// <summary>
        /// 需要传输的指令
        /// </summary>
        public byte[] Command
        {
            get; set;
        }

        /// <summary>
        /// 协议类型
        /// </summary>
        public int CommandData
        {
            get;set;
        }
    }
}
