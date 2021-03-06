﻿using System;
using System.Collections.Generic;
using System.Text;

namespace DevicePortCommunicationClient.Device
{
    /// <summary>
    /// 物联网指令
    /// </summary>
    public class IoTCommand
    {
        public string Id
        {
            get;set;
        }

        /// <summary>
        /// 需要传输的指令:16进制表示, 类似如下所示
        /// 01 02 02 02 d0
        /// </summary>
        public string Command
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

        public int Usage
        {
            get;set;
        }
    }
}
